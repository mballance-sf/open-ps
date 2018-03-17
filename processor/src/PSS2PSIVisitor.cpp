/*
 * PSS2PSIVisitor.cpp
 *
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#include "PSS2PSIVisitor.h"

#include <cstdio>
#include <exception>
#include <typeinfo>
#include <stdarg.h>

using namespace antlrcpp;


PSS2PSIVisitor::PSS2PSIVisitor(IModel *model, const std::string &path) :
		m_model(model), m_factory(model->getItemFactory()), m_file(path) {
	m_debug = false;
}

PSS2PSIVisitor::~PSS2PSIVisitor() {
	// TODO Auto-generated destructor stub
}

antlrcpp::Any PSS2PSIVisitor::visitModel(PSSParser::ModelContext *ctx) {

	enter("visitModel");
	push_scope(m_model);
	for (uint32_t i=0; i<ctx->portable_stimulus_description().size(); i++) {
		try {
		IBaseItem *it = ctx->portable_stimulus_description(i)->accept(this);
		if (it && it->getType() != IBaseItem::TypePackage) {
			m_model->add(it);
		}
		} catch (std::bad_cast &e) {
			error("Bad Cast: %s", e.what());
		}
	}
	pop_scope();

	leave("visitModel");

	return (IBaseItem *)0;
}

antlrcpp::Any PSS2PSIVisitor::visitAction_declaration(PSSParser::Action_declarationContext *ctx) {
	IBaseItem *super_type = 0;
	IBaseItem *ret = 0;

	enter("visitAction_declaration");

	if (ctx->action_super_spec()) {
		super_type = m_factory->mkRefType(scope(),
				type2vector(ctx->action_super_spec()->type_identifier()));
	}

	IAction *action = m_factory->mkAction(
			ctx->action_identifier()->identifier()->ID()->toString(),
			super_type);
	ret = action;

	push_scope(action);

	for (uint32_t i=0; i<ctx->action_body_item().size(); i++) {
		IBaseItem *it = ctx->action_body_item(i)->accept(this);

		if (it) {
			action->add(it);
		} else {
			error("null action item");
		}
	}
	pop_scope();

	leave("visitAction_declaration");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitAbstract_action_declaration(PSSParser::Abstract_action_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitAbstract_action_declaration");

	leave("visitAbstract_action_declaration");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitAction_data_type(PSSParser::Action_data_typeContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitAction_data_type");

	if (ctx->scalar_data_type()) {
		ret = ctx->scalar_data_type()->accept(this);
	} else if (ctx->user_defined_datatype()) {
		ret = ctx->user_defined_datatype()->accept(this);
	} else {
		error("Unknown action data type");
	}

	leave("visitAction_data_type");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_declaration(PSSParser::Activity_declarationContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitActivity_declaration");

	IAction *action = dynamic_cast<IAction *>(scope());
	IGraphBlockStmt *block = m_factory->mkGraphBlockStmt(IGraphBlockStmt::GraphStmt_Block);
	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		IGraphStmt *stmt = ctx->activity_stmt(i)->accept(this);
		if (stmt) {
			block->add(stmt);
		} else {
			error("null activity stmt");
		}
	}
	action->setGraph(block);

	leave("visitActivity_declaration");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_bind_stmt(PSSParser::Activity_bind_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	enter("visitActivity_bind_stmt");
	todo("visitActivity_bind_stmt\n");
	leave("visitActivity_bind_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_action_traversal_stmt(PSSParser::Activity_action_traversal_stmtContext *ctx) {
	IGraphStmt *ret = 0;
	IConstraint *with_c = 0;

	enter("visitActivity_action_traveral_stmt");

	if (ctx->inline_with_constraint()) {
		todo("inline constraint");
	}

	if (ctx->is_do) {
		// Do <type>
		todo("do <action::type> unimplemented");
	} else {
		// <type>
		std::vector<PSSParser::Variable_refContext *> path;
		path.push_back(ctx->variable_ref());
//		ret = m_factory->mkGraphTraverseStmt(elaborate_field_ref(path), with_c);
		todo("mkGraphTraverseStmt");
	}

	leave("visitActivity_action_traveral_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_parallel_stmt(PSSParser::Activity_parallel_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	enter("visitActivity_parallel_stmt");

	IGraphBlockStmt *parallel = m_factory->mkGraphBlockStmt(IGraphStmt::GraphStmt_Parallel);

	for (uint32_t i=0; i<ctx->activity_labeled_stmt().size(); i++) {
		IGraphStmt *stmt = ctx->activity_labeled_stmt(i)->activity_stmt()->accept(this);
		if (dynamic_cast<IGraphBlockStmt *>(stmt) &&
				dynamic_cast<IGraphBlockStmt *>(stmt)->getStmtType() == IGraphBlockStmt::GraphStmt_Block) {
			parallel->add(stmt);
		} else {
			IGraphBlockStmt *stmt_b = m_factory->mkGraphBlockStmt(IGraphStmt::GraphStmt_Block);
			if (stmt) {
				stmt_b->add(stmt);
			}
			parallel->add(stmt_b);
		}
	}
	ret = parallel;

	leave("visitActivity_parallel_stmt");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_repeat_stmt(PSSParser::Activity_repeat_stmtContext *ctx) {
	IGraphStmt *ret = 0;
	IExpr *expr = 0;

	enter("visitActivity_repeat_stmt");

	IGraphRepeatStmt::RepeatType type = IGraphRepeatStmt::RepeatType_Forever;
	IGraphBlockStmt *body = m_factory->mkGraphBlockStmt();

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
		IGraphStmt *stmt = ctx->activity_sequence_block_stmt()->activity_stmt(i)->accept(this);
		if (stmt) {
			body->add(stmt);
		} else {
			error("null repeat-body statement");
		}
	}

	ret = m_factory->mkGraphRepeatStmt(type, expr, body);

	leave("visitActivity_repeat_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_constraint_stmt(PSSParser::Activity_constraint_stmtContext *ctx) {
	IGraphStmt *ret = 0;
	enter("visitActivity_constraint_stmt");

	todo("visitActivity_constraint_stmt\n");

	leave("visitActivity_constraint_stmt");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_if_else_stmt(PSSParser::Activity_if_else_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	enter("visitActivity_if_else_stmt");
	todo("visitActivity_if_else_stmt");
	leave("visitActivity_if_else_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_select_stmt(PSSParser::Activity_select_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	enter("visitActivity_select_stmt");

	IGraphBlockStmt *select = m_factory->mkGraphBlockStmt(IGraphStmt::GraphStmt_Select);

	for (uint32_t i=0; i<ctx->activity_labeled_stmt().size(); i++) {
		IGraphStmt *stmt = ctx->activity_labeled_stmt(i)->activity_stmt()->accept(this);
		if (dynamic_cast<IGraphBlockStmt *>(stmt) &&
				dynamic_cast<IGraphBlockStmt *>(stmt)->getStmtType() == IGraphBlockStmt::GraphStmt_Block) {
			select->add(stmt);
		} else {
			IGraphBlockStmt *stmt_b = m_factory->mkGraphBlockStmt(IGraphStmt::GraphStmt_Block);
			if (stmt) {
				stmt_b->add(stmt);
			} else {
				error("null select statement");
			}
			select->add(stmt_b);
		}
	}

	ret = select;

	leave("visitActivity_select_stmt");
    return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_schedule_stmt(PSSParser::Activity_schedule_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	enter("visitActivity_schedule_stmt");
	todo("visitActivity_schedule_stmt");
	leave("visitActivity_schedule_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_foreach_stmt(PSSParser::Activity_foreach_stmtContext *ctx) {
	IGraphStmt *ret = 0;
	enter("visitActivity_schedule_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitActivity_sequence_block_stmt(PSSParser::Activity_sequence_block_stmtContext *ctx) {
	IGraphStmt *ret = 0;

	enter("visitActivity_sequence_block_stmt");

	IGraphBlockStmt *seq = m_factory->mkGraphBlockStmt(IGraphStmt::GraphStmt_Block);
	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		IGraphStmt *stmt = ctx->activity_stmt(i)->accept(this);
		if (stmt) {
			seq->add(stmt);
		}
	}
	ret = seq;

	leave("visitActivity_sequence_block_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitSymbol_declaration(PSSParser::Symbol_declarationContext *ctx) {
	enter("visitSymbol_declaration");
	todo("support symbol declaration");
	leave("visitSymbol_declaration");
	return (IBaseItem *)0;
}

antlrcpp::Any PSS2PSIVisitor::visitOverrides_declaration(PSSParser::Overrides_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitOverrides_declaration");
	todo("visitOverrides_declaration");
	leave("visitOverrides_declaration");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitStruct_declaration(PSSParser::Struct_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IBaseItem *super_type = 0;

	enter("visitStruct_declaration");

	if (ctx->struct_super_spec()) {
		super_type = m_factory->mkRefType(scope(),
				type2vector(ctx->struct_super_spec()->type_identifier()));
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
		error("Unknown struct type \"%s\"\n",
				ctx->struct_type()->img->getText().c_str());
	}

	IStruct *s = m_factory->mkStruct(
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

	leave("visitStruct_declaration");

    return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitStruct_field_declaration(PSSParser::Struct_field_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IField::FieldAttr attr = IField::FieldAttr_None;

	enter("visitStruct_field_declaration");

	if (ctx->struct_field_modifier()) {
		if (ctx->struct_field_modifier()->getText() == "rand") {
			attr = IField::FieldAttr_Rand;
		} else {
			error("Unknown field modifier \"%s\"",
					ctx->struct_field_modifier()->getText().c_str());
		}
	}

	IBaseItem *data_type = ctx->data_declaration()->data_type()->accept(this);

	for (uint32_t i=0; i<ctx->data_declaration()->data_instantiation().size(); i++) {
		PSSParser::Data_instantiationContext *di =
				ctx->data_declaration()->data_instantiation(i);
		IExpr *array_dim = 0;

		if (di->array_dim()) {
			todo("TODO: array_dim");
		}

		IField *field = m_factory->mkField(
				di->identifier()->getText(),
				data_type,
				attr,
				array_dim);

		scope()->add(field);
	}

	leave("visitStruct_field_declaration");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitAction_field_declaration(PSSParser::Action_field_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IField::FieldAttr attr = IField::FieldAttr_None;

	enter("visitAction_field_declaration");

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
			todo("array_dim");
		}

		IField *field = m_factory->mkField(
				di->identifier()->getText(),
				data_type,
				attr,
				array_dim);

		scope()->add(field);
	}

	leave("visitAction_field_declaration");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitData_type(PSSParser::Data_typeContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitData_type");

	if (ctx->scalar_data_type()) {
		ret = ctx->scalar_data_type()->accept(this);
	} else if (ctx->user_defined_datatype()) {
		ret = ctx->user_defined_datatype()->accept(this);
	} else {
		todo("TODO: user data-type");
	}

	leave("visitData_type");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitScalar_data_type(PSSParser::Scalar_data_typeContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitScalar_data_type");

	if (ctx->bool_type()) {
		ret = m_factory->mkScalarType(IScalarType::ScalarType_Bool, 0, 0);
	} else if (ctx->chandle_type()) {
		ret = m_factory->mkScalarType(IScalarType::ScalarType_Chandle, 0, 0);
	} else if (ctx->integer_type()) {
		// TODO: get range
		IExpr *msb=0, *lsb=0;
		if (ctx->integer_type()->lhs) {
			if (ctx->integer_type()->rhs) {
				msb = ctx->integer_type()->expression(0)->accept(this);
				lsb = ctx->integer_type()->expression(1)->accept(this);
			} else {
				// WIDTH-1:0
				msb = m_factory->mkBinExpr(
								ctx->integer_type()->expression(0)->accept(this),
								IBinaryExpr::BinOp_Minus,
								m_factory->mkIntLiteral(1));
				lsb = m_factory->mkIntLiteral(0);
			}
		} else {
			todo("support domain");
		}

		if (ctx->integer_type()->integer_atom_type()->getText() == "int") {
			if (!msb) {
				msb = m_factory->mkIntLiteral(31);
				lsb = m_factory->mkIntLiteral(0);
			}
			ret = m_factory->mkScalarType(IScalarType::ScalarType_Int, msb, lsb);
		} else if (ctx->integer_type()->integer_atom_type()->getText() == "bit") {
			if (!msb) {
				msb = m_factory->mkIntLiteral(0);
				lsb = m_factory->mkIntLiteral(0);
			}
			ret = m_factory->mkScalarType(IScalarType::ScalarType_Bit, msb, lsb);
		} else {
			error("unknown scalar data-type %s",
					ctx->integer_type()->integer_atom_type()->getText().c_str());
		}
	} else if (ctx->string_type()) {
		ret = m_factory->mkScalarType(IScalarType::ScalarType_String, 0, 0);
	} else {
		error("unknown scalar type %s", ctx->getText().c_str());
	}

	leave("visitScalar_data_type");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitEnum_declaration(PSSParser::Enum_declarationContext *ctx) {
	IBaseItem *ret = 0;
	std::vector<IEnumerator *> enumerators;
	enter("visitEnum_declaration");

	for (uint32_t i=0; i<ctx->enum_item().size(); i++) {
		IExpr *value = 0;

		if (ctx->enum_item(i)->constant_expression()) {
			value = ctx->enum_item(i)->constant_expression()->accept(this);
		}

		enumerators.push_back(
				m_factory->mkEnumerator(
						ctx->enum_item(i)->identifier()->getText(),
						value));
	}

	IEnumType *e = m_factory->mkEnumType(
			ctx->enum_identifier()->getText(),
			enumerators);

	todo("enum_declaration");

	leave("visitEnum_declaration");
	ret = e;
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitComponent_field_declaration(PSSParser::Component_field_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitComponent_field_declaration");

	todo("field_declaration");
	if (ctx->component_data_declaration()) {
		ret = ctx->component_data_declaration()->accept(this);
	} else {
		ret = ctx->component_pool_declaration()->accept(this);
	}

	leave("visitComponent_field_declaration");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitComponent_data_declaration(PSSParser::Component_data_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitComponent_data_declaration");

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
			todo("array_dim");
		}

		IField *field = m_factory->mkField(
				di->identifier()->getText(),
				data_type,
				attr,
				array_dim);

		scope()->add(field);
	}

	leave("visitComponent_data_declaration");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitComponent_pool_declaration(PSSParser::Component_pool_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitComponent_pool_declaration");
	todo("pool_declaration");

	leave("visitComponent_pool_declaration");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitObject_bind_stmt(PSSParser::Object_bind_stmtContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitObject_bind_stmt");

	todo("object_bind_stmt");

	leave("visitObject_bind_stmt");
	return ret;
}


antlrcpp::Any PSS2PSIVisitor::visitComponent_declaration(PSSParser::Component_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IBaseItem *super_type = 0;

	enter("visitComponent_declaration");

	if (ctx->component_super_spec()) {
		super_type = m_factory->mkRefType(scope(),
				type2vector(ctx->component_super_spec()->type_identifier()));
	}

	IComponent *comp = m_factory->mkComponent(
			ctx->component_identifier()->identifier()->ID()->toString(),
			super_type);

	ret = comp;

	push_scope(comp);
	for (uint32_t i=0; i<ctx->component_body_item().size(); i++) {
		enter("component_body_item()");
		IBaseItem *it = ctx->component_body_item(i)->accept(this);
		leave("component_body_item()");

		if (it) {
			comp->add(it);
		}
	}
	pop_scope();

	leave("visitComponent_declaration");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitConstraint_declaration(PSSParser::Constraint_declarationContext *ctx) {
	IBaseItem *ret = 0;
	std::vector<IConstraint *> constraints;
	std::string name;
	bool is_dynamic = false;

	enter("visitConstraint_declaration");

	if (ctx->single_stmt_constraint()) {
		constraints.push_back(ctx->single_stmt_constraint()->accept(this));
	} else {
		is_dynamic = (ctx->is_dynamic != 0);

		for (uint32_t i=0; i<ctx->constraint_body_item().size(); i++) {
			IConstraint *c = ctx->constraint_body_item(i)->accept(this);

			if (c) {
				constraints.push_back(c);
			} else {
				error("null constraint");
			}
		}
	}

	ret = m_factory->mkConstraintBlock(name, constraints);
    scope()->add(ret);

	leave("visitConstraint_declaration");

    return ret;
}
antlrcpp::Any PSS2PSIVisitor::visitExpression_constraint_item(PSSParser::Expression_constraint_itemContext *ctx) {
	IConstraint *ret = 0;

	enter("visitExpression_constraint_item");

	IExpr *expr = ctx->expression()->accept(this);

	if (ctx->implicand_constraint_item()) {
		ret = m_factory->mkConstraintImplies(expr,
				ctx->implicand_constraint_item()->accept(this));
	} else {
		ret = m_factory->mkConstraintExpr(expr);
	}

	leave("visitExpression_constraint_item");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitIf_constraint_item(PSSParser::If_constraint_itemContext *ctx) {
	IConstraint *ret = 0;

	enter("visitIf_constraint_item");

	IExpr *cond = ctx->expression()->accept(this);
	IConstraint *true_cs = ctx->constraint_set(0)->accept(this);
	IConstraint *false_cs = 0;

	if (ctx->constraint_set().size() > 1) {
		false_cs = ctx->constraint_set(1)->accept(this);
	}

	ret = m_factory->mkConstraintIf(cond, true_cs, false_cs);

	leave("visitIf_constraint_item");

    return ret;
}
antlrcpp::Any PSS2PSIVisitor::visitForeach_constraint_item(PSSParser::Foreach_constraint_itemContext *ctx) {
	IConstraint *ret = 0;

	enter("visitForeach_constraint_item");
	todo("visitForeach_constraint_item");
	leave("visitForeach_constraint_item");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitConstraint_set(PSSParser::Constraint_setContext *ctx) {
	IConstraint *ret = 0;

	enter("visitConstraint_set");

	if (ctx->constraint_body_item()) {
		ret = ctx->constraint_body_item()->accept(this);
	} else {
		ret = ctx->constraint_block()->accept(this);
	}

	leave("visitConstraint_set");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitConstraint_block(PSSParser::Constraint_blockContext *ctx) {
	IConstraint *ret = 0;
	std::vector<IConstraint *> constraints;

	enter("visitConstraint_block");

	for (uint32_t i=0; i<ctx->constraint_body_item().size(); i++) {
		IConstraint *c = ctx->constraint_body_item(i)->accept(this);
		if (c) {
			constraints.push_back(c);
		} else {
			error("null constraint");
		}
	}

	ret = m_factory->mkConstraintBlock("", constraints);

	leave("visitConstraint_block");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitUnique_constraint_item(PSSParser::Unique_constraint_itemContext *ctx) {
	enter("visitUnique_constraint_item");
	todo("unique constraint");
	leave("visitUnique_constraint_item");
	return (IConstraint *)0;
}

antlrcpp::Any PSS2PSIVisitor::visitBins_declaration(PSSParser::Bins_declarationContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitBins_declaration");
	todo("visitBins_declaration");
	enter("visitBins_declaration");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitCoverspec_declaration(PSSParser::Coverspec_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitCoverspec_declaration");
	ICoverspec *cs = m_factory->mkCoverspec(ctx->identifier()->getText());
	todo("visitCoverspec_declaration");
	leave("visitCoverspec_declaration");

	ret = cs;

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitPackage_declaration(PSSParser::Package_declarationContext *ctx) {
	IBaseItem *ret;

	enter("visitPackage_declaration");

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

	leave("visitPackage_declaration");
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExpression(PSSParser::ExpressionContext *ctx) {
	bool is_bin_op;
	IExpr *ret = 0;
	IExpr *expr = m_expr;

	enter("visitExpression");

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

		ret = m_factory->mkBinExpr(lhs, op, rhs);
	} else if (ctx->add_sub_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->add_sub_op()->getText() == "+") {
				op = IBinaryExpr::BinOp_Plus;
			} else if (ctx->add_sub_op()->getText() == "-") {
				op = IBinaryExpr::BinOp_Minus;
			}

		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->shift_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->shift_op()->getText() == "<<") {
//				op = IBinaryExpr::BinOp_ TODO
			} else if (ctx->shift_op()->getText() == ">>") {
//				op = IBinaryExpr::BinOp_ TODO
			}
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->eq_neq_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
		if (ctx->eq_neq_op()->getText() == "==") {
			op = IBinaryExpr::BinOp_EqEq;
		} else if (ctx->eq_neq_op()->getText() == "!=") {
			op = IBinaryExpr::BinOp_NotEq;
		}
		ret = m_factory->mkBinExpr(
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
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->binary_and_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_And,
				ctx->rhs->accept(this));
	} else if (ctx->binary_xor_op()) {
		// TODO:
//		ret = m_factory->mkBinExpr(
//				ctx->lhs->accept(this), IBinaryExpr::BinOp_Xor,
//				ctx->rhs->accept(this));
	} else if (ctx->binary_or_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_Or,
				ctx->rhs->accept(this));
	} else if (ctx->logical_and_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_AndAnd,
				ctx->rhs->accept(this));
	} else if (ctx->logical_or_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_OrOr,
				ctx->rhs->accept(this));
	} else if (ctx->primary()) {
		ret = ctx->primary()->accept(this);
	} else {
		enter("unknown %s\n", ctx->getText().c_str());
		/* ret = */ visitChildren(ctx);
		leave("unknown");
	}

	leave("visitExpression");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitPrimary(PSSParser::PrimaryContext *ctx) {
	IExpr *ret = 0;

	enter("visitPrimary");

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
			error("unsupported number format");
		}
		ret = m_factory->mkBitLiteral(v); // TODO:
	} else if (ctx->bool_literal()) {
		ret = m_factory->mkBoolLiteral(
				(ctx->bool_literal()->getText() == "true"));
	} else if (ctx->paren_expr()) {
		// TODO: should create a dedicated paren expression
		ret = ctx->paren_expr()->expression()->accept(this);
	} else if (ctx->string_literal()) {
		ret = m_factory->mkStringLiteral(
				ctx->string_literal()->getText());
	} else if (ctx->variable_ref_path()) {
		// Deref <FieldRef> [optional_part_select] [optional_follow_on]
		todo("variable_ref_path");
//		ret = m_factory->mkRefExpr(scope(),
//				ctx->variable_ref_path()->variable_ref());
//				path)
//		ret = elaborate_field_ref(ctx->variable_ref_path()->variable_ref());
	} else if (ctx->method_function_call()) {
		ctx->method_function_call()->function_call()->method_parameter_list()->expression().size();
		const std::vector<PSSParser::ExpressionContext *> &param_ctx =
				ctx->method_function_call()->function_call()->method_parameter_list()->expression();
		std::vector<IExpr *> parameters;

		for (uint32_t i=0; i<param_ctx.size(); i++) {
			parameters.push_back(param_ctx.at(i)->accept(this));
		}
		IImportFunc *func = find_import_func(ctx->method_function_call()->function_call()); // TODO: find
		ret = m_factory->mkMethodCallExpr(func, parameters);
	} else {
		error("unknown primary %s", ctx->getText().c_str());
	}

	leave("visitPrimary");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitUser_defined_datatype(PSSParser::User_defined_datatypeContext *ctx) {
	enter("visitUser_defined_datatype");

	IBaseItem *ret = m_factory->mkRefType(scope(),
			type2vector(ctx->type_identifier()));

	leave("visitUser_defined_datatype");

	return ret;
}


