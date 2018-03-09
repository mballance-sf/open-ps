/*
 * ExecTraceGenerator.h
 *
 *  Created on: Jan 3, 2017
 *      Author: ballance
 */

#ifndef SRC_EXECTRACEGENERATOR_H_
#define SRC_EXECTRACEGENERATOR_H_

#include "IModelProcessor.h"
#include "RulesPSIVisitor.h"
#include "Engine.h"
#include <map>
#include <string>
#include <vector>

namespace qpssc {

using namespace qpsrt;

class ExecTraceGenerator:
		public qpssc::IModelProcessor,
		public virtual RulesPSIVisitor {
	friend class ThreadContextCreate;
	friend class ThreadContextJoin;
	friend class ThreadBegin;
	friend class ThreadEnd;
public:
	ExecTraceGenerator(const std::string &outdir);

	virtual ~ExecTraceGenerator();

	virtual bool process(IModel *model);

	virtual void visit_field(IField *f);

	virtual void visit_exec(IExec *e);

	virtual void visit_extend(IExtend *e);

	virtual void visit_rules_action_exec_item(RulesExecActionItem *it);

	virtual void visit_import_func(IImportFunc *f);

public:

	virtual void repeat_begin();

	virtual void repeat_end();

	virtual IExecCallback *create_import_func_cb(IImportFunc *f);

	// Creates an exec callback for target-template exec blocks
	virtual IExecCallback *create_template_exec_cb(IExec *e);

protected:

	virtual void enter_parallel();

	virtual void start_thread();

	virtual void end_thread();

	virtual void join_parallel();

	void set_pass(uint32_t p) { m_pass = p; }

	uint32_t get_pass() const { return m_pass; }

private:

	void do_exec(IExec *e, const std::string &ctxt);

	void exec_stmt(IBaseItem *scope, IExecStmt *s, const std::string &ctxt);

	uint64_t eval_expr_u(IBaseItem *scope, const std::string &ctxt, IExpr *e);
	int64_t eval_expr_s(IBaseItem *scope, const std::string &ctxt, IExpr *e);

	void debug(const char *fmt, ...);
	void note(const char *fmt, ...);
	void warn(const char *fmt, ...);

	static std::string list2path(const std::vector<std::string> &l);

private:
	std::string									m_outdir;
	IModel										*m_model;
	Engine										*m_engine;
	std::vector<std::string>					m_exec_list;
	std::vector<std::string>					m_exec_path;
	std::map<uint32_t,IExec *>					m_id_exec_map;
	uint32_t									m_pass;

};

}

#endif /* SRC_EXECTRACEGENERATOR_H_ */
