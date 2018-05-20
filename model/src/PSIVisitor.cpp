/*
 * PSIProcessor.cpp
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
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
 *  Created on: Jul 15, 2016
 *      Author: ballance
 */

#include "PSIVisitor.h"
#include "ModelImpl.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>


PSIVisitor::PSIVisitor() : m_removed(false) {
	m_debug = true;
}

PSIVisitor::~PSIVisitor() {
	// TODO Auto-generated destructor stub
}

void PSIVisitor::visit_model(IModel *model) {
	ModelImpl *model_i = dynamic_cast<ModelImpl *>(model);
	push_scope(model);
	m_removed = false;
	visit_scope(model_i);
	pop_scope();
	m_removed = false;
}

void PSIVisitor::visit_package(IPackage *pkg) {
	std::vector<IBaseItem *>::const_iterator it=pkg->getItems().begin();

	push_scope(pkg);
	visit_scope(pkg);
	pop_scope();
}

void PSIVisitor::visit_action(IAction *a) {

	if (a->getSuperType()) {
		visit_ref_type(dynamic_cast<IRefType *>(a->getSuperType()));
	}

	push_scope(a);
	visit_scope(a);

	if (a->getActivity()) {
		visit_activity(a->getActivity());
	}
	pop_scope();

	m_removed = false;
}

void PSIVisitor::visit_bind(IBind *b) {

}

void PSIVisitor::visit_struct(IStruct *str) {
	m_removed = false;

	if (str->getSuperType()) {
		visit_ref_type(dynamic_cast<IRefType *>(str->getSuperType()));
	}

	push_scope(str);
	visit_scope(str);
	pop_scope();

	m_removed = false;
}

void PSIVisitor::visit_component(IComponent *c) {

	// TODO: visit super
	push_scope(c);
	visit_scope(c);
	pop_scope();
}

void PSIVisitor::visit_constraint(IConstraintBlock *c) {
	for (std::vector<IConstraint *>::const_iterator it=c->getConstraints().begin();
			it!=c->getConstraints().end(); it++) {
		visit_constraint_stmt(*it);
	}
}

void PSIVisitor::visit_constraint_stmt(IConstraint *c) {
	switch (c->getConstraintType()) {
	case IConstraint::ConstraintType_Block: {
		visit_constraint_block(dynamic_cast<IConstraintBlock *>(c));
	} break;

	case IConstraint::ConstraintType_Expr:
		visit_constraint_expr_stmt(dynamic_cast<IConstraintExpr *>(c));
		break;

	case IConstraint::ConstraintType_If:
		visit_constraint_if_stmt(dynamic_cast<IConstraintIf *>(c));
		break;
	}
}

void PSIVisitor::visit_constraint_expr_stmt(IConstraintExpr *c) {
	visit_expr(c->getExpr());
}

void PSIVisitor::visit_constraint_if_stmt(IConstraintIf *c) {
	visit_expr(c->getCond());

	visit_constraint_stmt(c->getTrue());

	if (c->getFalse()) {
		visit_constraint_stmt(c->getFalse());
	}
}

void PSIVisitor::visit_constraint_block(IConstraintBlock *block) {
	std::vector<IConstraint *>::const_iterator it = block->getConstraints().begin();

	for (; it!=block->getConstraints().end(); it++) {
		IConstraint *c = *it;
		visit_constraint_stmt(c);
	}
}

void PSIVisitor::visit_coverspec(ICoverspec *coverspec) {
	// TODO:
}

void PSIVisitor::visit_exec(IExec *e) {
	switch (e->getExecType()) {
	case IExec::TargetTemplate: {
		for (uint32_t i=0; i<e->getTargetTemplateReplacements().size(); i++) {
			visit_target_template_replacement(
					e->getTargetTemplateReplacements().at(i));
		}
	} break;
	case IExec::Native: {
		for (std::vector<IExecStmt *>::const_iterator it=e->getStmts().begin();
				it!=e->getStmts().end(); it++) {
			visit_exec_stmt(*it);
		}
	} break;

	}
}