IFieldRef *PSS2PSIVisitor::elaborate_field_ref(
			const std::vector<PSSParser::Variable_refContext *> &path) {
	std::vector<IField *> fields;

	enter("elaborate_field_ref");

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
                            error("field %s doesn't have a type",
                            		field->getName().c_str());
                            break;
                        }
                    }
                } else {
                	error("Field \"%s\" is not a field", name.c_str());
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
//                	error("failed to find field %s", name.c_str());
            }
    }

	leave("elaborate_field_ref");

	return m_factory->mkFieldRef(fields);
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
            	todo("search super-scope");
            }
        }
	} else {
		//
		todo("full-context lookup for function ID");
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
				error("Failed to file item %s", name.c_str());
				return 0;
			} else {
				if (dynamic_cast<IScopeItem *>(it)) {
					s = dynamic_cast<IScopeItem *>(it);
				} else {
					error("%s is not a scope", name.c_str());
					return 0;
				}
			}

			if (i+1 == type->ID().size()) {
				debug("Final result: %p\n", it);
				ret = it;
			}
		}
	} else {
		todo("must provide relative-path lookup");
	}

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExec_block(PSSParser::Exec_blockContext *ctx) {
	IBaseItem *ret = 0;
	IExec *exec = 0;
	std::string kind_s = ctx->exec_kind_identifier()->getText();

	IExec::ExecKind kind;

	if (kind_s == "body") {
		kind = IExec::Body;
	} else if (kind_s == "pre_solve") {
		kind = IExec::PreSolve;
	} else if (kind_s == "post_solve") {
		kind = IExec::PostSolve;
	} else {
		error("unknown exec kind \"%s\"", kind_s.c_str());
	}

	enter("visitExec_block");
	std::vector<IExecStmt *> stmts;

	for (uint32_t i=0; i<ctx->exec_body_stmt().size(); i++) {
		IExecStmt *stmt = 0;
		try {
			stmt = ctx->exec_body_stmt(i)->accept(this);
		} catch (std::bad_cast &e) {
			error("IExecStmt: bad-cast\n");
		}
		if (stmt) {
			stmts.push_back(stmt);
		} else {
			error("null exec stmt");
		}
	}

	exec = m_factory->mkNativeExec(kind, stmts);
	leave("visitExec_block");

	ret = exec;
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitTarget_code_exec_block(PSSParser::Target_code_exec_blockContext *ctx) {
	IExec *exec = 0;
	IBaseItem *ret = 0;

	enter("visitTarget_code_exec_block");

	std::string kind_s = ctx->exec_kind_identifier()->getText();

	IExec::ExecKind kind;

	if (kind_s == "body") {
		kind = IExec::Body;
	} else if (kind_s == "pre_solve") {
		kind = IExec::PreSolve;
	} else if (kind_s == "post_solve") {
		kind = IExec::PostSolve;
	} else {
		error("unknown exec kind \"%s\"", kind_s.c_str());
	}

	exec = m_factory->mkTargetTemplateExec(
			kind,
			ctx->language_identifier()->getText(),
			ctx->string()->getText());

	leave("visitTarget_code_exec_block");

	ret = exec;
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitTarget_file_exec_block(PSSParser::Target_file_exec_blockContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitTarget_file_exec_block");
	todo("visitTarget_file_exec_block");
	leave("visitTarget_file_exec_block");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExec_body_stmt(PSSParser::Exec_body_stmtContext *ctx) {
	IExecStmt *ret = 0;

	enter("visitExec_body_stmt\n");

	enter("visit_expression");
	IExpr *expr = ctx->expression(0)->accept(this);
	leave("visit_expression");

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
			error("Unknown assignment op %s", op_s.c_str());
		}

		enter("visit_expression (2)");
		ret = m_factory->mkExecExprStmt((IFieldRef *)expr, op,
				ctx->expression(1)->accept(this));
		leave("visit_expression (2)");
	} else {
		// Just an expression statement
		// TODO: must support expression on LHS
		ret = m_factory->mkExecExprStmt((IFieldRef *)expr, IExecExprStmt::AssignOp_None, 0);
	}

	leave("visitExec_body_stmt\n");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExtend_stmt(PSSParser::Extend_stmtContext *ctx) {
	IBaseItem *ret = 0;
	std::string type = ctx->ext_type->getText();

	enter("visitExtend_stmt");

	IRefType *target = m_factory->mkRefType(scope(),
			type2vector(ctx->type_identifier()));

	IExtend *ext = 0;

	if (type == "action") {
		ext = m_factory->mkExtend(IExtend::ExtendType_Action, target);
		for (uint32_t i=0; i<ctx->action_body_item().size(); i++) {
			ext->add(ctx->action_body_item(i)->accept(this));
		}
	} else if (type == "struct") {
		ext = m_factory->mkExtend(IExtend::ExtendType_Struct, target);
		for (uint32_t i=0; i<ctx->struct_body_item().size(); i++) {
			ext->add(ctx->struct_body_item(i)->accept(this));
		}
	} else if (type == "enum") {
		todo("enum type extension");
		ext = m_factory->mkExtend(IExtend::ExtendType_Enum, target);
	} else if (type == "component") {
		ext = m_factory->mkExtend(IExtend::ExtendType_Component, target);
		for (uint32_t i=0; i<ctx->component_body_item().size(); i++) {
			ext->add(ctx->component_body_item(i)->accept(this));
		}
	} else {
		error("unknown extension type %s", type.c_str());
	}

	leave("visitExtend_stmt");

	ret = ext;
	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitImport_stmt(PSSParser::Import_stmtContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitImport_stmt");
	IImport *imp = m_factory->mkImport(
			m_factory->mkRefType(scope(),
					type2vector(ctx->package_import_pattern()->type_identifier())),
			ctx->package_import_pattern()->wildcard);

	ret = imp;

	leave("visitImport_stmt");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitMethod_prototype(PSSParser::Method_prototypeContext *ctx) {
	IBaseItem *ret;

	enter("visitMethod_prototype");

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
				error("unknown parameter direction %s", dir.c_str());
			}
		}

		IBaseItem *type = p->data_type()->accept(this);
		const std::string &id = p->identifier()->getText();

		parameters.push_back(m_factory->mkField(id, type, attr, 0));
	}

	IImportFunc *method = m_factory->mkImportFunc(
			name, ret_type, parameters);

	ret = method;

	leave("visitMethod_prototype");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitFunction_decl(PSSParser::Function_declContext *ctx) {
	return ctx->method_prototype()->accept(this);
}

antlrcpp::Any PSS2PSIVisitor::visitImport_method_phase_qualifiers(PSSParser::Import_method_phase_qualifiersContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitMethod_prototype");

	todo("visitMethod_prototype");

	leave("visitMethod_prototype");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitImport_class_decl(PSSParser::Import_class_declContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitImport_class_decl");

	todo("visitImport_class_decl");

	leave("visitImport_class_decl");

	return ret;
}

antlrcpp::Any PSS2PSIVisitor::visitExport_action(PSSParser::Export_actionContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitExport_action");
	todo("visitExport_action");
	leave("visitExport_action");

	return ret;
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
	IBaseItem *super = 0;

	if (dynamic_cast<IAction *>(it)) {
		super = dynamic_cast<IAction *>(it)->getSuperType();
	} else if (dynamic_cast<IStruct *>(it)) {
		super = dynamic_cast<IStruct *>(it)->getSuperType();
	} else if (dynamic_cast<IComponent *>(it)) {
// TODO:		return dynamic_cast<IComponent *>(it)->getSuperType();
	}

	if (super) {
		if (super->getType() == IBaseItem::TypeRefType) {
			super = dynamic_cast<IRefType *>(super)->getTargetType();
		}
	}

	return dynamic_cast<IScopeItem *>(super);
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

std::vector<std::string> PSS2PSIVisitor::type2vector(PSSParser::Type_identifierContext *type) {
	std::vector<std::string> ret;
	for (uint32_t i=0; i<type->ID().size(); i++) {
		ret.push_back(type->ID(i)->getText());
	}

	return ret;
}

void PSS2PSIVisitor::enter(const char *fmt, ...) {
	va_list ap;
	if (m_debug) {
		va_start(ap, fmt);
		fprintf(stdout, "--> ");
		vfprintf(stdout, fmt, ap);
		fprintf(stdout, "\n");
		va_end(ap);
	}
}

void PSS2PSIVisitor::leave(const char *fmt, ...) {
	va_list ap;
	if (m_debug) {
		va_start(ap, fmt);
		fprintf(stdout, "<-- ");
		vfprintf(stdout, fmt, ap);
		fprintf(stdout, "\n");
		va_end(ap);
	}
}

void PSS2PSIVisitor::error(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "Error: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

void PSS2PSIVisitor::todo(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "TODO: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

void PSS2PSIVisitor::debug(const char *fmt, ...) {
	va_list ap;
	if (m_debug) {
		va_start(ap, fmt);
		fprintf(stdout, "[DEBUG] ");
		vfprintf(stdout, fmt, ap);
		fprintf(stdout, "\n");
		va_end(ap);
	}
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