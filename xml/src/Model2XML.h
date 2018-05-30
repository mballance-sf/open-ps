/*
 * Model2XML.h
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
 * Traverses a Semantic Model description using the IModel API
 * and writes an XML file as output
 *
 *  Created on: May 26, 2018
 *      Author: ballance
 */
#pragma once
#include <string>
#include <vector>
#include <stdint.h>

#include "ModelVisitor.h"
#include "IModel.h"
#include "IBaseItem.h"
#include <ostream>



class Model2XML : public virtual ModelVisitor {
public:
	Model2XML();

	virtual ~Model2XML();

	void setUseFixedInlineAddr() { m_fixed_inline_addr = true; }

	virtual std::string convert(IModel *model);

	virtual void convert(std::ostream &out, IModel *model);


private:

	virtual void visit_package(IPackage *pkg) override;

	virtual void visit_action(IAction *a) override;

	virtual void visit_bind(IBind *b) override;

	virtual void visit_struct(IStruct *str) override;

	virtual void visit_component(IComponent *c) override;

	virtual void visit_constraint(IConstraintBlock *c) override;

//	virtual void visit_constraint_stmt(IConstraint *c) override;
//
	virtual void visit_constraint_expr_stmt(IConstraintExpr *c) override;

	virtual void visit_constraint_if_stmt(IConstraintIf *c) override;

	virtual void visit_constraint_implies_stmt(IConstraintImplies *c) override;
//
//	virtual void visit_constraint_foreach_stmt(IConstraintForeach *c) override;
//
//	virtual void visit_constraint_unique_stmt(IConstraintUnique *c) override;
//
	virtual void visit_constraint_block(IConstraintBlock *block) override;
//
//	virtual void visit_exec(IExec *e) override;
//
//	virtual void visit_exec_stmt(IExecStmt *e) override;
//
//	virtual void visit_exec_call_stmt(IMethodCallExpr *s) override;
//
//	virtual void visit_exec_expr_stmt(IExecExprStmt *s) override;
//
//	virtual void visit_exec_vendor_stmt(IExecStmt *s) override;
//
//	virtual void visit_target_template_replacement(IExecReplacementExpr *r) override;
//
//	virtual void visit_expr(IExpr *e) override;
//
//	virtual void visit_extend(IExtend *e) override;
//
//	virtual void visit_extend_composite(IExtendComposite *e) override;
//
//	virtual void visit_extend_enum(IExtendEnum *e) override;
//
	virtual void visit_binary_expr(IBinaryExpr *be) override;
//
//	virtual void visit_fieldref_expr(IFieldRef *ref) override;
//
//	virtual void visit_in_expr(IInExpr *in) override;
//
	virtual void visit_literal_expr(ILiteral *l) override;
//
//	virtual void visit_method_call(IMethodCallExpr *c) override;
//
//	virtual void visit_open_range_list(IOpenRangeList *range_l) override;
//
//	virtual void visit_open_range_value(IOpenRangeValue *range_v) override;
//
//	virtual void visit_enum_type(IEnumType *e) override;
//
	virtual void visit_field(IField *f) override;
//
//	virtual void visit_activity(IActivityStmt *activity);
//
//	virtual void visit_activity_stmt(IActivityStmt *stmt);
//
//	virtual void visit_activity_block_stmt(IActivityBlockStmt *block);
//
//	virtual void visit_activity_if_else_stmt(IActivityIfElseStmt *stmt);
//
//	virtual void visit_activity_parallel_block_stmt(IActivityBlockStmt *block);
//
//	virtual void visit_activity_repeat_stmt(IActivityRepeatStmt *repeat);
//
//	virtual void visit_activity_schedule_block_stmt(IActivityBlockStmt *s);
//
//	virtual void visit_activity_select_stmt(IActivitySelectStmt *s);
//
//	virtual void visit_activity_select_branch_stmt(IActivitySelectBranchStmt *s);
//
//	virtual void visit_activity_traverse_stmt(IActivityTraverseStmt *t);
//
//	virtual void visit_activity_do_action_stmt(IActivityDoActionStmt *stmt);
//
//	virtual void visit_import(IImport *imp);
//
//	virtual void visit_import_func(IImportFunc *f);
//
//	virtual void visit_item(IBaseItem *it);
//
//	virtual void visit_ref_type(IRefType *ref);
//
//	virtual void visit_symbol(ISymbol *s);
//
//	virtual void visit_variable_ref(IVariableRef *ref);

	void process_body(
			const std::vector<IBaseItem *>  &items,
			const std::string				&ctxt);

	void process_exec(IExec *exec);

	void process_comp_pkg_body(const std::vector<IBaseItem *> &items);

	void process_constraint_set(IConstraint *c, const char *tag=0);

//	void process_constraint_block(IConstraintBlock *block);

//	void process_expr(IExpr *e, const char *tag=0);

	void process_extend(IExtend *e);

//	void process_field(IField *f);

	void process_graph(IActivityStmt *activity);

	void process_graph_stmt(IActivityStmt *stmt, const char *tag=0);

	void process_graph_block_stmt(IActivityBlockStmt *block, const char *tag=0);

	void process_import_func(IImportFunc *f, const std::string &tag="import_function");

	static std::string type2string(IBaseItem *it);

	void type2hierarchical_id(IBaseItem *it, const std::string &tag="pss:type");

	void type2data_type(IBaseItem *it, const std::string &tag="pss:type");

	void process_fieldref(IFieldRef *ref, const std::string &tag="ref");

	void to_hierarchical_id(const std::vector<IBaseItem *> &path, const char *tag=0);

	static std::string path2string(IFieldRef *f);

	void println(const std::string &str);

	void inc_indent();
	void dec_indent();

	void enter(const std::string &str);

	void exit(const std::string &str);

	void error(const char *fmt, ...);

	static INamedItem *toNamedItem(IBaseItem *it);

private:
	std::ostream				*m_out;
	std::string					m_ind;
	uint32_t					m_ind_incr;
	bool						m_fixed_inline_addr;


};


