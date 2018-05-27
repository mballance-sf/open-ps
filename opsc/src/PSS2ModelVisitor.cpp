/*
 * PSS2ModelVisitor.cpp
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
 * 
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#include "PSS2ModelVisitor.h"

#include <cstdio>
#include <exception>
#include <typeinfo>
#include <stdarg.h>
#include "VariableRefImpl.h"
#include "TargetTemplateBuilder.h"
#include <cstdio>
#include <iostream>

using namespace antlrcpp;
using namespace antlr4;


PSS2ModelVisitor::PSS2ModelVisitor(IModel *model, const std::string &path) :
		m_model(model), m_factory(model->getItemFactory()), m_file(path) {
	m_debug = true;
}

PSS2ModelVisitor::~PSS2ModelVisitor() {
	// TODO Auto-generated destructor stub
}

antlrcpp::Any PSS2ModelVisitor::visitModel(PSSParser::ModelContext *ctx) {

	enter("visitModel");
	push_scope(m_model);
	for (uint32_t i=0; i<ctx->portable_stimulus_description().size(); i++) {
		try {
		IBaseItem *it = ctx->portable_stimulus_description(i)->accept(this);
		if (it && it->getType() != IBaseItem::TypePackage) {
			if (m_debug) {
				INamedItem *ni = dynamic_cast<INamedItem *>(it);
				debug("Add item %d (%s) to root",
						it->getType(), (ni)?ni->getName().c_str():"unnamed");
			}
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

antlrcpp::Any PSS2ModelVisitor::visitAction_declaration(PSSParser::Action_declarationContext *ctx) {
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
			// This happens for data fields
//			error("null action item");
		}
	}
	pop_scope();

	leave("visitAction_declaration");
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitAbstract_action_declaration(PSSParser::Abstract_action_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitAbstract_action_declaration");

	leave("visitAbstract_action_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_declaration(PSSParser::Activity_declarationContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitActivity_declaration");

	IActivityBlockStmt *block = m_factory->mkActivityBlockStmt(IActivityBlockStmt::ActivityStmt_Block);
	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		IActivityStmt *stmt = ctx->activity_stmt(i)->accept(this);
		if (stmt) {
			block->add(stmt);
		} else {
			error("null activity stmt");
		}
	}
	if (dynamic_cast<IAction *>(scope())) {
		IAction *action = dynamic_cast<IAction *>(scope());
		action->setActivity(block);
	} else if (dynamic_cast<IExtendComposite *>(scope())) {
		IExtendComposite *ext = dynamic_cast<IExtendComposite *>(scope());
		ext->add(block);
	} else {
		todo("activity parent is neither an action nor an extension");
	}

	leave("visitActivity_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_labeled_stmt(PSSParser::Activity_labeled_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_labeled_stmt");
	todo("visitActivity_labeled_stmt");
	ret = ctx->labeled_activity_stmt()->accept(this);
	leave("visitActivity_labeled_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_bind_stmt(PSSParser::Activity_bind_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_bind_stmt");
	todo("visitActivity_bind_stmt\n");
	leave("visitActivity_bind_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_action_traversal_stmt(PSSParser::Activity_action_traversal_stmtContext *ctx) {
	IActivityStmt *ret = 0;
	IConstraintBlock *with_c = 0;

	enter("visitActivity_action_traveral_stmt");

	if (ctx->inline_with_constraint()) {
		std::vector<IConstraint *> constraints;

		if (ctx->inline_with_constraint()->single_stmt_constraint()) {
			IConstraint *c = ctx->inline_with_constraint()->single_stmt_constraint()->accept(this);
			if (c) {
				constraints.push_back(c);
			} else {
				fprintf(stdout, "Error: null constraint in single-statement action traversal\n");
			}
		} else {
			uint32_t n=ctx->inline_with_constraint()->constraint_body_item().size();
			for (uint32_t i=0; i<n; i++) {
				IConstraint *c = ctx->inline_with_constraint()->constraint_body_item(i)->accept(this);
				if (c) {
					constraints.push_back(c);
				} else {
					fprintf(stdout, "Error: null constraint in action traversal\n");
				}
			}
		}
		with_c = m_factory->mkConstraintBlock("", constraints);
	}

	if (ctx->is_do) {
		// Do <type>
		IActivityDoActionStmt *stmt = m_factory->mkActivityDoActionStmt(
				m_factory->mkRefType(scope(),
						type2vector(ctx->type_identifier())),
				with_c);
		ret = stmt;
	} else if (ctx->variable_ref()) {
		// <type>
		std::vector<PSSParser::Variable_refContext *> path;
		path.push_back(ctx->variable_ref());
		ret = m_factory->mkActivityTraverseStmt(
				m_factory->mkVariableRef(
						dynamic_cast<IBaseItem *>(scope()),
						ctx->variable_ref()->getText(), 0, 0),
				with_c);
	} else {
		todo("activity function_symbol_call");
	}

	leave("visitActivity_action_traveral_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_parallel_stmt(PSSParser::Activity_parallel_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_parallel_stmt");

	IActivityBlockStmt *parallel = m_factory->mkActivityBlockStmt(IActivityStmt::ActivityStmt_Parallel);

	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		IActivityStmt *stmt = ctx->activity_stmt(i)->accept(this);
		if (dynamic_cast<IActivityBlockStmt *>(stmt) &&
				dynamic_cast<IActivityBlockStmt *>(stmt)->getStmtType() == IActivityBlockStmt::ActivityStmt_Block) {
			parallel->add(stmt);
		} else {
			IActivityBlockStmt *stmt_b = m_factory->mkActivityBlockStmt(IActivityStmt::ActivityStmt_Block);
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

antlrcpp::Any PSS2ModelVisitor::visitActivity_repeat_stmt(PSSParser::Activity_repeat_stmtContext *ctx) {
	IActivityStmt *ret = 0;
	IExpr *expr = 0;

	enter("visitActivity_repeat_stmt");

	IActivityRepeatStmt::RepeatType type = IActivityRepeatStmt::RepeatType_Forever;
	IActivityBlockStmt *body = 0;

	if (ctx->expression()) {
		expr = ctx->expression()->accept(this);
	}

	if (ctx->is_while) {
		type = IActivityRepeatStmt::RepeatType_While;
	} else if (ctx->is_do_while) {
		type = IActivityRepeatStmt::RepeatType_Until;
	} else if (expr) {
		if (ctx->loop_var) {
			// TODO: must allow loop variable
		}
		type = IActivityRepeatStmt::RepeatType_Count;
	}

	IActivityStmt *stmt = ctx->activity_stmt()->accept(this);
	if (dynamic_cast<IActivityBlockStmt *>(stmt)) {
		body = dynamic_cast<IActivityBlockStmt *>(stmt);
	} else {
		body = m_factory->mkActivityBlockStmt();
		body->add(stmt);
	}

	IActivityRepeatStmt *rep = m_factory->mkActivityRepeatStmt(type, expr, body);

	if (ctx->loop_var) {
		IField *loop_var = m_factory->mkField(
				ctx->loop_var->getText(),
				m_factory->mkScalarType(IScalarType::ScalarType_Bit,
						m_factory->mkBitLiteral(32), 0, 0),
				IField::FieldAttr_None,
				0);
		rep->add(loop_var);
	}

	ret = rep;

	leave("visitActivity_repeat_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_constraint_stmt(PSSParser::Activity_constraint_stmtContext *ctx) {
	IActivityStmt *ret = 0;
	enter("visitActivity_constraint_stmt");

	todo("visitActivity_constraint_stmt\n");

	leave("visitActivity_constraint_stmt");
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_if_else_stmt(PSSParser::Activity_if_else_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_if_else_stmt");

	IExpr *cond = ctx->expression()->accept(this);
	IActivityStmt *true_stmt = ctx->activity_stmt(0)->accept(this);
	IActivityStmt *false_stmt = 0;

	if (ctx->activity_stmt().size() > 1) {
		false_stmt = ctx->activity_stmt(1)->accept(this);
	}

	ret = m_factory->mkActivityIfElseStmt(cond, true_stmt, false_stmt);
	leave("visitActivity_if_else_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_select_stmt(PSSParser::Activity_select_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_select_stmt");

	std::vector<IActivitySelectBranchStmt *> branches;

	for (uint32_t i=0; i<ctx->select_branch().size(); i++) {
		IExpr *guard = 0;
		IExpr *weight = 0;
		IActivityStmt *stmt = ctx->select_branch(i)->activity_stmt()->accept(this);

		if (ctx->select_branch(i)->select_guard_weight()) {
			if (ctx->select_branch(i)->select_guard_weight()->guard) {
				guard = ctx->select_branch(i)->select_guard_weight()->guard->accept(this);
			}
			if (ctx->select_branch(i)->select_guard_weight()->weight) {
				weight = ctx->select_branch(i)->select_guard_weight()->weight->accept(this);
			}
		}

		IActivitySelectBranchStmt *branch = m_factory->mkActivitySelectBranchStmt(
				stmt, guard, weight);
	}
	IActivitySelectStmt *select = m_factory->mkActivitySelectStmt(branches);

	ret = select;

	leave("visitActivity_select_stmt");
    return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_match_stmt(PSSParser::Activity_match_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_match_stmt");
	todo("visitActivity_match_stmt");
	leave("visitActivity_match_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_schedule_stmt(PSSParser::Activity_schedule_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_schedule_stmt");
	IActivityBlockStmt *schedule = m_factory->mkActivityBlockStmt(IActivityStmt::ActivityStmt_Schedule);

	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		IActivityStmt *stmt = ctx->activity_stmt(i)->accept(this);
		schedule->add(stmt);
	}

	ret = schedule;

	leave("visitActivity_schedule_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_foreach_stmt(PSSParser::Activity_foreach_stmtContext *ctx) {
	IActivityStmt *ret = 0;
	enter("visitActivity_schedule_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_sequence_block_stmt(PSSParser::Activity_sequence_block_stmtContext *ctx) {
	IActivityStmt *ret = 0;

	enter("visitActivity_sequence_block_stmt");

	IActivityBlockStmt *seq = m_factory->mkActivityBlockStmt(IActivityStmt::ActivityStmt_Block);
	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		IActivityStmt *stmt = ctx->activity_stmt(i)->accept(this);
		if (stmt) {
			seq->add(stmt);
		}
	}
	ret = seq;

	leave("visitActivity_sequence_block_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitSymbol_declaration(PSSParser::Symbol_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitSymbol_declaration");
	std::vector<IField *> params;

	if (ctx->symbol_paramlist()) {
		for (uint32_t i=0; i<ctx->symbol_paramlist()->symbol_param().size(); i++) {
			IBaseItem *t = ctx->symbol_paramlist()->symbol_param(i)->data_type()->accept(this);
			IField *p = m_factory->mkField(
					ctx->symbol_paramlist()->symbol_param(i)->identifier()->getText(),
					t,
					IField::FieldAttr_None,
					0);
			params.push_back(p);
		}
	}
	IActivityBlockStmt *body = m_factory->mkActivityBlockStmt(
			IActivityBlockStmt::ActivityStmt_Block);
	for (uint32_t i=0; i<ctx->activity_stmt().size(); i++) {
		IActivityStmt *body_s = ctx->activity_stmt(i)->accept(this);
		body->add(body_s);
	}

	ISymbol *sym = m_factory->mkSymbol(
			ctx->identifier()->getText(),
			params, body);
	ret = sym;

	leave("visitSymbol_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitOverrides_declaration(PSSParser::Overrides_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitOverrides_declaration");
	todo("visitOverrides_declaration");
	leave("visitOverrides_declaration");
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitStruct_declaration(PSSParser::Struct_declarationContext *ctx) {
	IBaseItem *ret = 0;
	IBaseItem *super_type = 0;

	enter("visitStruct_declaration");

	if (ctx->struct_super_spec()) {
		super_type = m_factory->mkRefType(scope(),
				type2vector(ctx->struct_super_spec()->type_identifier()));
	}

	IStruct::StructType t;
	const std::string &kind = ctx->struct_kind()->getText();
	if (kind == "struct") {
		t = IStruct::Base;
	} else if (kind == "buffer") {
		t = IStruct::Memory;
	} else if (kind == "stream") {
		t = IStruct::Stream;
	} else if (kind == "state") {
		t = IStruct::State;
	} else if (kind == "resource") {
		t = IStruct::Resource;
	} else {
		error("Unknown struct type \"%s\"\n", kind.c_str());
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

antlrcpp::Any PSS2ModelVisitor::visitAttr_field(PSSParser::Attr_fieldContext *ctx) {
	IBaseItem *ret = 0;
	IField::FieldAttr attr = IField::FieldAttr_None;

	enter("visitAttr_field");

	if (ctx->rand) {
		attr = IField::FieldAttr_Rand;
	}

	if (ctx->access_modifier()) {
		todo("Field access modifier");
	}

	IBaseItem *data_type = ctx->data_declaration()->data_type()->accept(this);

	for (uint32_t i=0; i<ctx->data_declaration()->data_instantiation().size(); i++) {
		PSSParser::Data_instantiationContext *di =
				ctx->data_declaration()->data_instantiation(i);

		if (di->plain_data_instantiation()) {
			PSSParser::Plain_data_instantiationContext *pi = di->plain_data_instantiation();
			IExpr *array_dim = 0;
			IBaseItem *field_data_type = data_type;

			if (pi->array_dim()) {
				bool has_sum = (dynamic_cast<IScalarType *>(data_type));
				IExpr *lhs = 0;
				IExpr *rhs = 0;

				if (pi->array_dim()->constant_expression()) {
					lhs = pi->array_dim()->constant_expression()->accept(this);
				}

				// Wrap data_type in an array_type
				field_data_type = m_factory->mkArrayType(
						data_type,
						has_sum,
						lhs,
						rhs);
			}

			IField *field = m_factory->mkField(
					pi->identifier()->getText(),
					field_data_type,
					attr,
					array_dim);

			scope()->add(field);

			if (m_debug) {
				INamedItem *ni = dynamic_cast<INamedItem *>(scope());
				debug("Add attr_field %s to scope %s\n",
						pi->identifier()->getText().c_str(),
						(ni)?ni->getName().c_str():"unnamed");
			}
		} else {
			todo("covergroup instance");
		}
	}

	leave("visitAttr_field");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitAttr_group(PSSParser::Attr_groupContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitAttr_group");
	todo("visitAttr_group");
	leave("visitAttr_group");

	return ret;
}

//antlrcpp::Any PSS2ModelVisitor::visitAction_field_declaration(PSSParser::Action_field_declarationContext *ctx) {
//	IBaseItem *ret = 0;
//	IField::FieldAttr attr = IField::FieldAttr_None;
//
//	enter("visitAction_field_declaration");
//
//#ifdef TODO
//	if (ctx->action_field_modifier()) {
//		const std::string &modifier = ctx->action_field_modifier()->getText();
//		if (modifier == "rand") {
//			attr = IField::FieldAttr_Rand;
//		} else if (modifier == "input") {
//			attr = IField::FieldAttr_Input;
//		} else if (modifier == "output") {
//			attr = IField::FieldAttr_Output;
//		} else if (modifier == "lock") {
//			attr = IField::FieldAttr_Lock;
//		} else if (modifier == "share") {
//			attr = IField::FieldAttr_Share;
//		} else if (modifier == "action") {
//			attr = IField::FieldAttr_Action;
//		} else {
//			todo("unknown action-field modifier %s", modifier.c_str());
//		}
//	}
//
//	IBaseItem *data_type = ctx->action_data_declaration()->action_data_type()->accept(this);
//
//	for (uint32_t i=0; i<ctx->action_data_declaration()->data_instantiation().size(); i++) {
//		PSSParser::Data_instantiationContext *di =
//				ctx->action_data_declaration()->data_instantiation(i);
//		// Array-dim as a field property is deprecated
//		IExpr *array_dim = 0;
//
//		if (di->array_dim()) {
//			bool has_sum = (dynamic_cast<IScalarType *>(data_type));
//			IExpr *lhs = 0;
//			IExpr *rhs = 0;
//
//			if (di->array_dim()->constant_expression()) {
//				lhs = di->array_dim()->constant_expression()->accept(this);
//			}
//
//			// Wrap data_type in an array_type
//			data_type = m_factory->mkArrayType(
//					data_type,
//					has_sum,
//					lhs,
//					rhs);
//		}
//
//		// TODO: use an 'array-type' to represent an array
//		// - Scalar array type
//		// - Has reference to data-type field
//		// - Has handles to 'size' and 'sum'
//		IField *field = m_factory->mkField(
//				di->identifier()->getText(),
//				data_type,
//				attr,
//				array_dim);
//
//		scope()->add(field);
//	}
//#endif
//
//	leave("visitAction_field_declaration");
//
//	return ret;
//}

antlrcpp::Any PSS2ModelVisitor::visitSub_action_field(PSSParser::Sub_action_fieldContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitSub_action_field");
	todo("visitSub_action_field");
	leave("visitSub_action_field");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitFlow_ref_field(PSSParser::Flow_ref_fieldContext *ctx) {
	IBaseItem *ret = 0;
	IField::FieldAttr attr;

	enter("visitFlow_ref_field");
	if (ctx->input) {
		attr = IField::FieldAttr_Input;
	} else if (ctx->output) {
		attr = IField::FieldAttr_Output;
	} else {
		error("flow_ref_field is missing input/output");
	}

	IBaseItem *data_type = ctx->type_identifier()->accept(this);

	for (uint32_t i=0; i<ctx->identifier().size(); i++) {
		IField *field = m_factory->mkField(
				ctx->identifier(i)->getText(),
				data_type,
				attr,
				0);
		scope()->add(field);
	}

	leave("visitFlow_ref_field");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitResource_ref_field(PSSParser::Resource_ref_fieldContext *ctx) {
	IBaseItem *ret = 0;
	IField::FieldAttr attr;

	enter("visitResource_ref_field");

	if (ctx->lock) {
		attr = IField::FieldAttr_Lock;
	} else if (ctx->share) {
		attr = IField::FieldAttr_Share;
	} else {
		error("resource_ref_field missing lock/share");
	}

	IBaseItem *data_type = ctx->type_identifier()->accept(this);

	for (uint32_t i=0; i<ctx->identifier().size(); i++) {
		IField *field = m_factory->mkField(
				ctx->identifier(i)->getText(),
				data_type,
				attr,
				0);
		scope()->add(field);
	}

	leave("visitResource_ref_field");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitActivity_data_field(PSSParser::Activity_data_fieldContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitActivity_data_field");
	IBaseItem *data_type = ctx->data_declaration()->data_type()->accept(this);

	for (uint32_t i=0; i<ctx->data_declaration()->data_instantiation().size(); i++) {
		const std::string &id = ctx->data_declaration()->data_instantiation(i)->plain_data_instantiation()->identifier()->getText();

		IField *field = m_factory->mkField(
				id,
				data_type,
				IField::FieldAttr_Action,
				0);
		scope()->add(field);
	}

	leave("visitActivity_data_field");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitScheduling_constraint(PSSParser::Scheduling_constraintContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitScheduling_constraint");
	todo("visitScheduling_constraint");
	leave("visitScheduling_constraint");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitData_type(PSSParser::Data_typeContext *ctx) {
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

antlrcpp::Any PSS2ModelVisitor::visitScalar_data_type(PSSParser::Scalar_data_typeContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitScalar_data_type");

	if (ctx->bool_type()) {
		ret = m_factory->mkScalarType(IScalarType::ScalarType_Bool, 0, 0);
	} else if (ctx->chandle_type()) {
		ret = m_factory->mkScalarType(IScalarType::ScalarType_Chandle, 0, 0);
	} else if (ctx->integer_type()) {
		IExpr *msb=0, *lsb=0;
		IOpenRangeList *domain;

		if (ctx->integer_type()->lhs) {
			if (ctx->integer_type()->rhs) {
				msb = ctx->integer_type()->expression(0)->accept(this);
				lsb = ctx->integer_type()->expression(1)->accept(this);
			} else {
				// Simple width
				lsb = ctx->integer_type()->expression(0)->accept(this);
			}
		} else {
			// Implied width
			if (ctx->integer_type()->integer_atom_type()->getText() == "int") {
				lsb = m_factory->mkBitLiteral(32);
			} else {
				lsb = m_factory->mkBitLiteral(1);
			}
		}

		if (ctx->integer_type()->domain_open_range_list()) {
			IOpenRangeList *domain =
					ctx->integer_type()->domain_open_range_list()->accept(this);
		}

		if (ctx->integer_type()->integer_atom_type()->getText() == "int") {
			ret = m_factory->mkScalarType(IScalarType::ScalarType_Int, msb, lsb);
		} else if (ctx->integer_type()->integer_atom_type()->getText() == "bit") {
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

antlrcpp::Any PSS2ModelVisitor::visitEnum_declaration(PSSParser::Enum_declarationContext *ctx) {
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

	leave("visitEnum_declaration");
	ret = e;
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitTypedef_declaration(PSSParser::Typedef_declarationContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitTypedef_declaration");
	todo("visitTypedef_declaration");
	leave("visitTypedef_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitComponent_field_declaration(PSSParser::Component_field_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitComponent_field_declaration");

	if (ctx->component_data_declaration()) {
		ret = ctx->component_data_declaration()->accept(this);
	} else {
		ret = ctx->component_pool_declaration()->accept(this);
	}

	leave("visitComponent_field_declaration");
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitComponent_data_declaration(PSSParser::Component_data_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitComponent_data_declaration");

	IField::FieldAttr attr = IField::FieldAttr_None;

	IBaseItem *data_type = ctx->data_declaration()->data_type()->accept(this);

	for (uint32_t i=0; i<ctx->data_declaration()->data_instantiation().size(); i++) {
		PSSParser::Data_instantiationContext *di =
				ctx->data_declaration()->data_instantiation(i);

		if (di->plain_data_instantiation()) {
			PSSParser::Plain_data_instantiationContext *pdi =
					di->plain_data_instantiation();

			IExpr *array_dim = 0;
			IBaseItem *field_data_type = data_type;

			if (pdi->array_dim()) {
				bool has_sum = (dynamic_cast<IScalarType *>(data_type));
				IExpr *lhs = 0;
				IExpr *rhs = 0;

				if (pdi->array_dim()->constant_expression()) {
					lhs = pdi->array_dim()->constant_expression()->accept(this);
				}

				// Wrap data_type in an array_type
				field_data_type = m_factory->mkArrayType(
						data_type,
						has_sum,
						lhs,
						rhs);
			}

			if (m_debug) {
				debug("Note: field %s has data type of %d\n",
						pdi->identifier()->getText().c_str(),
						field_data_type->getType());
			}
			IField *field = m_factory->mkField(
					pdi->identifier()->getText(),
					field_data_type,
					attr,
					array_dim);

			if (m_debug) {
				debug("Add component field %s\n",
						pdi->identifier()->getText().c_str());
			}

			scope()->add(field);
		} else {
			fprintf(stdout, "Error: illegal covergroup instance in a component\n");
		}
	}

	leave("visitComponent_data_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitComponent_pool_declaration(PSSParser::Component_pool_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitComponent_pool_declaration");
	IBaseItem *pool_type = ctx->data_declaration()->data_type()->accept(this);
	IExpr *pool_size = 0;

	if (ctx->expression()) {
		pool_size = ctx->expression()->accept(this);
	}

	for (uint32_t i=0;
			i<ctx->data_declaration()->data_instantiation().size(); i++) {
		const std::string &id = ctx->data_declaration()->data_instantiation(i)->
				plain_data_instantiation()->identifier()->getText();

		IPool *pool = m_factory->mkPool(id, pool_type, pool_size);

		scope()->add(pool);
	}

	leave("visitComponent_pool_declaration");
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitObject_bind_stmt(PSSParser::Object_bind_stmtContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitObject_bind_stmt");

	todo("object_bind_stmt");

	leave("visitObject_bind_stmt");
	return ret;
}


antlrcpp::Any PSS2ModelVisitor::visitComponent_declaration(PSSParser::Component_declarationContext *ctx) {
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

antlrcpp::Any PSS2ModelVisitor::visitDomain_open_range_value(PSSParser::Domain_open_range_valueContext *ctx) {
	IOpenRangeValue *ret = 0;
	IExpr *lhs=0, *rhs=0;
	bool domain_bound = false;

	if (ctx->lhs) {
		lhs = ctx->lhs->accept(this);
		if (ctx->rhs) {
			// dual bound
			rhs = ctx->rhs->accept(this);
		} else if (ctx->limit_high) {
			domain_bound = true;

		}
	} else if (ctx->rhs) {
		domain_bound = true;
		rhs = ctx->rhs->accept(this);
	}

	ret = m_factory->mkOpenRangeValue(lhs, rhs, domain_bound);

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitDomain_open_range_list(PSSParser::Domain_open_range_listContext *ctx) {
	IOpenRangeList *ret = 0;
	std::vector<IOpenRangeValue *> ranges;

	for (uint32_t i=0; i<ctx->domain_open_range_value().size(); i++) {
		ranges.push_back(ctx->domain_open_range_value(i)->accept(this));
	}
	ret = m_factory->mkOpenRangeList(ranges);

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitOpen_range_value(PSSParser::Open_range_valueContext *ctx) {
	IOpenRangeValue *ret = 0;
	IExpr *lhs=0, *rhs=0;

	lhs = ctx->lhs->accept(this);
	if (ctx->rhs) {
		rhs = ctx->rhs->accept(this);
	}

	ret = m_factory->mkOpenRangeValue(lhs, rhs);

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitOpen_range_list(PSSParser::Open_range_listContext *ctx) {
	IOpenRangeList *ret = 0;
	std::vector<IOpenRangeValue *> ranges;

	for (uint32_t i=0; i<ctx->open_range_value().size(); i++) {
		ranges.push_back(ctx->open_range_value(i)->accept(this));
	}
	ret = m_factory->mkOpenRangeList(ranges);

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitConstraint_declaration(PSSParser::Constraint_declarationContext *ctx) {
	IBaseItem *ret = 0;
	std::vector<IConstraint *> constraints;
	std::string name;
	bool is_dynamic = false;

	enter("visitConstraint_declaration");

	if (ctx->single_stmt_constraint()) {
		IConstraint *c = ctx->single_stmt_constraint()->accept(this);
		if (c) {
			constraints.push_back(c);
		} else {
			fprintf(stdout, "Error: null constraint in single constraint\n");
		}
	} else {
		is_dynamic = (ctx->is_dynamic != 0);

		if (ctx->identifier()) {
			name = ctx->identifier()->getText();
		}

		for (uint32_t i=0; i<ctx->constraint_body_item().size(); i++) {
			IConstraint *c = ctx->constraint_body_item(i)->accept(this);

			if (c) {
				constraints.push_back(c);
			} else {
				error("null constraint in %s", name.c_str());
			}
		}
	}

	ret = m_factory->mkConstraintBlock(name, constraints);

	leave("visitConstraint_declaration");

    return ret;
}
antlrcpp::Any PSS2ModelVisitor::visitExpression_constraint_item(PSSParser::Expression_constraint_itemContext *ctx) {
	IConstraint *ret = 0;

	enter("visitExpression_constraint_item");

	IExpr *expr = ctx->expression()->accept(this);

	if (!expr) {
		fprintf(stdout, "Error: null constraint expression: %s\n",
				ctx->expression()->getText().c_str());
		return ret;
	}

	if (ctx->implicand_constraint_item()) {
		ret = m_factory->mkConstraintImplies(expr,
				ctx->implicand_constraint_item()->accept(this));
	} else {
		ret = m_factory->mkConstraintExpr(expr);
	}

	leave("visitExpression_constraint_item");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitIf_constraint_item(PSSParser::If_constraint_itemContext *ctx) {
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
antlrcpp::Any PSS2ModelVisitor::visitForeach_constraint_item(PSSParser::Foreach_constraint_itemContext *ctx) {
	IConstraint *ret = 0;

	enter("visitForeach_constraint_item");
	IExpr *expr = ctx->expression()->accept(this);
	IConstraint *cs = ctx->constraint_set()->accept(this);
	IConstraintBlock *body;
	IVariableRef *target = 0;
	std::string iterator;

	if (expr->getType() != IExpr::ExprType_VariableRef) {
		fprintf(stdout, "Error: foreach expression is not variable-ref\n");
		return ret;
	}

	IVariableRef *last = dynamic_cast<IVariableRef *>(expr);
	while (last->getNext()) {
		last = last->getNext();
	}

	if (!last->getIndexLhs()) {
		fprintf(stdout, "Error: foreach expression is missing an index\n");
		return ret;
	} else if (last->getIndexLhs()->getType() != IExpr::ExprType_VariableRef) {
		fprintf(stdout, "Error: foreach expression index isn't a variable-ref\n");
		return ret;
	}
	iterator = dynamic_cast<IVariableRef *>(last->getIndexLhs())->getId();

	VariableRefImpl *last_vr = dynamic_cast<VariableRefImpl *>(last);
	delete last_vr->getIndexLhs();
	last_vr->setIndexLhs(0);

	if (dynamic_cast<IConstraintBlock *>(cs)) {
		body = dynamic_cast<IConstraintBlock *>(cs);
	} else {
		std::vector<IConstraint *> c;
		if (cs) {
			c.push_back(cs);
		} else {
			fprintf(stdout, "Error: null constraint in foreach");
		}
		body = m_factory->mkConstraintBlock("", c);
	}

	ret = m_factory->mkConstraintForeach(target, iterator, body);
	leave("visitForeach_constraint_item");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitConstraint_set(PSSParser::Constraint_setContext *ctx) {
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

antlrcpp::Any PSS2ModelVisitor::visitConstraint_block(PSSParser::Constraint_blockContext *ctx) {
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

antlrcpp::Any PSS2ModelVisitor::visitUnique_constraint_item(PSSParser::Unique_constraint_itemContext *ctx) {
	IConstraint *ret = 0;
	enter("visitUnique_constraint_item");
	ret = m_factory->mkConstraintUnique(ctx->open_range_list()->accept(this));
	leave("visitUnique_constraint_item");
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitCovergroup_declaration(PSSParser::Covergroup_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitCovergroup_declaration");
	todo("visitCovergroup_declaration");
	leave("visitCovergroup_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitInline_covergroup(PSSParser::Inline_covergroupContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitInline_covergroup");
	todo("visitInline_covergroup");
	leave("visitInline_covergroup");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitCovergroup_option(PSSParser::Covergroup_optionContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitCovergroup_option");
	todo("visitCovergroup_option");
	leave("visitCovergroup_option");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitCovergroup_type_option(PSSParser::Covergroup_type_optionContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitCovergroup_type_option");
	todo("visitCovergroup_type_option");
	leave("visitCovergroup_type_option");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitCovergroup_coverpoint(PSSParser::Covergroup_coverpointContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitCovergroup_coverpoint");
	todo("visitCovergroup_coverpoint");
	leave("visitCovergroup_coverpoint");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitPackage_declaration(PSSParser::Package_declarationContext *ctx) {
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

antlrcpp::Any PSS2ModelVisitor::visitNull_stmt(PSSParser::Null_stmtContext *ctx) {
	IBaseItem *ret = 0;

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitConst_field_declaration(PSSParser::Const_field_declarationContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitConst_field_declaration");
	todo("visitConst_field_declaration");
	leave("visitConst_field_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitStatic_const_field_declaration(PSSParser::Static_const_field_declarationContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitStatic_const_field_declaration");
	todo("visitStatic_const_field_declaration");
	leave("visitStatic_const_field_declaration");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitExpression(PSSParser::ExpressionContext *ctx) {
	bool is_bin_op;
	IExpr *ret = 0;
	IExpr *expr = m_expr;

	enter("visitExpression");

	if (ctx->unary_op()) {
		todo("unary op");
		ret = ctx->lhs->accept(this);
	} else if (ctx->cast_op()) {
		todo("cast op");
		ret = ctx->lhs->accept(this);
	} else if (ctx->exp_op()) {
		// TODO:
		todo("exp op");
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
		todo("shift_op");
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
		todo("binary_xor_op");
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
	} else if (ctx->inside_expr_term()) {
		IExpr *lhs = ctx->lhs->accept(this);
		IOpenRangeList *rhs = ctx->inside_expr_term()->open_range_list()->accept(this);
		ret = m_factory->mkInExpr(lhs, rhs);
	} else {
		todo("unknown expression %s\n", ctx->getText().c_str());
		enter("unknown %s\n", ctx->getText().c_str());
		/* ret = */ visitChildren(ctx);
		leave("unknown");
	}

	leave("visitExpression");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitPrimary(PSSParser::PrimaryContext *ctx) {
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
		const std::string &s = ctx->string_literal()->getText();
		ret = m_factory->mkStringLiteral(s.substr(1, s.size()-2));
	} else if (ctx->variable_ref_path()) {
		IVariableRef *vref = 0;
		IVariableRef *prev = 0;
		IBaseItem *s = dynamic_cast<IBaseItem *>(scope());

		for (uint32_t i=0; i<ctx->variable_ref_path()->variable_ref().size(); i++) {
			PSSParser::Variable_refContext *ref_ctx = ctx->variable_ref_path()->variable_ref(i);
			const std::string &id = ref_ctx->identifier()->getText();
			IExpr *index_lhs = 0, *index_rhs = 0;
			if (ref_ctx->expression(0)) {
				index_lhs = ref_ctx->expression(0)->accept(this);
			}
			if (ref_ctx->expression(1)) {
				index_rhs = ref_ctx->expression(1)->accept(this);
			}

			IVariableRef *ref = m_factory->mkVariableRef(s, id, index_lhs, index_rhs);

			if (i == 0) {
				vref = ref;
			}
			if (prev) {
				prev->setNext(ref);
			}
			prev = ref;
		}

		ret = vref;
	} else if (ctx->method_function_call()) {
//		ctx->method_function_call()->function_call()->method_parameter_list()->expression().size();
		if (ctx->method_function_call()->function_symbol_call()) {
			PSSParser::Function_symbol_callContext *call_ctx  =
					ctx->method_function_call()->function_symbol_call();
			const std::vector<PSSParser::ExpressionContext *> &param_ctx =
					call_ctx->method_parameter_list()->expression();
			std::vector<IExpr *> parameters;

			for (uint32_t i=0; i<param_ctx.size(); i++) {
				parameters.push_back(param_ctx.at(i)->accept(this));
			}
			IBaseItem *func = ctx->method_function_call()->function_symbol_call()->function_symbol_id()->accept(this);
			ret = m_factory->mkMethodCallExpr(dynamic_cast<IRefType *>(func), parameters);
		} else {
			todo("method_call");
		}

	} else {
		error("unknown primary %s", ctx->getText().c_str());
	}

	leave("visitPrimary");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitUser_defined_datatype(PSSParser::User_defined_datatypeContext *ctx) {
	enter("visitUser_defined_datatype");

	IBaseItem *ret = ctx->type_identifier()->accept(this);

	leave("visitUser_defined_datatype");

	return ret;
}


