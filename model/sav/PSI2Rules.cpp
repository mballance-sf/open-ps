/*
 * PSI2Rules.cpp
 *
 *  Created on: Aug 12, 2016
 *      Author: ballance
 */

#include "PSI2Rules.h"
#include "QPSSCUtils.h"
#include <stdio.h>

namespace qpssc {

PSI2Rules::PSI2Rules(const std::string &outdir) {
	m_root_a = 0;
	m_root_c = 0;
	m_outdir = outdir;
	m_anon_constraint_id = 0;
}

PSI2Rules::~PSI2Rules() {
	// TODO Auto-generated destructor stub
}

bool PSI2Rules::process(IModel *model) {
	m_root_a = 0;
	m_root_c = 0;

	if (!QPSSCUtils::get_roots(model, &m_root_c, &m_root_a)) {
		fprintf(stdout, "Error: Failed to find roots\n");
		return false;
	}

	if (model->hasAttribute("RULES_NAME")) {
		m_rules_name = model->getAttribute("RULES_NAME");
	} else {
		m_rules_name = m_root_c->getName() + "_" + m_root_a->getName();
	}

	std::string outfile = m_outdir + "/" + m_rules_name + ".rules";
	FILE *fp = fopen(outfile.c_str(), "w");

	m_out = new FormattedFileOutput(fp);

	visit_model(model);

	m_out->flush();
	fclose(fp);

	return true;
}

void PSI2Rules::visit_model(IModel *model) {

	println("rule_graph " + m_rules_name + " {");
	m_out->inc_indent();

	// First, global actions
	println("");
	RulesPSIVisitor::visit_model(model);
	println("");

	// Make a root-action instance
	println(m_root_a->getName() + " __root;");
	println("");
	println(m_rules_name + " = __root;");
	m_out->dec_indent();
	println("}");
	println("");
}

void PSI2Rules::visit_package(IPackage *pkg) {
// For now, just assume a flat namespace without implementing it using mangling
//	if (pkg->getName() != "") {
//		fprintf(stdout, "Error: encountered a package\n");
//	} else {
		RulesPSIVisitor::visit_package(pkg);
//	}
}

void PSI2Rules::visit_action(IAction *a) {
	m_active_a = a;

	// Declare a struct to contain the elements
	std::string action_decl = "struct ";
	action_decl += a->getName();

	if (a->getSuperType()) {
		action_decl += " extends ";
		action_decl += a->getSuperType()->getName();
	}

	action_decl += " {";
	println(action_decl);
	m_out->inc_indent();


	// If we have exec blocks, then we must stich those into the
	// struct/action structure as actions.
	bool have_pre_solve = false;
	bool have_post_solve = false;
	bool have_body = false;

	for (std::vector<IBaseItem *>::const_iterator it=a->getItems().begin();
			it!=a->getItems().end(); it++) {
		IExec *exec;

		if ((exec=dynamic_cast<IExec *>(*it))) {
			switch (exec->getExecKind()) {
			case IExec::PreSolve: have_pre_solve = true; break;
			case IExec::PostSolve: have_post_solve = true; break;
			case IExec::Body: have_body = true; break;
			}
		}
	}

	RulesPSIVisitor::visit_action(a);

	// Declare special actions first
	if (have_pre_solve) {
		m_out->println("action __pre_solve;");
	}
	if (have_post_solve) {
		m_out->println("action __post_solve;");
	}
	if (have_body) {
		m_out->println("action __body;");
	}

	// Have to build a graph for this
	m_out->println("%s = ", a->getName().c_str());
	m_out->inc_indent();

	if (have_pre_solve) {
		m_out->println("__pre_solve");
	}

	// List rand fields
	for (std::vector<IBaseItem *>::const_iterator it=a->getItems().begin();
			it!=a->getItems().end(); it++) {
		IField *field;

		if ((*it)->getType() == IBaseItem::TypeField) {
			field = dynamic_cast<IField *>(*it);
			if (field->getAttr() == IField::FieldAttr_Rand) {
				m_out->println(field->getName().c_str());
			}
		}
	}

	if (have_post_solve) {
		m_out->println("__post_solve");
	}

	if (a->getGraph()) {
		// An explicit graph is specified.
		RulesPSIVisitor::visit_graph(a->getGraph());
	}

	if (have_body) {
		println("__body");
	}

	m_out->dec_indent();
	m_out->println(";");
	m_out->println("");

	m_out->dec_indent();
	println("}");
	println("");

}

void PSI2Rules::visit_constraint(IConstraintBlock *c) {
	std::string id;
	char tmp[64];

	if (c->getName() == "") {
		sprintf(tmp, "__anon_%d", m_anon_constraint_id);
		m_anon_constraint_id++;
		id = tmp;
	} else {
		id = c->getName();
	}

	m_out->println("constraint %s {", id.c_str());
	m_out->inc_indent();

	PSIVisitor::visit_constraint(c);

	m_out->dec_indent();
	println("}");

}

void PSI2Rules::visit_constraint_expr_stmt(IConstraintExpr *c) {
	m_out->print(m_out->indent().c_str());
	PSIVisitor::visit_constraint_expr_stmt(c);
	m_out->print(";\n");
}

void PSI2Rules::visit_constraint_if_stmt(IConstraintIf *c) {
	m_out->print(m_out->indent().c_str());
	m_out->print("if (");
	visit_expr(c->getCond());
	m_out->print(") {\n");
	m_out->inc_indent();
	m_out->dec_indent();
	if (c->getFalse()) {
		println("} else {");
		m_out->inc_indent();
		visit_constraint_stmt(c->getFalse());
		m_out->dec_indent();
		println("}");
	} else {
		println("}");
	}
}

void PSI2Rules::visit_binary_expr(IBinaryExpr *be) {
	if (be->getBinOpType() == IBinaryExpr::BinOp_ArrayRef) {
		visit_expr(be->getLHS());
		m_out->print("[");
		visit_expr(be->getRHS());
		m_out->print("]");
	} else {
		visit_expr(be->getLHS());
		m_out->print(" ");
		m_out->print(binop_tostring(be->getBinOpType()));
		m_out->print(" ");
		visit_expr(be->getRHS());
	}
}

void PSI2Rules::visit_fieldref_expr(IFieldRef *ref) {
	m_out->print(path2string(ref).c_str());
}

void PSI2Rules::visit_literal_expr(ILiteral *l) {
	switch (l->getLiteralType()) {
	case ILiteral::LiteralBit: {
		m_out->print("0x%llx", (long long)l->getBit());
	} break;

	case ILiteral::LiteralInt: {
		m_out->print("%lld", l->getInt());
	} break;

	case ILiteral::LiteralBool: {
		m_out->print((l->getBool())?"true":"false");
	} break;

	default:
		m_out->print("UNKNOWN(%d)", l->getLiteralType());
	break;
	}
}

void PSI2Rules::visit_struct(IStruct *str) {
	std::string str_decl = "struct ";
	str_decl += str->getName();

	if (str->getSuperType()) {
		str_decl += " extends ";
		str_decl += str->getSuperType()->getName();
	}

	str_decl += " {";
	println(str_decl);
	m_out->inc_indent();

	RulesPSIVisitor::visit_struct(str);

	// If we have exec blocks, then we must stich those into the
	// struct/action structure as actions.
	bool have_pre_solve = false;
	bool have_post_solve = false;
	bool have_body = false;

	for (std::vector<IBaseItem *>::const_iterator it=str->getItems().begin();
			it!=str->getItems().end(); it++) {
		IExec *exec;

		if ((exec=dynamic_cast<IExec *>(*it))) {
			switch (exec->getExecKind()) {
			case IExec::PreSolve: have_pre_solve = true; break;
			case IExec::PostSolve: have_post_solve = true; break;
			case IExec::Body: have_body = true; break;
			}
		}
	}

	if (have_pre_solve) {
		println("action __pre_solve;");
	}
	if (have_post_solve) {
		println("action __post_solve;");
	}
	if (have_body) {
		println("action __body;");
	}

	if (have_pre_solve || have_post_solve || have_body) {
		println("");

		// Have to build a graph for this
		println(str->getName() + " = ");
		m_out->inc_indent();
		if (have_pre_solve) {
			println("__pre_solve");
		}
		// List fields
		for (std::vector<IBaseItem *>::const_iterator it=str->getItems().begin();
				it!=str->getItems().end(); it++) {
			IField *field;

			if ((field = dynamic_cast<IField *>(*it))) {
				println(field->getName());
			}
		}

		if (have_post_solve) {
			println("__post_solve");
		}
		if (have_body) {
			println("__body");
		}

		m_out->dec_indent();
		println(";");
		println("");
	}

	m_out->dec_indent();
	println("}");
	println("");
}

void PSI2Rules::visit_exec(IExec *e) {
	// Do nothing
}

void PSI2Rules::visit_extend(IExtend *e) {
	// Do nothing
}

void PSI2Rules::visit_field(IField *f) {
	// Convert non-rand fields to actions
	IBaseItem *dt = f->getDataType();
	char tmp[256];

	if (f->getAttr() != IField::FieldAttr_Rand &&
			(dt->getType() != IBaseItem::TypeAction &&
					dt->getType() != IBaseItem::TypeStruct)) {
		println("action " + f->getName() + ";");
	} else {
		if (dt->getType() == IBaseItem::TypeStruct ||
				dt->getType() == IBaseItem::TypeAction) {
			INamedItem *ni = dynamic_cast<INamedItem *>(dt);
			println(ni->getName() + " " + f->getName() + ";");
		} else if (dt->getType() == IBaseItem::TypeScalar) {
			// scalar field
			IScalarType *st = dynamic_cast<IScalarType *>(dt);

			switch (st->getScalarType()) {
			case IScalarType::ScalarType_Bit: {
				m_out->print(m_out->indent().c_str());

				m_out->print("meta_action " + f->getName() + "[unsigned ");
				visit_expr(st->getMSB());
				m_out->print(":");
				visit_expr(st->getLSB());
				m_out->print("];\n");
			} break;
			case IScalarType::ScalarType_Int: {
				m_out->print(m_out->indent().c_str());
				m_out->print("meta_action " + f->getName() + "[");
				visit_expr(st->getMSB());
				m_out->print(":");
				visit_expr(st->getLSB());
				m_out->print("];\n");
			} break;
			case IScalarType::ScalarType_Bool: {
				m_out->print(m_out->indent().c_str());
				m_out->print("meta_action " + f->getName() + "[unsigned enum false, true];\n");
			} break;

			default:
				fprintf(stdout, "Error: unsupported field data-type %d\n",
						st->getScalarType());
				break;
			}
		} else {
			fprintf(stdout, "Error: Unsupported data type\n");
		}
	}

}

void PSI2Rules::visit_graph(IActivityStmt *graph) {
//	m_out->println("%s = ", m_active_a->getName().c_str());
//	m_out->inc_indent();
//	RulesPSIVisitor::visit_graph(graph);
//	m_out->dec_indent();
//	m_out->println(";");
}

void PSI2Rules::visit_graph_parallel_block_stmt(IActivityBlockStmt *block) {
	for (std::vector<IActivityStmt *>::const_iterator it=block->getStmts().begin();
			it!=block->getStmts().end(); it++) {
		RulesPSIVisitor::visit_graph_stmt(*it);
	}
}

void PSI2Rules::visit_graph_select_stmt(IActivityBlockStmt *s) {
	m_out->println("(");
	m_out->inc_indent();
	for (uint32_t i=0; i<s->getStmts().size(); i++) {
		if (i > 0) {
			m_out->println("| (");
		} else {
			m_out->println("(");
		}
		m_out->inc_indent();
		RulesPSIVisitor::visit_graph_stmt(s->getStmts().at(i));
		m_out->dec_indent();
		m_out->println(")");
	}
	m_out->dec_indent();
	m_out->println(")");
}

void PSI2Rules::visit_graph_repeat_stmt(IActivityRepeatStmt *r) {
	m_out->println("repeat {");
	m_out->inc_indent();
	RulesPSIVisitor::visit_graph_repeat_stmt(r);
	m_out->dec_indent();
	m_out->println("}");
}

void PSI2Rules::visit_activity_traverse_stmt(IActivityTraverseStmt *t) {
	if (t->getWith()) {
		m_out->println("%s // TODO: with",
				fieldref2string(t->getAction()).c_str());
	} else {
		m_out->println("%s",
				fieldref2string(t->getAction()).c_str());
	}
}

void PSI2Rules::visit_rules_action_exec_item(RulesExecActionItem *it) {
	// In rules, an action exec is just an action
	m_out->println("action %s;", it->getName().c_str());

}

void PSI2Rules::println(const std::string &s) {
	m_out->println(s.c_str());
}

std::string PSI2Rules::binop_tostring(IBinaryExpr::BinOpType t) {
	switch (t) {
	case IBinaryExpr::BinOp_Eq: return "=";
	case IBinaryExpr::BinOp_EqEq: return "==";
	case IBinaryExpr::BinOp_NotEq: return "!=";
	case IBinaryExpr::BinOp_GE: return ">=";
	case IBinaryExpr::BinOp_GT: return ">";
	case IBinaryExpr::BinOp_LE: return "<=";
	case IBinaryExpr::BinOp_LT: return "<";
	case IBinaryExpr::BinOp_And: return "&";
	case IBinaryExpr::BinOp_AndAnd: return "&&";
	case IBinaryExpr::BinOp_Or: return "|";
	case IBinaryExpr::BinOp_OrOr: return "||";
	case IBinaryExpr::BinOp_Minus: return "-";
	case IBinaryExpr::BinOp_Plus: return "+";
	case IBinaryExpr::BinOp_Multiply: return "*";
	case IBinaryExpr::BinOp_Divide: return "/";
	case IBinaryExpr::BinOp_Mod: return "%";
	}

	return "UNKNOWN_OP";
}

std::string PSI2Rules::fieldref2string(IFieldRef *r) {
	std::string ret;

	for (uint32_t i=0; i<r->getFieldPath().size(); i++) {
		ret.append(r->getFieldPath().at(i)->getName());
		if (i+1 < r->getFieldPath().size()) {
			ret.append(".");
		}
	}

	return ret;
}

//std::string PSI2Rules::type2rulestype(IBaseItem *type) {
//	char tmp[64];
//	switch (type->getType()) {
//	case IBaseItem::TypeStruct: return dynamic_cast<IStruct *>(type)->getName();
//	case IBaseItem::TypeAction: return dynamic_cast<IAction *>(type)->getName();
//	case IBaseItem::TypeScalar: {
//		std::string ret = "meta_action[";
//		IScalarType *st = dynamic_cast<IScalarType *>(type);
//
//		switch (st->getScalarType()) {
//			case IScalarType::ScalarType_Bit: {
//
//			} break;
//
//			case IScalarType::ScalarType_Bool: {
//				ret += "unsigned enum false, true];";
//			} break;
//
//			case IScalarType::ScalarType_Int:
//		}
//
//	} break;
//
//	default:
//		return "<unknown_type>";
//	}
//}

} /* namespace qpss */
