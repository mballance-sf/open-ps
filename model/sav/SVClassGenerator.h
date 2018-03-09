/*
 * SVClassGenerator.h
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#ifndef SRC_SVCLASSGENERATOR_H_
#define SRC_SVCLASSGENERATOR_H_
#include <string>
#include <stack>
#include "IModelProcessor.h"
#include "IFormattedOutput.h"
#include "RulesPSIVisitor.h"

using namespace psi::apps;

namespace qpssc {

class SVClassGenerator:
		public IModelProcessor,
		public virtual RulesPSIVisitor {
public:
	SVClassGenerator(const std::string &outdir);

	virtual ~SVClassGenerator();

	virtual bool process(psi_api::IModel *model);

	virtual void visit_action(IAction *a);

	virtual void visit_struct(IStruct *str);

	virtual void visit_exec(IExec *e);

	virtual void visit_exec_call_stmt(IMethodCallExpr *s);

	virtual void visit_exec_expr_stmt(IExecExprStmt *s);

	virtual void visit_rules_exec_if(RulesExecStmtIfItem *s);

	virtual void visit_field(IField *f);

	// Expressions
	virtual void visit_expr(IExpr *expr);

	virtual void visit_binary_expr(IBinaryExpr *be);

	virtual void visit_fieldref_expr(IFieldRef *ref);

	virtual void visit_literal_expr(ILiteral *l);

	virtual void visit_import_func(IImportFunc *f);

	virtual void visit_rules_action_exec_item(RulesExecActionItem *it);

	void generate_builtins();

	void generate_run(const std::string &rules_name);

	void generate_registeraut(const std::string &rules_name);

private:
	enum ProcessPass {
		ProcessPass_ExecMethods,
		ProcessPass_IfMethods,
		ProcessPass_ExecMapping
	};

	std::string type2string(IBaseItem *t);

	std::string fieldref2string(IFieldRef *r);

	static std::string list2path(const std::vector<std::string> &l);

private:
	ProcessPass					m_pass;
	bool						m_emit_expr;
	std::string					m_outdir;
	IFormattedOutput			*m_out;
	// Path to a specific exec
	std::vector<std::string>	m_exec_path;
	// Type-based list of execs
	std::vector<std::string>	m_exec_list;

	IAction						*m_root_action;

};

} /* namespace qpssc */

#endif /* SRC_SVCLASSGENERATOR_H_ */
