/*
 * PSI2Rules.h
 *
 *  Created on: Aug 12, 2016
 *      Author: ballance
 */

#ifndef INCLUDED_PSI2RULES_H
#define INCLUDED_PSI2RULES_H
#include "RulesPSIVisitor.h"
#include "IModelProcessor.h"
#include "FormattedFileOutput.h"

using namespace psi::apps;
using namespace psi_api;

namespace qpssc {

class PSI2Rules :
		public RulesPSIVisitor,
		public virtual IModelProcessor {
public:

	PSI2Rules(const std::string &outdir);

	virtual ~PSI2Rules();

	virtual bool process(IModel *model);

protected:
	virtual void visit_model(IModel *model);

	virtual void visit_package(IPackage *pkg);

	virtual void visit_action(IAction *a);

	virtual void visit_constraint(IConstraintBlock *c);

	virtual void visit_constraint_expr_stmt(IConstraintExpr *c);

	virtual void visit_constraint_if_stmt(IConstraintIf *c);

	virtual void visit_struct(IStruct *str);

	virtual void visit_binary_expr(IBinaryExpr *be);

	virtual void visit_fieldref_expr(IFieldRef *ref);

	virtual void visit_literal_expr(ILiteral *l);

	virtual void visit_exec(IExec *e);

	virtual void visit_extend(IExtend *e);

	virtual void visit_field(IField *f);

	virtual void visit_graph(IActivityStmt *graph);

	virtual void visit_graph_parallel_block_stmt(IActivityBlockStmt *block);

	virtual void visit_graph_select_stmt(IActivityBlockStmt *s);

	virtual void visit_graph_repeat_stmt(IActivityRepeatStmt *r);

	virtual void visit_activity_traverse_stmt(IActivityTraverseStmt *t);

	virtual void visit_rules_action_exec_item(RulesExecActionItem *it);

private:

	void println(const std::string &s);

	static std::string binop_tostring(IBinaryExpr::BinOpType t);

	static std::string fieldref2string(IFieldRef *r);

private:
	FormattedFileOutput		*m_out;
	std::string				m_outdir;
	std::string				m_rules_name;
	IComponent				*m_root_c;
	IAction					*m_root_a;
	IAction					*m_active_a;
	uint32_t				m_anon_constraint_id;

//	std::string type2rulestype(IBaseItem *type);


};

} /* namespace qpss */

#endif /* SRC_PSI2RULES_H_ */