void PSIVisitor::visit_exec_stmt(IExecStmt *e) {
	switch (e->getStmtType()) {
	case IExecStmt::StmtType_Expr: {
		visit_exec_expr_stmt(dynamic_cast<IExecExprStmt *>(e));
	} break;

	case IExecStmt::StmtType_Vendor: {
		visit_exec_vendor_stmt(e);
	} break;

	default:
		fprintf(stdout, "Error: unknown exec stmt %d\n", e->getStmtType());
		break;
	}
//	switch (e->getStmtType()) {
//	case IExecStmt::StmtType_Call:
//		visit_exec_call_stmt(dynamic_cast<IMethodCallExpr *>(e));
//		break;
//
//	case IExecStmt::StmtType_Expr:
//		visit_exec_expr_stmt(dynamic_cast<IExecExprStmt *>(e));
//		break;
//
//	case IExecStmt::StmtType_Vendor:
//		visit_exec_vendor_stmt(e);
//		break;
//	}
}

void PSIVisitor::visit_exec_call_stmt(IMethodCallExpr *s) {
//	fprintf(stdout, "visit_exec_call_stmt %p\n", s->getFunc());
}

void PSIVisitor::visit_exec_expr_stmt(IExecExprStmt *s) {
	if (s->getLhs()) {
		visit_expr(s->getLhs());
	}
	if (s->getRhs()) {
		visit_expr(s->getRhs());
	}
}

void PSIVisitor::visit_exec_vendor_stmt(IExecStmt *s) {
	// NOP
}

void PSIVisitor::visit_target_template_replacement(IExecReplacementExpr *r) {
	visit_expr(r->getExpr());
}

void PSIVisitor::visit_expr(IExpr *e) {
	if (!e) {
		fprintf(stdout, "Error: null expression\n");
		e->getType();
		return;
	}
	switch (e->getType()) {
		case IExpr::ExprType_BinOp:
			visit_binary_expr(dynamic_cast<IBinaryExpr *>(e));
			break;
		case IExpr::ExprType_FieldRef:
			visit_fieldref_expr(dynamic_cast<IFieldRef *>(e));
			break;
		case IExpr::ExprType_Literal:
			visit_literal_expr(dynamic_cast<ILiteral *>(e));
			break;
		case IExpr::ExprType_VariableRef:
			visit_variable_ref(dynamic_cast<IVariableRef *>(e));
			break;
		case IExpr::ExprType_MethodCall:
			visit_method_call(dynamic_cast<IMethodCallExpr *>(e));
			break;
		case IExpr::ExprType_In:
			visit_in_expr(dynamic_cast<IInExpr *>(e));
			break;
		default:
			fprintf(stdout, "Error: unhandled expr type %d\n", e->getType());
			break;
	}
}

void PSIVisitor::visit_extend(IExtend *e) {
	push_scope(e);
	visit_item(e->getTarget());

	switch (e->getExtendType()) {
	case IExtend::ExtendType_Enum:
		visit_extend_enum(dynamic_cast<IExtendEnum *>(e));
		break;
	case IExtend::ExtendType_Component:
	case IExtend::ExtendType_Action:
	case IExtend::ExtendType_Struct:
		visit_extend_composite(dynamic_cast<IExtendComposite *>(e));
		break;
	default:
		fprintf(stdout, "Error: unhandled extension type %d\n",
				e->getExtendType());
	}
	pop_scope();
}

void PSIVisitor::visit_extend_composite(IExtendComposite *e) {
	push_scope(e);
	visit_scope(e);
	pop_scope();
}

void PSIVisitor::visit_extend_enum(IExtendEnum *e) {
	// Nothing do to
}

void PSIVisitor::visit_binary_expr(IBinaryExpr *be) {
	visit_expr(be->getLHS());
	visit_expr(be->getRHS());
}

void PSIVisitor::visit_fieldref_expr(IFieldRef *ref) {

}

void PSIVisitor::visit_in_expr(IInExpr *in) {
	visit_expr(in->getLhs());
	visit_open_range_list(in->getRhs());
}

