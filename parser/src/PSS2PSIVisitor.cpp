/*
 * PSS2PSIVisitor.cpp
 *
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#include "PSS2PSIVisitor.h"
#include <cstdio>
using namespace antlrcpp;


PSS2PSIVisitor::PSS2PSIVisitor(IModel *model, const std::string &path) :
		m_model(model), m_file(path) {
}

PSS2PSIVisitor::~PSS2PSIVisitor() {
	// TODO Auto-generated destructor stub
}

antlrcpp::Any PSS2PSIVisitor::visitModel(PSSParser::ModelContext *ctx) {
	push_scope(m_model);
	for (uint32_t i=0; i<ctx->portable_stimulus_description().size(); i++) {
		IBaseItem *it = ctx->portable_stimulus_description(i)->accept(this);
		if (it && it->getType() != IBaseItem::TypePackage) {
			m_model->add(it);
		}
	}
	pop_scope();

	return 0;
}

antlrcpp::Any PSS2PSIVisitor::visitAction_declaration(PSSParser::Action_declarationContext *ctx) {
	IAction *super_type = 0;
	IBaseItem *ret = 0;

	if (ctx->action_super_spec()) {
		fprintf(stdout, "TODO: find super spec\n");
	}

	IAction *action = m_model->mkAction(
			ctx->action_identifier()->identifier()->ID()->toString(),
			super_type);
	ret = action;

	push_scope(action);

	for (uint32_t i=0; i<ctx->action_body_item().size(); i++) {
		IBaseItem *it = ctx->action_body_item(i)->accept(this);

		if (it) {
			action->add(it);
		}
	}
	pop_scope();

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitAction_data_type(PSSParser::Action_data_typeContext *ctx) {
	IBaseItem *ret = 0;

	if (ctx->scalar_data_type()) {
		ret = ctx->scalar_data_type()->accept(this);
	} else if (ctx->user_defined_datatype()) {
		ret = ctx->user_defined_datatype()->accept(this);
	} else {
		fprintf(stdout, "Error: Unknown action data type\n");
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_declaration(PSSParser::Activity_declarationContext *ctx) {
	IBaseItem *ret = 0;

	IAction *action = dynamic_cast<IAction *>(scope());
	if (ctx->activity_stmt().size() == 0 || ctx->activity_stmt().size() > 1) {
		IGraphBlockStmt *block = m_model->mkGraphBlockStmt(IGraphBlockStmt::GraphStmt_Block);
		for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
			block->add(ctx->activity_stmt(i)->accept(this));
		}
		action->setGraph(block);
	} else {
		action->setGraph(ctx->activity_stmt(0)->accept(this));
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_action_traversal_stmt(PSSParser::Activity_action_traversal_stmtContext *ctx) {
	IGraphStmt *ret = 0;
	IConstraint *with_c = 0;

	if (ctx->inline_with_constraint()) {
		fprintf(stdout, "TODO: inline constraint\n");
	}

	if (ctx->is_do) {
		// Do <type>
	} else {
		// <type>
		std::vector<PSSParser::Variable_refContext *> path;
		path.push_back(ctx->variable_ref());
		ret = m_model->mkGraphTraverseStmt(elaborate_field_ref(path), with_c);
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_parallel_stmt(PSSParser::Activity_parallel_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	IGraphBlockStmt *parallel = m_model->mkGraphBlockStmt(IGraphStmt::GraphStmt_Parallel);

	for (uint32_t i=0; i<ctx->activity_labeled_stmt().size(); i++) {
		IGraphStmt *stmt = ctx->activity_labeled_stmt(i)->activity_stmt()->accept(this);
		if (dynamic_cast<IGraphBlockStmt *>(stmt) &&
				dynamic_cast<IGraphBlockStmt *>(stmt)->getStmtType() == IGraphBlockStmt::GraphStmt_Block) {
			parallel->add(stmt);
		} else {
			IGraphBlockStmt *stmt_b = m_model->mkGraphBlockStmt(IGraphStmt::GraphStmt_Block);
			stmt_b->add(stmt);
			parallel->add(stmt_b);
		}
	}
	ret = parallel;

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_repeat_stmt(PSSParser::Activity_repeat_stmtContext *ctx) {
	IGraphStmt *ret = 0;
	IExpr *expr = 0;
	IGraphRepeatStmt::RepeatType type = IGraphRepeatStmt::RepeatType_Forever;
	IGraphBlockStmt *body = m_model->mkGraphBlockStmt();

	if (ctx->expression()) {
		expr = ctx->expression()->accept(this);
	}

	if (ctx->is_while) {
		type = IGraphRepeatStmt::RepeatType_While;
	} else if (ctx->is_do_while) {
		type = IGraphRepeatStmt::RepeatType_Until;
	} else if (expr) {
		if (ctx->loop_var) {
			// TODO: must allow loop variable
		}
		type = IGraphRepeatStmt::RepeatType_Count;
	}

	for (uint32_t i=0; i<ctx->activity_sequence_block_stmt()->activity_stmt().size(); i++) {
		body->add(ctx->activity_sequence_block_stmt()->activity_stmt(i)->accept(this));
	}

	ret = m_model->mkGraphRepeatStmt(type, expr, body);

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_select_stmt(PSSParser::Activity_select_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	IGraphBlockStmt *select = m_model->mkGraphBlockStmt(IGraphStmt::GraphStmt_Select);

	for (uint32_t i=0; i<ctx->activity_labeled_stmt().size(); i++) {
		IGraphStmt *stmt = ctx->activity_labeled_stmt(i)->activity_stmt()->accept(this);
		if (dynamic_cast<IGraphBlockStmt *>(stmt) &&
				dynamic_cast<IGraphBlockStmt *>(stmt)->getStmtType() == IGraphBlockStmt::GraphStmt_Block) {
			select->add(stmt);
		} else {
			IGraphBlockStmt *stmt_b = m_model->mkGraphBlockStmt(IGraphStmt::GraphStmt_Block);
			stmt_b->add(stmt);
			select->add(stmt_b);
		}
	}

	ret = select;
    return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_sequence_block_stmt(PSSParser::Activity_sequence_block_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	IGraphBlockStmt *seq = m_model->mkGraphBlockStmt(IGraphStmt::GraphStmt_Block);
	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		seq->add(ctx->activity_stmt(i)->accept(this));
	}
	ret = seq;

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitStruct_declaration(PSSParser::Struct_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IStruct *super_type = 0;

	if (ctx->struct_super_spec()) {
		fprintf(stdout, "TODO: find super spec\n");
	}

	IStruct::StructType t;
	if (ctx->struct_type()->getText() == "struct") {
		t = IStruct::Base;
	} else if (ctx->struct_type()->getText() == "buffer") {
		t = IStruct::Memory;
	} else if (ctx->struct_type()->getText() == "stream") {
		t = IStruct::Stream;
	} else if (ctx->struct_type()->getText() == "state") {
		t = IStruct::State;
	} else if (ctx->struct_type()->getText() == "resource") {
		t = IStruct::Resource;
	} else {
		fprintf(stdout, "Unknown struct type \"%s\"\n",
				ctx->struct_type()->img->getText().c_str());
	}

	IStruct *s = m_model->mkStruct(
			ctx->identifier()->ID()->toString(),
			t, super_type);
	push_scope(s);
	ret = s;

	for (uint32_t i=0; i<ctx->struct_body_item().size(); i++) {
		IBaseItem *it = ctx->struct_body_item(i)->accept(this);
		if (it) {
			s->add(it);
		}
	}

	pop_scope();

    return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitStruct_field_declaration(PSSParser::Struct_field_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IField::FieldAttr attr = IField::FieldAttr_None;

	if (ctx->struct_field_modifier()) {
		if (ctx->struct_field_modifier()->getText() == "rand") {
			attr = IField::FieldAttr_Rand;
		} else {
			fprintf(stdout, "Unknown field modifier \"%s\"\n",
					ctx->struct_field_modifier()->getText().c_str());
		}
	}

	IBaseItem *data_type = ctx->data_declaration()->data_type()->accept(this);

	for (uint32_t i=0; i<ctx->data_declaration()->data_instantiation().size(); i++) {
		PSSParser::Data_instantiationContext *di =
				ctx->data_declaration()->data_instantiation(i);
		IExpr *array_dim = 0;

		if (di->array_dim()) {
			fprintf(stdout, "TODO: array_dim\n");
		}

		IField *field = m_model->mkField(
				di->identifier()->getText(),
				data_type,
				attr,
				array_dim);

		scope()->add(field);
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitAction_field_declaration(PSSParser::Action_field_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IField::FieldAttr attr = IField::FieldAttr_None;

	if (ctx->action_field_modifier()) {
		const std::string &modifier = ctx->action_field_modifier()->getText();
		if (modifier == "rand") {
			attr = IField::FieldAttr_Rand;
		} else if (modifier == "input") {
			attr = IField::FieldAttr_Input;
		} else if (modifier == "output") {
			attr = IField::FieldAttr_Output;
		} else if (modifier == "lock") {
			attr = IField::FieldAttr_Lock;
		} else if (modifier == "share") {
			attr = IField::FieldAttr_Share;
		} else if (modifier == "action") {
			// TODO:
		} else {

		}
	}

	IBaseItem *data_type = ctx->action_data_declaration()->action_data_type()->accept(this);

	for (uint32_t i=0; i<ctx->action_data_declaration()->data_instantiation().size(); i++) {
		PSSParser::Data_instantiationContext *di =
				ctx->action_data_declaration()->data_instantiation(i);
		IExpr *array_dim = 0;

		if (di->array_dim()) {
			fprintf(stdout, "TODO: array_dim\n");
		}

		IField *field = m_model->mkField(
				di->identifier()->getText(),
				data_type,
				attr,
				array_dim);

		scope()->add(field);
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitData_type(PSSParser::Data_typeContext *ctx) {
	IBaseItem *ret = 0;

	if (ctx->scalar_data_type()) {
		ret = ctx->scalar_data_type()->accept(this);
	} else if (ctx->user_defined_datatype()) {
		ret = ctx->user_defined_datatype()->accept(this);
	} else {
		fprintf(stdout, "TODO: user data-type\n");
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitScalar_data_type(PSSParser::Scalar_data_typeContext *ctx) {
	IBaseItem *ret = 0;

	if (ctx->bool_type()) {
		ret = m_model->mkScalarType(IScalarType::ScalarType_Bool, 0, 0);
	} else if (ctx->chandle_type()) {
		ret = m_model->mkScalarType(IScalarType::ScalarType_Chandle, 0, 0);
	} else if (ctx->integer_type()) {
		// TODO: get range
		IExpr *msb=0, *lsb=0;
		if (ctx->integer_type()->lhs) {
			if (ctx->integer_type()->rhs) {
				msb = ctx->integer_type()->expression(0)->accept(this);
				lsb = ctx->integer_type()->expression(1)->accept(this);
			} else {
				// WIDTH-1:0
				msb = m_model->mkBinExpr(
								ctx->integer_type()->expression(0)->accept(this),
								IBinaryExpr::BinOp_Minus,
								m_model->mkIntLiteral(1));
				lsb = m_model->mkIntLiteral(0);
			}
		} else {
			fprintf(stdout, "Error: support domain\n");
		}

		if (ctx->integer_type()->integer_atom_type()->getText() == "int") {
			if (!msb) {
				msb = m_model->mkIntLiteral(31);
				lsb = m_model->mkIntLiteral(0);
			}
			ret = m_model->mkScalarType(IScalarType::ScalarType_Int, msb, lsb);
		} else if (ctx->integer_type()->integer_atom_type()->getText() == "bit") {
			if (!msb) {
				msb = m_model->mkIntLiteral(0);
				lsb = m_model->mkIntLiteral(0);
			}
			ret = m_model->mkScalarType(IScalarType::ScalarType_Bit, msb, lsb);
		} else {
			fprintf(stdout, "Error: unknown scalar data-type %s\n",
					ctx->integer_type()->integer_atom_type()->getText().c_str());
		}
	} else if (ctx->string_type()) {
		ret = m_model->mkScalarType(IScalarType::ScalarType_String, 0, 0);
	} else {
		fprintf(stdout, "Error: unknown scalar type %s\n", ctx->getText().c_str());
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitComponent_field_declaration(PSSParser::Component_field_declarationContext *ctx) {
	fprintf(stdout, "field_declaration\n");
	if (ctx->component_data_declaration()) {
		return ctx->component_data_declaration()->accept(this);
	} else {
		return ctx->component_pool_declaration()->accept(this);
	}
}

antlrcpp::Any PSS2PSIVisitor::visitComponent_data_declaration(PSSParser::Component_data_declarationContext *ctx) {
	IBaseItem *ret = 0;
	fprintf(stdout, "data_declaration\n");

	IField::FieldAttr attr = IField::FieldAttr_None;

//	if (ctx->struct_field_modifier()) {
//		if (ctx->struct_field_modifier()->getText() == "rand") {
//			attr = IField::FieldAttr_Rand;
//		} else {
//			fprintf(stdout, "Unknown field modifier \"%s\"\n",
//					ctx->struct_field_modifier()->getText().c_str());
//		}
//	}

	IBaseItem *data_type = ctx->data_declaration()->data_type()->accept(this);

	for (uint32_t i=0; i<ctx->data_declaration()->data_instantiation().size(); i++) {
		PSSParser::Data_instantiationContext *di =
				ctx->data_declaration()->data_instantiation(i);
		IExpr *array_dim = 0;

		if (di->array_dim()) {
			fprintf(stdout, "TODO: array_dim\n");
		}

		IField *field = m_model->mkField(
				di->identifier()->getText(),
				data_type,
				attr,
				array_dim);

		scope()->add(field);
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitComponent_pool_declaration(PSSParser::Component_pool_declarationContext *ctx) {
	IBaseItem *ret = 0;
	fprintf(stdout, "pool_declaration\n");

	return ret;
}


antlrcpp::Any PSS2PSIVisitor::visitComponent_declaration(PSSParser::Component_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IComponent *comp = m_model->mkComponent(
			ctx->component_identifier()->identifier()->ID()->toString());

	ret = comp;

	push_scope(comp);
	for (uint32_t i=0; i<ctx->component_body_item().size(); i++) {
		IBaseItem *it = ctx->component_body_item(i)->accept(this);

		if (it) {
			comp->add(it);
		}
	}
	pop_scope();

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitConstraint_declaration(PSSParser::Constraint_declarationContext *ctx) {
	IBaseItem *ret = 0;
	std::vector<IConstraint *> constraints;
	std::string name;
	bool is_dynamic = false;

	if (ctx->single_stmt_constraint()) {
		constraints.push_back(ctx->single_stmt_constraint()->accept(this));
	} else {
		is_dynamic = (ctx->is_dynamic != 0);

		for (uint32_t i=0; i<ctx->constraint_body_item().size(); i++) {
			constraints.push_back(ctx->constraint_body_item(i)->accept(this));
		}
	}

	ret = m_model->mkConstraintBlock(name, constraints);
    scope()->add(ret);

    return ret;
}
antlrcpp::Any PSS2PSIVisitor::visitExpression_constraint_item(PSSParser::Expression_constraint_itemContext *ctx) {
	IConstraint *ret = 0;
	IExpr *expr = ctx->expression()->accept(this);

	if (ctx->implicand_constraint_item()) {
		ret = m_model->mkConstraintImplies(expr,
				ctx->implicand_constraint_item()->accept(this));
	} else {
		ret = m_model->mkConstraintExpr(expr);
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitIf_constraint_item(PSSParser::If_constraint_itemContext *ctx) {
	IConstraint *ret = 0;

	IExpr *cond = ctx->expression()->accept(this);
	IConstraint *true_cs = ctx->constraint_set(0)->accept(this);
	IConstraint *false_cs = 0;

	if (ctx->constraint_set().size() > 1) {
		false_cs = ctx->constraint_set(1)->accept(this);
	}

	ret = m_model->mkConstraintIf(cond, true_cs, false_cs);

    return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitConstraint_set(PSSParser::Constraint_setContext *ctx) {
	IConstraint *ret = 0;

	if (ctx->constraint_body_item()) {
		ret = ctx->constraint_body_item()->accept(this);
	} else {
		ret = ctx->constraint_block()->accept(this);
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitConstraint_block(PSSParser::Constraint_blockContext *ctx) {
	IConstraint *ret = 0;
	std::vector<IConstraint *> constraints;

	for (uint32_t i=0; i<ctx->constraint_body_item().size(); i++) {
		constraints.push_back(ctx->constraint_body_item(i)->accept(this));
	}

	ret = m_model->mkConstraintBlock("", constraints);

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitPackage_declaration(PSSParser::Package_declarationContext *ctx) {
	IBaseItem *ret;
	IPackage *pkg = m_model->findPackage(
			ctx->package_identifier()->type_identifier()->ID(0)->toString(), true);
	push_scope(pkg);
	ret = pkg;
	for (uint32_t i=0; i<ctx->package_body_item().size(); i++) {
		Any it_a = ctx->package_body_item(i)->accept(this);
		if (it_a.isNotNull()) {
			IBaseItem *it = it_a;
			pkg->add(it);
		}
	}
	pop_scope();

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExpression(PSSParser::ExpressionContext *ctx) {
	bool is_bin_op;
	IExpr *ret = 0;
	IExpr *expr = m_expr;
	if (ctx->unary_op()) {
		// TODO: unary op
		ret = ctx->lhs->accept(this);

		// TODO: m_expr = unary_op(op, m_expr)
	} else if (ctx->exp_op()) {
		// TODO:
	} else if (ctx->mul_div_mod_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->mul_div_mod_op()->getText() == "*") {
				op = IBinaryExpr::BinOp_Multiply;
			} else if (ctx->mul_div_mod_op()->getText() == "/") {
				op = IBinaryExpr::BinOp_Divide;
			} else if (ctx->mul_div_mod_op()->getText() == "%") {
				op = IBinaryExpr::BinOp_Mod;
			}
		IExpr *lhs = ctx->lhs->accept(this);
		IExpr *rhs = ctx->rhs->accept(this);

		ret = m_model->mkBinExpr(lhs, op, rhs);
	} else if (ctx->add_sub_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->add_sub_op()->getText() == "+") {
				op = IBinaryExpr::BinOp_Plus;
			} else if (ctx->add_sub_op()->getText() == "-") {
				op = IBinaryExpr::BinOp_Minus;
			}

		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->shift_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->shift_op()->getText() == "<<") {
//				op = IBinaryExpr::BinOp_ TODO
			} else if (ctx->shift_op()->getText() == ">>") {
//				op = IBinaryExpr::BinOp_ TODO
			}
		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->eq_neq_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
		if (ctx->eq_neq_op()->getText() == "==") {
			op = IBinaryExpr::BinOp_EqEq;
		} else if (ctx->eq_neq_op()->getText() == "!=") {
			op = IBinaryExpr::BinOp_NotEq;
		}
		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->logical_inequality_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
		if (ctx->logical_inequality_op()->getText() == "<") {
			op = IBinaryExpr::BinOp_LT;
		} else if (ctx->logical_inequality_op()->getText() == "<=") {
			op = IBinaryExpr::BinOp_LE;
		} else if (ctx->logical_inequality_op()->getText() == ">") {
			op = IBinaryExpr::BinOp_GT;
		} else if (ctx->logical_inequality_op()->getText() == ">=") {
			op = IBinaryExpr::BinOp_GE;
		}
		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->binary_and_op()) {
		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_And,
				ctx->rhs->accept(this));
	} else if (ctx->binary_xor_op()) {
		// TODO:
//		ret = m_model->mkBinExpr(
//				ctx->lhs->accept(this), IBinaryExpr::BinOp_Xor,
//				ctx->rhs->accept(this));
	} else if (ctx->binary_or_op()) {
		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_Or,
				ctx->rhs->accept(this));
	} else if (ctx->logical_and_op()) {
		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_AndAnd,
				ctx->rhs->accept(this));
	} else if (ctx->logical_or_op()) {
		ret = m_model->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_OrOr,
				ctx->rhs->accept(this));
	} else if (ctx->primary()) {
		ret = ctx->primary()->accept(this);
	} else {
		fprintf(stdout, "--> unknown %s\n", ctx->getText().c_str());
		/* ret = */ visitChildren(ctx);
		fprintf(stdout, "<-- unknown\n");
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitPrimary(PSSParser::PrimaryContext *ctx) {
	IExpr *ret = 0;

	if (ctx->number()) {
		uint64_t v = 0;
		if (ctx->number()->dec_number()) {
			v = strtoul(ctx->number()->getText().c_str(), 0, 0);
		} else if (ctx->number()->hex_number()) {
			const char *t = ctx->number()->getText().c_str();
			v = strtoul(&t[2], 0, 16);
		} else if (ctx->number()->oct_number()) {
			v = strtoul(ctx->number()->getText().c_str(), 0, 8);
		} else {
			fprintf(stdout, "Error: unsupported number format\n");
		}
		ret = m_model->mkBitLiteral(v); // TODO:
	} else if (ctx->bool_literal()) {
		ret = m_model->mkBoolLiteral(
				(ctx->bool_literal()->getText() == "true"));
	} else if (ctx->paren_expr()) {
		// TODO: should create a dedicated paren expression
		ret = ctx->paren_expr()->expression()->accept(this);
	} else if (ctx->string_literal()) {
		ret = m_model->mkStringLiteral(
				ctx->string_literal()->getText());
	} else if (ctx->variable_ref_path()) {
		ret = elaborate_field_ref(ctx->variable_ref_path()->variable_ref());
	} else if (ctx->method_function_call()) {
		ctx->method_function_call()->function_call()->method_parameter_list()->expression().size();
		const std::vector<PSSParser::ExpressionContext *> &param_ctx =
				ctx->method_function_call()->function_call()->method_parameter_list()->expression();
		std::vector<IExpr *> parameters;

		for (uint32_t i=0; i<param_ctx.size(); i++) {
			parameters.push_back(param_ctx.at(i)->accept(this));
		}
		IImportFunc *func = find_import_func(ctx->method_function_call()->function_call()); // TODO: find
		ret = m_model->mkMethodCallExpr(func, parameters);
	} else {
		fprintf(stdout, "Error: unknown primary %s\n",
				ctx->getText().c_str());
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitUser_defined_datatype(PSSParser::User_defined_datatypeContext *ctx) {
	IBaseItem *ret = 0;

	if (ctx->type_identifier()->ID().size() > 1) {
		IScopeItem *scope = 0;

		fprintf(stdout, "Note: hierarchical type specified\n");
		for (uint32_t i=0; i<ctx->type_identifier()->ID().size(); i++) {
			const std::string name = ctx->type_identifier()->ID().at(i)->getText();
			if (i == 0) {
				// Find the root first
				fprintf(stdout, "  Searching for root type %s\n", name.c_str());
				std::pair<IBaseItem *, IScopeItem *> root = find_type(name);

				if (root.first) {
					fprintf(stdout, "    Found\n");
					scope = dynamic_cast<IScopeItem *>(root.first);
				} else {
					fprintf(stdout, "    Did not find\n");
					break;
				}
			} else {
				fprintf(stdout, "  Searching for non-root type %s\n", name.c_str());
				IBaseItem *item = find_type(scope, name);

				if (item) {
					fprintf(stdout, "    Found\n");

					if (i+1 < ctx->type_identifier()->ID().size()) {
						scope = dynamic_cast<IScopeItem *>(item);
					} else {
						ret = item;
					}
				} else {
					fprintf(stdout, "    Did not find\n");
				}
			}
		}
	} else {
		for (int32_t i=m_scopes.size()-1; i>=0; i--) {
			IScopeItem *scope = m_scopes.at(i);

			for (uint32_t j=0; j<scope->getItems().size(); j++) {
				IBaseItem::ItemType t = scope->getItems().at(j)->getType();

				if (t == IBaseItem::TypeStruct ||
						t == IBaseItem::TypeAction ||
						t == IBaseItem::TypeComponent) {
					INamedItem *ni = dynamic_cast<INamedItem *>(scope->getItems().at(j));
					if (ni->getName() == ctx->type_identifier()->ID(0)->getText()) {
						ret = scope->getItems().at(j);
						break;
					}
				}
			}

			if (ret) {
				break;
			}
		}
	}

	if (!ret) {
		// Compose an error message
		std::string msg = "Failed to find type ";

		for (uint32_t i=0; i<ctx->type_identifier()->ID().size(); i++) {
			msg += ctx->type_identifier()->ID(i)->getText();
			if (i+1 < ctx->type_identifier()->ID().size()) {
				msg.append("::");
			}
		}

		fatal(msg);
	}

	return ret;
}


IFieldRef *PSS2PSIVisitor::elaborate_field_ref(
			const std::vector<PSSParser::Variable_refContext *> &path) {
	std::vector<IField *> fields;

    // This is the active scope
    IScopeItem *active_scope = dynamic_cast<IScopeItem *>(scope());

    // First, find the root
        // Traverse through the reference path built up above
        for (uint32_t i=0; i<path.size(); i++) {
        	const std::string &name = path.at(i)->getText();
            IBaseItem *t_it = 0;

            IScopeItem *search_s = active_scope;
            while (search_s) {
//                debug_high("Searching for %s in scope %s\n",
//                        name.c_str(), getName(search_s));
                for (std::vector<IBaseItem *>::const_iterator s_it=search_s->getItems().begin();
                        s_it!=search_s->getItems().end(); s_it++) {
                    if ((*s_it)->getType() == IBaseItem::TypeField &&
                            dynamic_cast<IField *>(*s_it)->getName() == name) {
                        t_it = *s_it;
                        break;
                    }
                }
                if (t_it) {
                    break;
                }

                // Traverse the inheritance hierarchy
                search_s = getSuperType(search_s);
            }

            if (t_it) {
                if (t_it->getType() == IBaseItem::TypeField) {
                    IField *field = dynamic_cast<IField *>(t_it);

                    fields.push_back(field);

                    if (i < path.size()) {
                        if (field->getDataType()) {
                            active_scope = dynamic_cast<IScopeItem *>(field->getDataType());

                            if (!active_scope) {
                                break;
                            }
                        } else {
                            fprintf(stdout, "Error: field %s doesn't have a type\n", field->getName().c_str());
                            break;
                        }
                    }
                } else {
                	fprintf(stdout, "Error: Field \"%s\" is not a field\n", name.c_str());
                }
            } else {
            	std::string error = "Failed to find field ";
            	for (uint32_t j=0; j<path.size(); j++) {
            		error += path.at(j)->getText();
            		if (j+1<path.size()) {
            			error += ".";
            		}
            	}
//            	error += " at " + path.at(0)->getStart()->getLine();
            	fatal(path.at(0)->getStart(), error);
                	fprintf(stdout, "Error: failed to find field %s\n", name.c_str());
            }
    }

	return m_model->mkFieldRef(fields);
}

IImportFunc *PSS2PSIVisitor::find_import_func(
		PSSParser::Function_callContext *func
		) {
	IImportFunc *ret = 0;
	PSSParser::Type_identifierContext *func_id = func->function_id()->type_identifier();

	if (func_id->ID().size() == 1) {
        const std::string &name = func_id->ID(0)->getText();
        for (int32_t i=m_scopes.size()-1; i>=0; i--) {
        	IScopeItem *search_s = dynamic_cast<IScopeItem *>(m_scopes.at(i));

        	if (!search_s) {
        		continue;
        	}

            for (std::vector<IBaseItem *>::const_iterator s_it=search_s->getItems().begin();
                    s_it!=search_s->getItems().end(); s_it++) {
                if ((*s_it)->getType() == IBaseItem::TypeImportFunc) {
                	if (dynamic_cast<IImportFunc *>(*s_it)->getName() == name) {
                		ret = dynamic_cast<IImportFunc *>(*s_it);
                		break;
                	}
                }
            }

            if (ret) {
                break;
            }

            // Traverse the inheritance hierarchy
            IScopeItem *super_scope_s = getSuperType(search_s);

            if (super_scope_s) {
            	fprintf(stdout, "TODO: search super-scope\n");
            }
        }
	} else {
		//
		fprintf(stdout, "TODO: full-context lookup for function ID\n");
	}

	return ret;
}

IBaseItem *PSS2PSIVisitor::find_type(PSSParser::Type_identifierContext *type) {
	IBaseItem *ret = 0;

	if (type->ID().size() > 1) {
		IScopeItem *s = m_model;

		// Assume we have a full path
		for (uint32_t i=0; i<type->ID().size(); i++) {
			//
			const std::string &name = type->ID(i)->getText();
			IBaseItem *it = 0;
			for (uint32_t j=0; j<s->getItems().size(); j++) {
				if (dynamic_cast<INamedItem *>(s->getItems().at(j)) &&
						dynamic_cast<INamedItem *>(s->getItems().at(j))->getName() == name) {
					it = s->getItems().at(j);
					break;
				}
			}
			if (!it) {
				fprintf(stdout, "Error: Failed to file item %s\n", name.c_str());
				return 0;
			} else {
				if (dynamic_cast<IScopeItem *>(it)) {
					s = dynamic_cast<IScopeItem *>(it);
				} else {
					fprintf(stdout, "Error: %s is not a scope\n", name.c_str());
					return 0;
				}
			}

			if (i+1 == type->ID().size()) {
				fprintf(stdout, "Final result: %p\n", it);
				ret = it;
			}
		}
	} else {
		fprintf(stdout, "TODO: must provide relative-path lookup\n");
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExec_block_stmt(PSSParser::Exec_block_stmtContext *ctx) {
	IBaseItem *ret = 0;
	IExec *exec = 0;

	std::string kind_s;

	if (ctx->exec_block()) {
		kind_s = ctx->exec_block()->exec_kind_identifier()->getText();
	} else if (ctx->target_code_exec_block()) {
		kind_s = ctx->target_code_exec_block()->exec_kind_identifier()->getText();
	}

	IExec::ExecKind kind;

	if (kind_s == "body") {
		kind = IExec::Body;
	} else if (kind_s == "pre_solve") {
		kind = IExec::PreSolve;
	} else if (kind_s == "post_solve") {
		kind = IExec::PostSolve;
	} else {
		fprintf(stdout, "Error: unknown exec kind \"%s\"\n", kind_s.c_str());
	}

	if (ctx->exec_block()) {
		std::vector<IExecStmt *> stmts;

		for (uint32_t i=0; i<ctx->exec_block()->exec_body_stmt().size(); i++) {
			stmts.push_back(ctx->exec_block()->exec_body_stmt(i)->accept(this));
		}

		exec = m_model->mkNativeExec(kind, stmts);
	} else if (ctx->target_code_exec_block()) {
		exec = m_model->mkTargetTemplateExec(kind,
				ctx->target_code_exec_block()->language_identifier()->getText(),
				ctx->target_code_exec_block()->string()->getText());
	} else if (ctx->target_file_exec_block()) {
	} else {
		fprintf(stdout, "Error: unknown exec-block type\n");
	}

	ret = exec;
    return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExec_body_stmt(PSSParser::Exec_body_stmtContext *ctx) {
	IExecStmt *ret;

	IExpr *expr = ctx->expression(0)->accept(this);

	if (ctx->assign_op()) {
		const std::string &op_s = ctx->assign_op()->getText();
		IExecExprStmt::AssignOp op;

		if (op_s == "=") {
			op = IExecExprStmt::AssignOp_Eq;
		} else if (op_s == "+=") {
			op = IExecExprStmt::AssignOp_PlusEq;
		} else if (op_s == "-=") {
			op = IExecExprStmt::AssignOp_MinusEq;
		} else if (op_s == "<<=") {
			op = IExecExprStmt::AssignOp_LShiftEq;
		} else if (op_s == ">>=") {
			op = IExecExprStmt::AssignOp_RShiftEq;
		} else if (op_s == "|=") {
			op = IExecExprStmt::AssignOp_OrEq;
		} else if (op_s == "&=") {
			op = IExecExprStmt::AssignOp_AndEq;
		} else {
			fprintf(stdout, "Unknown assignment op %s\n", op_s.c_str());
		}

		ret = m_model->mkExecExprStmt((IFieldRef *)expr, op,
				ctx->expression(1)->accept(this));
	} else {
		// Just an expression statement
		// TODO: must support expression on LHS
		ret = m_model->mkExecExprStmt((IFieldRef *)expr, IExecExprStmt::AssignOp_None, 0);
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExtend_stmt(PSSParser::Extend_stmtContext *ctx) {
	IBaseItem *ret = 0;
	std::string type = ctx->ext_type->getText();

	IBaseItem *target = find_type(ctx->type_identifier());

	IExtend *ext = m_model->mkExtend(target);

	if (type == "action") {
		for (uint32_t i=0; i<ctx->action_body_item().size(); i++) {
			ext->add(ctx->action_body_item(i)->accept(this));
		}
	} else if (type == "struct") {
		for (uint32_t i=0; i<ctx->struct_body_item().size(); i++) {
			ext->add(ctx->struct_body_item(i)->accept(this));
		}
	} else if (type == "enum") {
		fprintf(stdout, "TODO: enum type extension\n");
	} else if (type == "component") {
		for (uint32_t i=0; i<ctx->component_body_item().size(); i++) {
			ext->add(ctx->component_body_item(i)->accept(this));
		}
	} else {
		fprintf(stdout, "Error: unknown extension type %s\n", type.c_str());
	}

	ret = ext;
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitMethod_prototype(PSSParser::Method_prototypeContext *ctx) {
	IBaseItem *ret;

	const std::string &name = ctx->method_identifier()->getText();

	IBaseItem *ret_type = 0;
	if (ctx->method_return_type()->data_type()) {
		ret_type = ctx->method_return_type()->data_type()->accept(this);
	}

	std::vector<IField *> parameters;
	for (uint32_t i=0; i<ctx->method_parameter_list_prototype()->method_parameter().size(); i++) {
		IField::FieldAttr attr = IField::FieldAttr_Input;
		PSSParser::Method_parameterContext *p =
				ctx->method_parameter_list_prototype()->method_parameter(i);
		if (p->method_parameter_dir()) {
			const std::string &dir = p->method_parameter_dir()->getText();
			if (dir == "input") {
				attr = IField::FieldAttr_Input;
			} else if (dir == "output") {
				attr = IField::FieldAttr_Output;
			} else if (dir == "inout") {
				attr = IField::FieldAttr_Inout;
			} else {
				fprintf(stdout, "Error: unknown parameter direction %s\n", dir.c_str());
			}
		}

		IBaseItem *type = p->data_type()->accept(this);
		const std::string &id = p->identifier()->getText();

		parameters.push_back(m_model->mkField(id, type, attr, 0));
	}

	IImportFunc *method = m_model->mkImportFunc(
			name, ret_type, parameters);

	ret = method;

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitFunction_decl(PSSParser::Function_declContext *ctx) {
	return ctx->method_prototype()->accept(this);
}

IExpr *PSS2PSIVisitor::expression(PSSParser::ExpressionContext *ctx) {
	ctx->accept(this);
	return 0;
}

void PSS2PSIVisitor::push_scope(IScopeItem *scope) {
	m_scopes.push_back(scope);
}

IScopeItem *PSS2PSIVisitor::scope() const {
	return m_scopes.at(m_scopes.size()-1);
}

IScopeItem *PSS2PSIVisitor::pop_scope() {
	IScopeItem *s = scope();
	m_scopes.pop_back();
	return s;
}

IScopeItem *PSS2PSIVisitor::getSuperType(IScopeItem *it) {
	if (dynamic_cast<IAction *>(it)) {
		return dynamic_cast<IAction *>(it)->getSuperType();
	} else if (dynamic_cast<IStruct *>(it)) {
		return dynamic_cast<IStruct *>(it)->getSuperType();
	} else if (dynamic_cast<IComponent *>(it)) {
// TODO:		return dynamic_cast<IComponent *>(it)->getSuperType();
	}
	return 0;
}

std::pair<IBaseItem *, IScopeItem *> PSS2PSIVisitor::find_type(const std::string &name) {
	std::pair<IBaseItem *, IScopeItem *> ret;
	ret.first = 0;
	ret.second = 0;

	// Search up the scopes looking for the named type
	for (int32_t i=m_scopes.size()-1; i>=0; i--) {
		IScopeItem *scope = m_scopes.at(i);
		IBaseItem *item = find_type(scope, name);

		if (item) {
			ret.first = item;
			ret.second = scope;
			break;
		}
	}

	return ret;
}

IBaseItem *PSS2PSIVisitor::find_type(IScopeItem *scope, const std::string &name) {
	for (uint32_t j=0; j<scope->getItems().size(); j++) {
		IBaseItem::ItemType t = scope->getItems().at(j)->getType();

		if (t == IBaseItem::TypeStruct ||
				t == IBaseItem::TypeAction ||
				t == IBaseItem::TypeComponent ||
				t == IBaseItem::TypePackage) {
			INamedItem *ni = dynamic_cast<INamedItem *>(scope->getItems().at(j));
			if (ni && ni->getName() == name) {
				return scope->getItems().at(j);
			}
		}
	}
	return 0;
}

void PSS2PSIVisitor::fatal(const std::string &msg) {
	throw msg;
}

void PSS2PSIVisitor::fatal(
		antlr4::Token 		*loc,
		const std::string	&msg) {
	std::string error_msg = "Fatal: " + msg;
	char tmp[128];

	error_msg += " " + m_file;
	sprintf(tmp, ":%d", loc->getLine());
	error_msg += tmp;

	throw error_msg;
}
