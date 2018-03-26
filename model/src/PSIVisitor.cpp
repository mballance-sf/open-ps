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

using namespace psi;

namespace psi {
namespace apps {

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

	if (a->getGraph()) {
		visit_graph(a->getGraph());
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
	for (std::vector<IExecStmt *>::const_iterator it=e->getStmts().begin();
			it!=e->getStmts().end(); it++) {
		visit_exec_stmt(*it);
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

void PSIVisitor::visit_expr(IExpr *e) {
	if (!e) {
		fprintf(stdout, "Error: null expression\n");
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
		default:
			fprintf(stdout, "Error: unhandled expr type %d\n", e->getType());
			break;
	}
}

void PSIVisitor::visit_extend(IExtend *e) {
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

void PSIVisitor::visit_enum_type(IEnumType *e) {

}

void PSIVisitor::visit_literal_expr(ILiteral *l) {

}

void PSIVisitor::visit_method_call(IMethodCallExpr *c) {

}

void PSIVisitor::visit_field(IField *f) {
	visit_item(f->getDataType());
}

void PSIVisitor::visit_graph(IGraphStmt *activity) {
	if (activity->getStmtType() == IGraphStmt::GraphStmt_Block) {
		IGraphBlockStmt *b = dynamic_cast<IGraphBlockStmt *>(activity);
		push_graph(b);
		for (std::vector<IGraphStmt *>::const_iterator it=b->getStmts().begin();
				it!=b->getStmts().end(); it++) {
			visit_graph_stmt(*it);
		}
		pop_graph();
	} else {
		visit_graph_stmt(activity);
	}
}

void PSIVisitor::visit_graph_stmt(IGraphStmt *stmt) {
	push_graph(stmt);

	switch (stmt->getStmtType()) {
	case IGraphStmt::GraphStmt_Block: {
		visit_graph_block_stmt(dynamic_cast<IGraphBlockStmt *>(stmt));
	} break;

	case IGraphStmt::GraphStmt_IfElse: {
		fprintf(stdout, "TODO: GraphStmt_IfElse\n");
	} break;

	case IGraphStmt::GraphStmt_Parallel: {
		visit_graph_parallel_block_stmt(dynamic_cast<IGraphBlockStmt *>(stmt));
	} break;

	case IGraphStmt::GraphStmt_Schedule: {
		visit_graph_schedule_block_stmt(dynamic_cast<IGraphBlockStmt *>(stmt));
	} break;

	case IGraphStmt::GraphStmt_Select: {
		visit_graph_select_stmt(dynamic_cast<IGraphBlockStmt *>(stmt));
	} break;

	case IGraphStmt::GraphStmt_Repeat: {
		visit_graph_repeat_stmt(dynamic_cast<IGraphRepeatStmt *>(stmt));
	} break;

	case IGraphStmt::GraphStmt_Traverse: {
		visit_activity_traverse_stmt(dynamic_cast<IActivityTraverseStmt *>(stmt));
	} break;

	case IGraphStmt::GraphStmt_DoAction: {
		visit_activity_do_action_stmt(dynamic_cast<IActivityDoActionStmt *>(stmt));
	} break;

	default: fprintf(stdout, "TODO: handle activity stmt %d\n", stmt->getStmtType());

	}

	pop_graph();
}

void PSIVisitor::visit_graph_parallel_block_stmt(IGraphBlockStmt *block) {
	std::vector<IGraphStmt *>::const_iterator it;

	for (it=block->getStmts().begin(); it!=block->getStmts().end(); it++) {
		visit_graph_stmt(*it);
	}
}

void PSIVisitor::visit_graph_repeat_stmt(IGraphRepeatStmt *repeat) {
	visit_graph_stmt(repeat->getBody());
}

void PSIVisitor::visit_graph_schedule_block_stmt(IGraphBlockStmt *block) {
	std::vector<IGraphStmt *>::const_iterator it;

	for (it=block->getStmts().begin(); it!=block->getStmts().end(); it++) {
		visit_graph_stmt(*it);
	}
}

void PSIVisitor::visit_graph_select_stmt(IGraphBlockStmt *s) {
	visit_graph_block_stmt(s);
}

void PSIVisitor::visit_activity_traverse_stmt(IActivityTraverseStmt *t) {
	visit_variable_ref(t->getAction());
	if (t->getWith()) {
		visit_constraint_stmt(t->getWith());
	}
}

void PSIVisitor::visit_activity_do_action_stmt(IActivityDoActionStmt *stmt) {
	visit_ref_type(dynamic_cast<IRefType *>(stmt->getTargetType()));
	if (stmt->getConstraint()) {
		visit_constraint_block(stmt->getConstraint());
	}
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

void PSIVisitor::visit_graph_block_stmt(IGraphBlockStmt *block) {
	std::vector<IGraphStmt *>::const_iterator it;

	for (it=block->getStmts().begin(); it!=block->getStmts().end(); it++) {
		visit_graph_stmt(*it);
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
	visit_graph_stmt(sym->getBody());
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

void PSIVisitor::push_scope(IScopeItem *scope) {
	m_scope_stack.push_back(scope);
}

const std::vector<IScopeItem *> &PSIVisitor::scopes() const {
	return m_scope_stack;
}

IScopeItem *PSIVisitor::scope() const {
	return m_scope_stack.back();
}

IScopeItem *PSIVisitor::pop_scope() {
	IScopeItem *ret = m_scope_stack.back();
	m_scope_stack.pop_back();

	return ret;
}

void PSIVisitor::push_graph(IGraphStmt *it) {
	m_graph_stack.push_back(it);
}

void PSIVisitor::pop_graph() {
	if (m_graph_stack.size() > 0) {
		m_graph_stack.pop_back();
	}
}

IGraphStmt *PSIVisitor::graph_parent(IGraphStmt *it) {
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

} /* namespace apps */
} /* namespace psi */