void PSIVisitor::visit_enum_type(IEnumType *e) {

}

void PSIVisitor::visit_literal_expr(ILiteral *l) {

}

void PSIVisitor::visit_method_call(IMethodCallExpr *c) {
	if (m_debug) {
		debug("--> visit_method_call %s", c->getFunc()->toString().c_str());
	}
	visit_ref_type(c->getFunc());

	for (std::vector<IExpr *>::const_iterator it=c->getParameters().begin();
			it!=c->getParameters().end(); it++) {
		visit_expr(*it);
	}

	if (m_debug) {
		debug("<-- visit_method_call %s", c->getFunc()->toString().c_str());
	}
}

void PSIVisitor::visit_open_range_list(IOpenRangeList *range_l) {
	for (std::vector<IOpenRangeValue *>::const_iterator it=range_l->ranges().begin();
			it!=range_l->ranges().end(); it++) {
		visit_open_range_value(*it);
	}
}

void PSIVisitor::visit_open_range_value(IOpenRangeValue *range_v) {
	if (range_v->getLHS()) {
		visit_expr(range_v->getLHS());
	}
	if (range_v->getRHS()) {
		visit_expr(range_v->getRHS());
	}
}

void PSIVisitor::visit_field(IField *f) {
	// Visiting the component type of a 'comp'
	// field results in recursion
	if (f->getName() != "comp") {
		visit_item(f->getDataType());
	}
}

void PSIVisitor::visit_activity(IActivityStmt *activity) {
	if (activity->getStmtType() == IActivityStmt::ActivityStmt_Block) {
		IActivityBlockStmt *b = dynamic_cast<IActivityBlockStmt *>(activity);
		push_graph(b);
		for (std::vector<IActivityStmt *>::const_iterator it=b->getStmts().begin();
				it!=b->getStmts().end(); it++) {
			visit_activity_stmt(*it);
		}
		pop_graph();
	} else {
		visit_activity_stmt(activity);
	}
}

