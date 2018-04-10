/*
 * PSIProcessor.h
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

#ifndef SRC_APPS_PSIVISITOR_H_
#define SRC_APPS_PSIVISITOR_H_
#include "IModel.h"
#include "IBaseItem.h"
#include <stack>

using namespace psi_api;

namespace psi {
namespace apps {

class PSIVisitor {

public:

	PSIVisitor();

	virtual ~PSIVisitor();

	virtual void visit_model(IModel *model);

protected:

	virtual void visit_package(IPackage *pkg);

	virtual void visit_action(IAction *a);

	virtual void visit_bind(IBind *b);

	virtual void visit_struct(IStruct *str);

	virtual void visit_component(IComponent *c);

	virtual void visit_constraint(IConstraintBlock *c);

	virtual void visit_constraint_stmt(IConstraint *c);

	virtual void visit_constraint_expr_stmt(IConstraintExpr *c);

	virtual void visit_constraint_if_stmt(IConstraintIf *c);

	virtual void visit_constraint_block(IConstraintBlock *block);

	virtual void visit_coverspec(ICoverspec *coverspec);

	virtual void visit_exec(IExec *e);

	virtual void visit_exec_stmt(IExecStmt *e);

	virtual void visit_exec_call_stmt(IMethodCallExpr *s);

	virtual void visit_exec_expr_stmt(IExecExprStmt *s);

	virtual void visit_exec_vendor_stmt(IExecStmt *s);

	virtual void visit_expr(IExpr *e);

	virtual void visit_extend(IExtend *e);

	virtual void visit_extend_composite(IExtendComposite *e);

	virtual void visit_extend_enum(IExtendEnum *e);

	virtual void visit_binary_expr(IBinaryExpr *be);

	virtual void visit_fieldref_expr(IFieldRef *ref);

	virtual void visit_in_expr(IInExpr *in);

	virtual void visit_literal_expr(ILiteral *l);

	virtual void visit_method_call(IMethodCallExpr *c);

	virtual void visit_open_range_list(IOpenRangeList *range_l);

	virtual void visit_open_range_value(IOpenRangeValue *range_v);

	virtual void visit_enum_type(IEnumType *e);

	virtual void visit_field(IField *f);

	virtual void visit_activity(IActivityStmt *activity);

	virtual void visit_activity_stmt(IActivityStmt *stmt);

	virtual void visit_activity_block_stmt(IActivityBlockStmt *block);

	virtual void visit_activity_if_else_stmt(IActivityIfElseStmt *stmt);

	virtual void visit_activity_parallel_block_stmt(IActivityBlockStmt *block);

	virtual void visit_activity_repeat_stmt(IActivityRepeatStmt *repeat);

	virtual void visit_activity_schedule_block_stmt(IActivityBlockStmt *s);

	virtual void visit_activity_select_stmt(IActivityBlockStmt *s);

	virtual void visit_activity_traverse_stmt(IActivityTraverseStmt *t);

	virtual void visit_activity_do_action_stmt(IActivityDoActionStmt *stmt);

	virtual void visit_import(IImport *imp);

	virtual void visit_import_func(IImportFunc *f);

	virtual void visit_item(IBaseItem *it);

	virtual void visit_ref_type(IRefType *ref);

	virtual void visit_symbol(ISymbol *s);

	virtual void visit_variable_ref(IVariableRef *ref);

	virtual void visit_vendor_item(IBaseItem *it);

	virtual void remove();

	virtual void visit_scope(IScopeItem *s);

	virtual void push_scope(IBaseItem *scope);

	virtual IBaseItem *scope() const;

	virtual const std::vector<IBaseItem *> &scopes() const;

	virtual const std::vector<IScopeItem *> &get_decl_scopes() const;

	virtual IBaseItem *pop_scope();

	static std::string type2string(IBaseItem *it);

	static std::string path2string(IFieldRef *f);

	static std::string path2string(const std::vector<IField *> &path);

	void push_graph(IActivityStmt *it);

	void pop_graph();

	IActivityStmt *graph_parent(IActivityStmt *it=0);

private:
	void enter(const char *fmt, ...);

	void leave(const char *fmt, ...);

private:
	bool							m_debug;
	bool							m_removed;
	std::vector<IBaseItem *>		m_scope_stack;
	std::vector<IScopeItem *>		m_decl_scopes;
	std::vector<IActivityStmt *>	m_graph_stack;

};

} /* namespace apps */
} /* namespace psi */

#endif /* SRC_APPS_PSIVISITOR_H_ */
