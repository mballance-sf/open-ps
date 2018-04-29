/*
 * Z3ModelProcessor.h
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_Z3MODELPROCESSOR_H_
#define PROCESSOR_SRC_Z3MODELPROCESSOR_H_
#include <string>
#include <vector>
#include <map>
#include "Z3ModelVar.h"
#include "Z3ExprTerm.h"
#include "PSIVisitor.h"
#include "StringTableBuilder.h"
#include "SolverErrorException.h"
#include "IVarValueProvider.h"
#include "IExecListener.h"
#include "LFSR.h"
#include "z3.h"

using namespace psi::apps;

class Z3ModelProcessor:
		public virtual PSIVisitor,
		public virtual IVarValueProvider {
public:

	Z3ModelProcessor();

	virtual ~Z3ModelProcessor();

	virtual bool build(
			IComponent 		*comp,
			IAction			*action);

	virtual bool run();

	void set_exec_listener(IExecListener *l);

	virtual void visit_field(IField *f) override;

	virtual VarVal get_value(const std::string &path);

protected:

	bool solve(const std::vector<Z3ModelVar *> &vars);

	uint32_t compute_bits(IScalarType *t);

	bool compute_sign(IScalarType *t);

	virtual void visit_binary_expr(IBinaryExpr *be) override;

	virtual void visit_constraint_expr_stmt(IConstraintExpr *c) override;

	virtual void visit_constraint_if_stmt(IConstraintIf *c) override;

	virtual void visit_literal_expr(ILiteral *l) override;

	virtual void visit_variable_ref(IVariableRef *ref) override;

	void push_prefix(const std::string &pref);

	void pop_prefix();

	const std::string &prefix();

	Z3ExprTerm upsize(const Z3ExprTerm &target, uint32_t bits);

	void compute_domain(Z3ModelVar &var);

private:

	void exec_action(
			const std::string				&context,
			IAction 						*action);

	void collect_variables(
			std::vector<Z3ModelVar *>		&vars,
			const std::string				&context,
			IAction							*action);

	void collect_rand_variables(
			std::vector<Z3ModelVar *>		&vars,
			const std::string				&context,
			IAction							*action);

	void exec_activity_stmt(
			const std::string				&context,
			IActivityStmt					*stmt);

	void exec_activity_traverse_stmt(
			const std::string				&context,
			IActivityTraverseStmt			*stmt);

	static void z3_error_handler(Z3_context c, Z3_error_code e);

private:
	IAction									*m_root_action;
	IComponent								*m_root_comp;
	StringTableBuilder						m_strtab;
	Z3_config								m_cfg;
	Z3_context								m_ctxt;
	Z3_model								m_model;
	Z3_solver								m_solver;
	std::map<std::string, Z3ModelVar *>		m_variables;
	std::vector<std::string>				m_prefix_v;
	std::string								m_prefix;
	bool									m_prefix_valid;
	LFSR									m_lfsr;
	Z3ExprTerm								m_expr;
	std::vector<Z3_ast>						m_if_else_conds;
	uint32_t								m_expr_depth;
	IExecListener							*m_exec_listener;

};

#endif /* PROCESSOR_SRC_Z3MODELPROCESSOR_H_ */