void PSIVisitor::visit_activity_stmt(IActivityStmt *stmt) {
	push_graph(stmt);

	switch (stmt->getStmtType()) {
	case IActivityStmt::ActivityStmt_Block: {
		visit_activity_block_stmt(dynamic_cast<IActivityBlockStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_IfElse: {
		visit_activity_if_else_stmt(dynamic_cast<IActivityIfElseStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_Parallel: {
		visit_activity_parallel_block_stmt(dynamic_cast<IActivityBlockStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_Schedule: {
		visit_activity_schedule_block_stmt(dynamic_cast<IActivityBlockStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_Select: {
		visit_activity_select_stmt(dynamic_cast<IActivityBlockStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_Repeat: {
		visit_activity_repeat_stmt(dynamic_cast<IActivityRepeatStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_Traverse: {
		visit_activity_traverse_stmt(dynamic_cast<IActivityTraverseStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_DoAction: {
		visit_activity_do_action_stmt(dynamic_cast<IActivityDoActionStmt *>(stmt));
	} break;

	default: fprintf(stdout, "TODO: handle activity stmt %d\n", stmt->getStmtType());

	}

	pop_graph();
}

void PSIVisitor::visit_activity_parallel_block_stmt(IActivityBlockStmt *block) {
	std::vector<IActivityStmt *>::const_iterator it;

	for (it=block->getStmts().begin(); it!=block->getStmts().end(); it++) {
		visit_activity_stmt(*it);
	}
}

void PSIVisitor::visit_activity_repeat_stmt(IActivityRepeatStmt *repeat) {
	push_scope(repeat);
	visit_activity_stmt(repeat->getBody());
	pop_scope();
}

void PSIVisitor::visit_activity_schedule_block_stmt(IActivityBlockStmt *block) {
	std::vector<IActivityStmt *>::const_iterator it;

	for (it=block->getStmts().begin(); it!=block->getStmts().end(); it++) {
		visit_activity_stmt(*it);
	}
}

void PSIVisitor::visit_activity_if_else_stmt(IActivityIfElseStmt *stmt) {
	visit_expr(stmt->getCond());
	visit_activity_stmt(stmt->getTrue());
	if (stmt->getFalse()) {
		visit_activity_stmt(stmt->getFalse());
	}
}

void PSIVisitor::visit_activity_select_stmt(IActivityBlockStmt *s) {
	visit_activity_block_stmt(s);
}

void PSIVisitor::visit_activity_traverse_stmt(IActivityTraverseStmt *t) {
	push_scope(t);
	visit_variable_ref(t->getAction());
	if (t->getWith()) {
		visit_constraint_stmt(t->getWith());
	}
	pop_scope();
}

void PSIVisitor::visit_activity_do_action_stmt(IActivityDoActionStmt *stmt) {
	push_scope(stmt);
	visit_ref_type(dynamic_cast<IRefType *>(stmt->getTargetType()));
	if (stmt->getConstraint()) {
		visit_constraint_block(stmt->getConstraint());
	}
	pop_scope();
}

void PSIVisitor::visit_import(IImport *imp) {
	if (imp->getTargetType()) {
		visit_item(imp->getTargetType());
	}
}

void PSIVisitor::visit_import_func(IImportFunc *f) {
	// NOP
}

void PSIVisitor::visit_vendor_item(IBaseItem *it) {

}

void PSIVisitor::visit_activity_block_stmt(IActivityBlockStmt *block) {
	std::vector<IActivityStmt *>::const_iterator it;

	for (it=block->getStmts().begin(); it!=block->getStmts().end(); it++) {
		visit_activity_stmt(*it);
	}
}

void PSIVisitor::remove() {
	m_removed = true;
}

void PSIVisitor::visit_scope(IScopeItem *s) {
	ScopeItemImpl *s_i = dynamic_cast<ScopeItemImpl *>(s);
	m_removed = false;

	for (int32_t i=0; i<s->getItems().size(); i++) {
		IBaseItem *it = s->getItems().at(i);
//		IScopeItem *ss = dynamic_cast<IScopeItem *>(ss);

		m_removed = false;

		visit_item(it);

		if (m_removed) {
			fprintf(stdout, "Remove: %p %d\n", it, it->getType());
			s_i->remove(it);
			delete it;
			i--;
		}
	}

	m_removed = false;
}

void PSIVisitor::visit_item(IBaseItem *it) {
	switch (it->getType()) {
	case IBaseItem::TypePackage:
		visit_package(dynamic_cast<IPackage *>(it));
		break;

	case IBaseItem::TypeComponent:
		visit_component(dynamic_cast<IComponent *>(it));
		break;

	case IBaseItem::TypeAction:
		visit_action(dynamic_cast<IAction *>(it));
		break;

	case IBaseItem::TypeActivityStmt:
		visit_activity_stmt(dynamic_cast<IActivityStmt *>(it));
		break;

	case IBaseItem::TypeStruct:
		visit_struct(dynamic_cast<IStruct *>(it));
		break;

	case IBaseItem::TypeEnum:
		visit_enum_type(dynamic_cast<IEnumType *>(it));
		break;

	case IBaseItem::TypeExtend:
		visit_extend(dynamic_cast<IExtend *>(it));
		break;

	case IBaseItem::TypeImport:
		visit_import(dynamic_cast<IImport *>(it));
		break;

	case IBaseItem::TypeImportFunc:
		visit_import_func(dynamic_cast<IImportFunc *>(it));
		break;

	case IBaseItem::TypeBind:
		visit_bind(dynamic_cast<IBind *>(it));
		break;

	case IBaseItem::TypeConstraint:
		visit_constraint(dynamic_cast<IConstraintBlock *>(it));
		break;

	case IBaseItem::TypeCoverspec:
		visit_coverspec(dynamic_cast<ICoverspec *>(it));
		break;

	case IBaseItem::TypeField:
		visit_field(dynamic_cast<IField *>(it));
		break;

	case IBaseItem::TypeExec:
		visit_exec(dynamic_cast<IExec *>(it));
		break;

	case IBaseItem::TypeVendor:
		visit_vendor_item(it);
		break;

	case IBaseItem::TypeRefType:
		visit_ref_type(dynamic_cast<IRefType *>(it));
		break;

	case IBaseItem::TypeScalar:
		// TODO: Ignore
		break;

	case IBaseItem::TypeArray:
		// TODO: Ignore
		break;

	case IBaseItem::TypeSymbol:
		visit_symbol(dynamic_cast<ISymbol *>(it));
		break;

	default:
		fprintf(stdout, "Error: unhandled item %d\n", it->getType());
		break;
	}
}

void PSIVisitor::visit_ref_type(IRefType *ref) {

}

void PSIVisitor::visit_symbol(ISymbol *sym) {
	push_scope(sym);
	visit_activity_stmt(sym->getBody());
	pop_scope();
}

void PSIVisitor::visit_variable_ref(IVariableRef *ref) {
	if (ref->getIndexLhs()) {
		visit_expr(ref->getIndexLhs());
	}
	if (ref->getIndexRhs()) {
		visit_expr(ref->getIndexRhs());
	}
	if (ref->getNext()) {
		visit_variable_ref(ref->getNext());
	}
}

std::string PSIVisitor::type2string(IBaseItem *it) {
	std::string ret;

	while (it) {
		INamedItem *ni = dynamic_cast<INamedItem *>(it);

		if (ni) {
			if (ret.size() > 0) {
				ret.insert(0, "::");
			}
			ret.insert(0, ni->getName());
		} else {
			break;
		}

		it = it->getParent();
	}

	return ret;
}

std::string PSIVisitor::path2string(IFieldRef *f) {
	std::string ret;
	std::vector<IField *>::const_iterator it;

	for (it=f->getFieldPath().begin(); it!=f->getFieldPath().end(); ) {
		IField *field = *(it);
		ret += field->getName();
		it++;
		if (it != f->getFieldPath().end()) {
			ret += ".";
		}
	}

	return ret;
}

std::string PSIVisitor::path2string(const std::vector<IField *> &path) {
	std::string ret;
	std::vector<IField *>::const_iterator it;

	for (it=path.begin(); it!=path.end(); ) {
		IField *field = *(it);
		ret += field->getName();
		it++;
		if (it != path.end()) {
			ret += ".";
		}
	}

	return ret;
}

void PSIVisitor::push_scope(IBaseItem *scope) {
	m_scope_stack.push_back(scope);
	if (dynamic_cast<IScopeItem *>(scope)) {
		m_decl_scopes.push_back(dynamic_cast<IScopeItem *>(scope));
	}
}

const std::vector<IBaseItem *> &PSIVisitor::scopes() const {
	return m_scope_stack;
}

const std::vector<IScopeItem *> &PSIVisitor::get_decl_scopes() const {
	return m_decl_scopes;
}

IBaseItem *PSIVisitor::scope() const {
	return m_scope_stack.back();
}

IBaseItem *PSIVisitor::pop_scope() {
	IBaseItem *ret = m_scope_stack.back();
	m_scope_stack.pop_back();

	if (dynamic_cast<IScopeItem *>(ret)) {
		m_decl_scopes.pop_back();
	}

	return ret;
}

void PSIVisitor::push_graph(IActivityStmt *it) {
	m_graph_stack.push_back(it);
}

void PSIVisitor::pop_graph() {
	if (m_graph_stack.size() > 0) {
		m_graph_stack.pop_back();
	}
}

IActivityStmt *PSIVisitor::graph_parent(IActivityStmt *it) {
	if (it) {
		for (int32_t i=m_graph_stack.size()-1; i>=0; i--) {
			if (m_graph_stack.at(i) == it && i>0) {
				return m_graph_stack.at(i-1);
			}
		}
	} else {
		if (m_graph_stack.size() > 0) {
			return m_graph_stack.at(m_graph_stack.size()-1);
		}
	}
	return 0;
}

void PSIVisitor::debug(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	fprintf(stdout, "[DEBUG] ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");

	va_end(ap);
}