IFieldRef *PSS2ModelVisitor::elaborate_field_ref(
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

IImportFunc *PSS2ModelVisitor::find_import_func(
		PSSParser::Function_symbol_callContext *func
		) {
	IImportFunc *ret = 0;
	PSSParser::Type_identifierContext *func_id = func->function_symbol_id()->type_identifier();

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

IBaseItem *PSS2ModelVisitor::find_type(PSSParser::Type_identifierContext *type) {
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

antlrcpp::Any PSS2ModelVisitor::visitExec_block(PSSParser::Exec_blockContext *ctx) {
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
	} else if (kind_s == "init") {
		kind = IExec::Init;
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

antlrcpp::Any PSS2ModelVisitor::visitTarget_code_exec_block(PSSParser::Target_code_exec_blockContext *ctx) {
	IExec *exec = 0;
	IBaseItem *ret = 0;

	enter("visitTarget_code_exec_block");

	std::string kind_s = ctx->exec_kind_identifier()->getText();
	std::vector<IExecReplacementExpr *> replacements;

	IExec::ExecKind kind;

	if (kind_s == "body") {
		kind = IExec::Body;
	} else if (kind_s == "pre_solve") {
		kind = IExec::PreSolve;
	} else if (kind_s == "post_solve") {
		kind = IExec::PostSolve;
	} else if (kind_s == "init") {
		kind = IExec::Init;
	} else {
		error("unknown exec kind \"%s\"", kind_s.c_str());
	}

	std::string templ;

	if (ctx->string()->DOUBLE_QUOTED_STRING()) {
		templ = ctx->string()->DOUBLE_QUOTED_STRING()->getText();
		// Remove leading and trailing double quotes
		templ = templ.substr(1, templ.size()-2);
	} else if (ctx->string()->TRIPLE_DOUBLE_QUOTED_STRING()) {
		templ = ctx->string()->TRIPLE_DOUBLE_QUOTED_STRING()->getText();
		templ = templ.substr(3, templ.size()-6);
	} else {
		fprintf(stdout, "Error: unknown type of string\n");
	}

	fprintf(stdout, "template=%s\n", templ.c_str());

	replacements = TargetTemplateBuilder::build(templ, m_factory,
			dynamic_cast<IBaseItem *>(scope()));

	exec = m_factory->mkTargetTemplateExec(
			kind,
			ctx->language_identifier()->getText(),
			templ,
			replacements);

	leave("visitTarget_code_exec_block");

	ret = exec;
	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitTarget_file_exec_block(PSSParser::Target_file_exec_blockContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitTarget_file_exec_block");
	todo("visitTarget_file_exec_block");
	leave("visitTarget_file_exec_block");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitExec_body_stmt(PSSParser::Exec_body_stmtContext *ctx) {
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
		IExpr *rhs = 0;
		if (ctx->expression().size() > 1) {
			try {
				rhs = ctx->expression(1)->accept(this);
			} catch (std::bad_cast &e) {
				error("IExprBodyStmt: bad-cast on RHS");
			}
		}

		ret = m_factory->mkExecExprStmt(
				expr,
				op,
				rhs);
		leave("visit_expression (2)");
	} else {
		// Just an expression statement
		// TODO: must support expression on LHS
		ret = m_factory->mkExecExprStmt((IFieldRef *)expr, IExecExprStmt::AssignOp_None, 0);
	}

	leave("visitExec_body_stmt\n");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitExtend_stmt(PSSParser::Extend_stmtContext *ctx) {
	IBaseItem *ret = 0;
	std::string type = ctx->ext_type->getText();

	enter("visitExtend_stmt");

	IRefType *target = m_factory->mkRefType(scope(),
			type2vector(ctx->type_identifier()));

	if (type == "action") {
		IExtendComposite *ext = m_factory->mkExtendComposite(IExtend::ExtendType_Action, target);
		push_scope(ext);
		for (uint32_t i=0; i<ctx->action_body_item().size(); i++) {
			IBaseItem *item = ctx->action_body_item(i)->accept(this);
			if (item) {
				ext->add(item);
			}
		}
		pop_scope();
		ret = ext;
	} else if (type == "struct") {
		IExtendComposite *ext = m_factory->mkExtendComposite(IExtend::ExtendType_Struct, target);
		push_scope(ext);
		for (uint32_t i=0; i<ctx->struct_body_item().size(); i++) {
			IBaseItem *item = ctx->struct_body_item(i)->accept(this);
			if (item) {
				ext->add(item);
			}
		}
		pop_scope();
		ret = ext;
	} else if (type == "enum") {
		std::vector<IEnumerator *> enumerators;
		for (uint32_t i=0; i<ctx->enum_item().size(); i++) {
			const std::string &id = ctx->enum_item(i)->identifier()->getText();
			IExpr *v = 0;

			if (ctx->enum_item(i)->constant_expression()) {
				v = ctx->enum_item(i)->constant_expression()->accept(this);
			}

			enumerators.push_back(m_factory->mkEnumerator(id, v));
		}
		IExtendEnum *ext = m_factory->mkExtendEnum(target, enumerators);
		ret = ext;
	} else if (type == "component") {
		IExtendComposite *ext = m_factory->mkExtendComposite(IExtend::ExtendType_Component, target);
		push_scope(ext);
		for (uint32_t i=0; i<ctx->component_body_item().size(); i++) {
			IBaseItem *item = ctx->component_body_item(i)->accept(this);
			if (item) {
				ext->add(item);
			}
		}
		pop_scope();
		ret = ext;
	} else {
		error("unknown extension type %s", type.c_str());
	}

	leave("visitExtend_stmt");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitImport_stmt(PSSParser::Import_stmtContext *ctx) {
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

antlrcpp::Any PSS2ModelVisitor::visitMethod_prototype(PSSParser::Method_prototypeContext *ctx) {
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

	leave("visitMethod_prototype %p %s", method, (method)?method->getName().c_str():"unknown");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitFunction_decl(PSSParser::Function_declContext *ctx) {
	return ctx->method_prototype()->accept(this);
}

antlrcpp::Any PSS2ModelVisitor::visitImport_method_phase_qualifiers(PSSParser::Import_method_phase_qualifiersContext *ctx) {
	IBaseItem *ret = 0;
	enter("visitImport_method_phase_qualifiers");
	todo("visitImport_method_phase_qualifiers");
	leave("visitImport_method_phase_qualifiers");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitImport_class_decl(PSSParser::Import_class_declContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitImport_class_decl");

	todo("visitImport_class_decl");

	leave("visitImport_class_decl");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitExport_action(PSSParser::Export_actionContext *ctx) {
	IBaseItem *ret = 0;

	enter("visitExport_action");
	todo("visitExport_action");
	leave("visitExport_action");

	return ret;
}

antlrcpp::Any PSS2ModelVisitor::visitType_identifier(PSSParser::Type_identifierContext *ctx) {
	IBaseItem *ret = 0;
	std::vector<std::string> type;

	enter("visitType_identifier");

	for (uint32_t i=0; i<ctx->ID().size(); i++) {
		type.push_back(ctx->ID(i)->getText());
	}

	ret = m_factory->mkRefType(scope(), type);

	leave("visitType_identifier");

	return ret;
}

IExpr *PSS2ModelVisitor::expression(PSSParser::ExpressionContext *ctx) {
	ctx->accept(this);
	return 0;
}

void PSS2ModelVisitor::push_scope(IScopeItem *scope) {
	m_scopes.push_back(scope);
}

IScopeItem *PSS2ModelVisitor::scope() const {
	return m_scopes.at(m_scopes.size()-1);
}

IScopeItem *PSS2ModelVisitor::pop_scope() {
	IScopeItem *s = scope();
	m_scopes.pop_back();
	return s;
}

IScopeItem *PSS2ModelVisitor::getSuperType(IScopeItem *it) {
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

std::pair<IBaseItem *, IScopeItem *> PSS2ModelVisitor::find_type(const std::string &name) {
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

IBaseItem *PSS2ModelVisitor::find_type(IScopeItem *scope, const std::string &name) {
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

std::vector<std::string> PSS2ModelVisitor::type2vector(PSSParser::Type_identifierContext *type) {
	std::vector<std::string> ret;
	for (uint32_t i=0; i<type->ID().size(); i++) {
		ret.push_back(type->ID(i)->getText());
	}

	return ret;
}

void PSS2ModelVisitor::enter(const char *fmt, ...) {
	va_list ap;
	if (m_debug) {
		va_start(ap, fmt);
		fprintf(stdout, "--> ");
		vfprintf(stdout, fmt, ap);
		fprintf(stdout, "\n");
		va_end(ap);
	}
}

void PSS2ModelVisitor::leave(const char *fmt, ...) {
	va_list ap;
	if (m_debug) {
		va_start(ap, fmt);
		fprintf(stdout, "<-- ");
		vfprintf(stdout, fmt, ap);
		fprintf(stdout, "\n");
		va_end(ap);
	}
}

void PSS2ModelVisitor::error(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "Error: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

void PSS2ModelVisitor::todo(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "TODO: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

void PSS2ModelVisitor::debug(const char *fmt, ...) {
	va_list ap;
	if (m_debug) {
		va_start(ap, fmt);
		fprintf(stdout, "[DEBUG] ");
		vfprintf(stdout, fmt, ap);
		fprintf(stdout, "\n");
		va_end(ap);
	}
}

void PSS2ModelVisitor::fatal(const std::string &msg) {
	throw msg;
}

void PSS2ModelVisitor::fatal(
		antlr4::Token 		*loc,
		const std::string	&msg) {
	std::string error_msg = "Fatal: " + msg;
	char tmp[128];

	error_msg += " " + m_file;
	sprintf(tmp, ":%d", loc->getLine());
	error_msg += tmp;

	throw error_msg;
}
