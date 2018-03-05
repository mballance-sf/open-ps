/*
 * SVClassGenerator.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#include "SVClassGenerator.h"
#include "FormattedFileOutput.h"
#include "QPSSCUtils.h"

namespace qpssc {

SVClassGenerator::SVClassGenerator(const std::string &outdir) {
	m_outdir = outdir;
	m_out = 0;
	m_emit_expr = false;
}

SVClassGenerator::~SVClassGenerator() {
	// TODO Auto-generated destructor stub
}

bool SVClassGenerator::process(IModel *model) {
	std::string rules_name;
	std::vector<std::string> cmdline;

	IComponent *comp;

	if (!QPSSCUtils::get_roots(model, &comp, &m_root_action)) {
		return false;
	}

	rules_name = comp->getName() + "_" + m_root_action->getName();

	FILE *sv_file_fp = fopen(
			std::string(rules_name + ".svh").c_str(), "w");
	FormattedFileOutput sv_file(sv_file_fp);
	m_out = &sv_file;

	m_out->println("// Forward declaration for interface class");
	m_out->println("typedef class %s_if;", rules_name.c_str());
	m_out->println("");

	m_out->println("class %s;", rules_name.c_str());
	m_out->inc_indent();

	// Fields
	m_out->println("%s_if m_if;", rules_name.c_str());
	m_out->println("");

	m_out->println("chandle m_te;");
	m_out->println("string  m_name;");
	m_out->println("");

	// Constructor
	m_out->println("function new(%s_if ifc);", rules_name.c_str());
	m_out->inc_indent();
	m_out->println("m_if = ifc;");
	m_out->dec_indent();
	m_out->println("endfunction");
	m_out->println("");

	generate_builtins();
	m_out->println("");

	// Generate exec methods
	m_pass = ProcessPass_ExecMethods;
	visit_model(model);

	// Generate the invoke_exec() task
	m_out->println("virtual task invoke_exec(int unsigned eid, string ctxt);");
	m_out->inc_indent();
	// Emit the decode statements that call action methods
	m_out->println("case (eid)");
	m_out->inc_indent();
	for (uint32_t i=0; i<m_exec_list.size(); i++) {
		m_out->println("%d: %s(ctxt);", (i+1), m_exec_list.at(i).c_str());
	}
	m_out->println("default: $display(\"Error: unknown exec ID %%0d\", eid);");
	m_out->dec_indent();
	m_out->println("endcase");
	m_out->dec_indent();
	m_out->println("endtask");
	m_out->println("");

	// Generate entry-point
	generate_run(rules_name);

	// Define registration method
	generate_registeraut(rules_name);
	m_out->println("");

	m_out->dec_indent();
	m_out->println("endclass");


	m_out->println("");

	m_out->println("class %s_if;", rules_name.c_str());
	m_out->println("");
	m_out->inc_indent();
	m_pass = ProcessPass_IfMethods;
	visit_model(model);

	m_out->dec_indent();
	m_out->println("endclass");
	m_out->println("");

	fclose(sv_file_fp);

	return true;
}

void SVClassGenerator::visit_action(IAction *a) {
	if (m_pass == ProcessPass_ExecMapping) {
	} else {
	}
	RulesPSIVisitor::visit_action(a);
}

void SVClassGenerator::visit_struct(IStruct *s) {
	if (m_pass == ProcessPass_ExecMapping) {
	} else {
	}
	RulesPSIVisitor::visit_struct(s);
}

void SVClassGenerator::visit_exec(IExec *e) {
	std::string type;
	switch (e->getExecKind()) {
	case IExec::Declaration: type = "declaration"; break;
	case IExec::Body: type = "body"; break;
	case IExec::PreSolve: type = "pre_solve"; break;
	case IExec::PostSolve: type = "post_solve"; break;
	}

	INamedItem *e_parent = dynamic_cast<INamedItem *>(e->getParent());

	std::string exec_name = "exec_";
	exec_name.append(e_parent->getName());
	exec_name.append("_");
	exec_name.append(type);

	if (m_pass == ProcessPass_ExecMethods) {
		m_emit_expr = true;

		m_out->println("virtual task %s(string ctxt);", exec_name.c_str());

		m_exec_list.push_back(exec_name);

		m_out->inc_indent();
//		m_out->println("$display(\"--> task %s(%%0s)\", ctxt);", exec_name.c_str());
		RulesPSIVisitor::visit_exec(e);
//		m_out->println("$display(\"<-- task %s(%%0s)\", ctxt);", exec_name.c_str());
		m_out->dec_indent();

		m_out->println("endtask");
		m_out->println("");

		m_emit_expr = false;
	} else if (m_pass == ProcessPass_ExecMapping) {
		switch (e->getExecKind()) {
		case IExec::Body: m_exec_path.push_back("__body"); break;
		case IExec::PreSolve: m_exec_path.push_back("__pre_solve"); break;
		case IExec::PostSolve: m_exec_path.push_back("__post_solve"); break;
		default: m_exec_path.push_back("__unsupported"); break;
		}

		std::string exec_path = list2path(m_exec_path);
		int32_t id = -1;

		for (uint32_t i=0; i<m_exec_list.size(); i++) {
			if (m_exec_list.at(i) == exec_name) {
				id = (i+1);
				break;
			}
		}

		// Emit registration from action path to id
		m_out->println("qpsrt::set_method_id(m_te, \"%s\", %d);", exec_path.c_str(), id);

		m_exec_path.pop_back();
	}
}


void SVClassGenerator::visit_exec_call_stmt(IMethodCallExpr *s) {
	m_out->println("begin");
	m_out->inc_indent();

	if (s->getFunc()->hasAttribute("BUILTIN")) {
		// Don't qualify
		m_out->println("%s(", s->getFunc()->getName().c_str());
	} else {
		m_out->println("m_if.%s(", s->getFunc()->getName().c_str());
	}

	m_out->inc_indent();
	for (uint32_t i=0; i<s->getParameters().size(); i++) {
		// TODO: need an SV version of visit_expr()
		m_out->print(m_out->indent());
		visit_expr(s->getParameters().at(i));
		m_out->print("%s\n", (i+1<s->getParameters().size())?",":"");
	}
	m_out->dec_indent();
	m_out->println(");\n");

	m_out->dec_indent();
	m_out->println("end");
}

void SVClassGenerator::visit_exec_expr_stmt(IExecExprStmt *s) {

	if (s->getRhs()) {
		IFieldRef *t = dynamic_cast<IFieldRef *>(s->getLhs());

		if (!t) {
			fprintf(stdout, "Error: ExecExprStmt::LHS is not a field ref\n");
		}
		m_out->print(m_out->indent().c_str());

		m_out->print("qpsrt::set_field(m_te, {ctxt, \".%s\"}, ", fieldref2string(t).c_str());

		switch (s->getOp()) {
		case IExecExprStmt::AssignOp_PlusEq:
			m_out->print("qpsrt::get_field(m_te, {ctxt, \".%s\"})+",
					fieldref2string(t).c_str());
			break;
		case IExecExprStmt::AssignOp_AndEq:
			m_out->print("qpsrt::get_field(m_te, {ctxt, \".%s\"})&",
					fieldref2string(t).c_str());
			break;
		case IExecExprStmt::AssignOp_LShiftEq:
			m_out->print("qpsrt::get_field(m_te, {ctxt, \".%s\"})<<",
					fieldref2string(t).c_str());
			break;
		case IExecExprStmt::AssignOp_MinusEq:
			m_out->print("qpsrt::get_field(m_te, {ctxt, \".%s\"})-",
					fieldref2string(t).c_str());
			break;
		case IExecExprStmt::AssignOp_OrEq:
			m_out->print("qpsrt::get_field(m_te, {ctxt, \"%s\"})|",
					fieldref2string(t).c_str());
			break;
		case IExecExprStmt::AssignOp_RShiftEq:
			m_out->print("qpsrt::get_field(m_te, {ctxt, \".%s\"})>>", fieldref2string(t).c_str());
			break;
		}
		visit_expr(s->getRhs());
		m_out->print(");\n");
	} else {
		// Single-statement expression
		fprintf(stdout, "TODO: single-statement exec expr statement\n");
	}
}

void SVClassGenerator::visit_rules_exec_if(RulesExecStmtIfItem *s) {
	fprintf(stdout, "visit_rules_exec_if\n");
	fflush(stdout);
	m_emit_expr = true;
	m_out->print("%sif (", m_out->indent().c_str());
	RulesPSIVisitor::visit_expr(s->getCond());
	m_out->print(") begin\n");
	m_out->inc_indent();
	for (std::vector<IExecStmt *>::const_iterator it=s->getTrue().begin();
			it!=s->getTrue().end(); it++) {
		RulesPSIVisitor::visit_exec_stmt(*it);
	}
	m_out->dec_indent();
	if (s->getFalse().size() != 0) {
		m_out->println("end else begin");
		m_out->inc_indent();
		for (std::vector<IExecStmt *>::const_iterator it=s->getFalse().begin();
				it!=s->getFalse().end(); it++) {
			RulesPSIVisitor::visit_exec_stmt(*it);
		}
		m_out->dec_indent();
	}
	m_out->println("end");
}

void SVClassGenerator::visit_field(IField *f) {
	if (m_pass == ProcessPass_ExecMapping) {
		m_exec_path.push_back(f->getName());
		if (f->getDataType()->getType() == IBaseItem::TypeAction) {
			visit_action(dynamic_cast<IAction *>(f->getDataType()));
		} else if (f->getDataType()->getType() == IBaseItem::TypeStruct) {
			visit_struct(dynamic_cast<IStruct *>(f->getDataType()));
		}
		m_exec_path.pop_back();
	} else {
		RulesPSIVisitor::visit_field(f);
	}
}

void SVClassGenerator::visit_expr(IExpr *e) {
	if (m_emit_expr) {
		RulesPSIVisitor::visit_expr(e);
	}
}

void SVClassGenerator::visit_binary_expr(IBinaryExpr *be) {
	if (be->getBinOpType() == IBinaryExpr::BinOp_ArrayRef) {
	} else {
		std::string op = "UNKNOWN";
		switch (be->getBinOpType()) {
		case IBinaryExpr::BinOp_And: op = "&"; break;
		case IBinaryExpr::BinOp_AndAnd: op = "&&"; break;
		case IBinaryExpr::BinOp_Divide: op = "/"; break;
		case IBinaryExpr::BinOp_Eq: op = "="; break;
		case IBinaryExpr::BinOp_EqEq: op = "=="; break;
		case IBinaryExpr::BinOp_GE: op = ">="; break;
		case IBinaryExpr::BinOp_GT: op = ">"; break;
		case IBinaryExpr::BinOp_LE: op = "<="; break;
		case IBinaryExpr::BinOp_LT: op = "<"; break;
		case IBinaryExpr::BinOp_Minus: op = "-"; break;
		case IBinaryExpr::BinOp_Mod: op = "%"; break;
		case IBinaryExpr::BinOp_Multiply: op = "*"; break;
		case IBinaryExpr::BinOp_NotEq: op = "!="; break;
		case IBinaryExpr::BinOp_Or: op = "|"; break;
		case IBinaryExpr::BinOp_OrOr: op = "||"; break;
		case IBinaryExpr::BinOp_Plus: op = "+"; break;
		}

		m_out->print("(");
		visit_expr(be->getLHS());
		m_out->print(op.c_str());
		visit_expr(be->getRHS());
		m_out->print(")");
	}
}

void SVClassGenerator::visit_fieldref_expr(IFieldRef *ref) {
	m_out->print("qpsrt::get_field(m_te, {ctxt, \".%s\"})", fieldref2string(ref).c_str());
}

void SVClassGenerator::visit_literal_expr(ILiteral *l) {
	switch (l->getLiteralType()) {
	case ILiteral::LiteralBit: m_out->print("%llx", (unsigned long long)l->getBit()); break;
	case ILiteral::LiteralInt: m_out->print("%lld", (long long)l->getInt()); break;
	case ILiteral::LiteralBool: m_out->print("%s", (l->getBool())?"true":"false"); break;
	case ILiteral::LiteralString: m_out->print(l->getString().c_str());
	}
}

void SVClassGenerator::visit_import_func(IImportFunc *f) {
	if (m_pass != ProcessPass_IfMethods) {
		return;
	}

	// Builtins are provided
	if (f->hasAttribute("BUILTIN")) {
		return;
	}

	m_out->print(m_out->indent());
	m_out->print("virtual task %s(\n", f->getName().c_str());
	m_out->inc_indent();
	// First parameter is the return (if present)
	if (f->getReturn()) {
		m_out->println("output %s ret%s // return value",
				type2string(f->getReturn()->getDataType()).c_str(),
				(f->getParameters().size()>0)?",":"");
	}

	for (uint32_t i=0; i<f->getParameters().size(); i++) {
		IField *p = f->getParameters().at(i);
		std::string dir = "input";

		switch (p->getAttr()) {
		case IField::FieldAttr_Input:  dir = "input";  break;
		case IField::FieldAttr_Output: dir = "output"; break;
		case IField::FieldAttr_Inout:  dir = "inout";  break;
		}

		m_out->println("%s %s %s%s",
				dir.c_str(),
				type2string(p->getDataType()).c_str(),
				p->getName().c_str(),
				(i+1<f->getParameters().size())?",":"");
	}
	m_out->dec_indent();
	m_out->print("%s);\n", m_out->indent().c_str());

	m_out->inc_indent();
	m_out->println("$display(\"Error: Method %s is unimplemented\");",
			f->getName().c_str());
	m_out->println("$finish();");
	m_out->dec_indent();

	m_out->print("%sendtask\n", m_out->indent().c_str());

	m_out->println("");
}

void SVClassGenerator::visit_rules_action_exec_item(RulesExecActionItem *it) {
	fprintf(stdout, "visit_rules_action_exec_item()\n");
	fflush(stdout);

	std::string exec_name = "exec_";
	exec_name.append(it->getName());

	if (m_pass == ProcessPass_ExecMethods) {
		m_emit_expr = true;

		m_out->println("virtual task %s(string ctxt);", exec_name.c_str());

		m_exec_list.push_back(exec_name);

		m_out->inc_indent();
//		m_out->println("$display(\"--> task %s(%%0s)\", ctxt);", exec_name.c_str());
		RulesPSIVisitor::visit_exec(it->getExec());
//		m_out->println("$display(\"<-- task %s(%%0s)\", ctxt);", exec_name.c_str());
		m_out->dec_indent();

		m_out->println("endtask");
		m_out->println("");

		m_emit_expr = false;
	} else if (m_pass == ProcessPass_ExecMapping) {
		m_exec_path.push_back(it->getName());

		std::string exec_path = list2path(m_exec_path);
		int32_t id = -1;

		for (uint32_t i=0; i<m_exec_list.size(); i++) {
			if (m_exec_list.at(i) == exec_name) {
				id = (i+1);
				break;
			}
		}

		// Emit registration from action path to id
		m_out->println("qpsrt::set_method_id(m_te, \"%s\", %d);", exec_path.c_str(), id);

		m_exec_path.pop_back();
	}
}

void SVClassGenerator::generate_builtins() {
	m_out->println("function void stop_loop_expansion();");
	m_out->inc_indent();
//	m_out->println("$display(\"--> stop_loop_expansion\");");
	m_out->println("qpsrt::stop_loop_expansion(m_te);");
//	m_out->println("$display(\"<-- stop_loop_expansion\");");
	m_out->dec_indent();
	m_out->println("endfunction");
	m_out->println("");
}

void SVClassGenerator::generate_run(const std::string &rules_name) {
	m_out->println("task run();");
	m_out->inc_indent();

	m_out->println("registeraut();");
	m_out->println("");

	// Create a new engine
	m_out->println("m_te = qpsrt::create_engine(\"%s\", m_name);\n", rules_name.c_str());
	m_out->println("");

	// Map instance paths to action IDs
	m_pass = ProcessPass_ExecMapping;
	m_exec_path.clear();
	m_exec_path.push_back("__root");
	visit_action(m_root_action);

	// Now, create the main loop
	m_out->println("begin");
	m_out->inc_indent();
	m_out->println("string ctxt;");
	m_out->println("int unsigned eid;");
	m_out->println("");
	m_out->println("while (qpsrt::get_next_method(m_te, eid, ctxt)) begin");
	m_out->inc_indent();
//	m_out->println("$display(\"Method EID %%0d\", eid);");
	m_out->println("invoke_exec(eid, ctxt);");

	m_out->println("qpsrt::end_of_method(m_te);");

	m_out->dec_indent();
	m_out->println("end");
	m_out->println("");

	m_out->dec_indent();
	m_out->println("end");
	m_out->println("");

	m_out->println("qpsrt::delete_engine(m_te);");
	m_out->println("");



	m_out->dec_indent();
	m_out->println("endtask");

	m_out->println("");
}

void SVClassGenerator::generate_registeraut(const std::string &rules_name) {
	m_out->println("static bit registered = 0;");
	m_out->println("function registeraut();");
	m_out->inc_indent();
	m_out->println("if (!registered) begin");
	m_out->inc_indent();
	m_out->println("chandle hndl = null;");

	char tmp[64];
	char line[128];
	size_t len;
#ifndef UNIT_TEST
	FILE *fp = fopen(std::string(m_outdir + "/" + rules_name + ".aut").c_str(), "rb");
#else
	FILE *fp = fopen(filename, "rb");
#endif

	while ((len = fread(tmp, 1, 45, fp)) > 0) {
		uint32_t lp = 0;
		line[lp++] = len+32; // length

		// Pack 3 bytes as 4 characters
		for (uint32_t i=0; i<len; i+=3) {
			uint32_t td = (((uint32_t)tmp[i]) & 0xFF);

			if (i+1 < len) {
				td |= ((((uint32_t)tmp[i+1]) & 0xFF) << 8);
			}

			if (i+2 < len) {
				td |= ((((uint32_t)tmp[i+2]) & 0xFF) << 16);
			}

			// Now, break down into four characters
			for (uint32_t j=0; j<4; j++) {
				line[lp++] = ((td >> 6*j) & 0x3F) + 32;
#ifndef UNIT_TEST
				if (line[lp-1] == '"' || line[lp-1] == '\\') {
					line[lp] = line[lp-1];
					line[lp-1] = '\\';
					lp++;
				}
#endif
			}
		}

		line[lp] = 0;
#ifndef UNIT_TEST
		m_out->println("hndl = qpsrt::load_data(hndl, \"%s\");", line);
#else
		data[data_idx++] = strdup(line);
#endif
	}

	m_out->println("registered = 1;");
	m_out->println("qpsrt::load_data_done(hndl);");
	fclose(fp);

	m_out->dec_indent();
	m_out->println("end");
	m_out->dec_indent();
	m_out->println("endfunction");
}

std::string SVClassGenerator::type2string(IBaseItem *t) {
	std::string ret;

	switch (t->getType()) {
	case IBaseItem::TypeScalar: {
		IScalarType *st = dynamic_cast<IScalarType *>(t);
		switch (st->getScalarType()) {
		case IScalarType::ScalarType_Bit:
			ret = "longint unsigned";
			break;

		case IScalarType::ScalarType_Bool:
			ret = "bit";
			break;

		case IScalarType::ScalarType_Chandle:
			ret = "chandle";
			break;

		case IScalarType::ScalarType_Int:
			ret = "longint";
			break;

		case IScalarType::ScalarType_String:
			ret = "string";
			break;
		}
	} break;

	case IBaseItem::TypeStruct: {
		IStruct *s = dynamic_cast<IStruct *>(t);
		ret = s->getName();
	} break;
	}

	return ret;
}

std::string SVClassGenerator::fieldref2string(IFieldRef *r) {
	std::string ret;

	for (uint32_t i=0; i<r->getFieldPath().size(); i++) {
		IField *f = r->getFieldPath().at(i);
		ret.append(f->getName());

		if (i+1<r->getFieldPath().size()) {
			ret.append(".");
		}
	}

	return ret;
}

std::string SVClassGenerator::list2path(const std::vector<std::string> &l) {
	std::string ret;

	for (uint32_t i=0; i<l.size(); i++) {
		ret.append(l.at(i));

		if (i+1<l.size()) {
			ret.append(".");
		}
	}

	return ret;
}

} /* namespace qpssc */
