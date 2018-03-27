/*
 * Z3ModelProcessor.cpp
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#include "Z3ModelProcessor.h"


Z3ModelProcessor::Z3ModelProcessor() : m_cfg(0), m_ctxt(0) {
	m_prefix_valid = false;

}

Z3ModelProcessor::~Z3ModelProcessor() {
	// TODO Auto-generated destructor stub
}

bool Z3ModelProcessor::process(IComponent *comp, IAction *action) {
	m_cfg = Z3_mk_config();
	m_ctxt = Z3_mk_context(m_cfg);

	m_solver = Z3_mk_solver(m_ctxt);
	Z3_solver_inc_ref(m_ctxt, m_solver);

	push_prefix(action->getName());
	visit_action(action);
	pop_prefix();

//	for (uint32_t i=1; i<m_variables.size(); i++) {
//		Z3_solver_assert(m_ctxt,
//				m_solver,
//				Z3_mk_bvult(m_ctxt,
//						m_variables.at(i-1),
//						m_variables.at(i))
//		);
//	}

	Z3_lbool result = Z3_solver_check(m_ctxt, m_solver);
	if (result == Z3_L_FALSE) {
		fprintf(stdout, "Result: unsat\n");
	} else if (result == Z3_L_UNDEF) {
		fprintf(stdout, "Result: unknown\n");
	} else if (result == Z3_L_TRUE) {
		fprintf(stdout, "Result: sat\n");
	}
	Z3_model m = Z3_solver_get_model(m_ctxt, m_solver);
	if (m) {
//		Z3_model_eval(m_ctxt, m, t, true, v)
		Z3_model_inc_ref(m_ctxt, m);
		fprintf(stdout, "  Model:\n%s\n", Z3_model_to_string(m_ctxt, m));
//		for (uint32_t i=0; i<m_variables.size(); i++) {
//			Z3_ast v;
//			Z3_model_eval(m_ctxt, m,
//					m_variables.at(i),
//					true, &v);
//			fprintf(stdout, "Result kind: %d\n", Z3_get_ast_kind(m_ctxt, v));
//			fprintf(stdout, "  Value: %s\n",
//					Z3_get_numeral_string(m_ctxt, v));
//		}

		for (uint32_t i=0; i<Z3_model_get_num_consts(m_ctxt, m); i++) {
			Z3_func_decl cnst = Z3_model_get_const_decl(m_ctxt, m, i);
			Z3_symbol name = Z3_get_decl_name(m_ctxt, cnst);
			fprintf(stdout, "Symbol: %s\n",
					Z3_get_symbol_string(m_ctxt, name));

		}
//		for (std::vector<Z3_ast>::const_iterator it=m_variables.begin();
//				it!=m_variables.end(); it++) {
//			Z3_symbol name = Z3_get_decl_name(m_ctxt, (Z3_func_decl)*it);
//			fprintf(stdout, "Symbol: %s\n",
//					Z3_get_symbol_string(m_ctxt, name));
//		}
		Z3_model_dec_ref(m_ctxt, m);
	}
	Z3_solver_dec_ref(m_ctxt, m_solver);

	return true;
}

void Z3ModelProcessor::visit_field(IField *f) {
	fprintf(stdout, "visit_field: %s\n", f->getName().c_str());

	if (f->getDataType()->getType() == IBaseItem::TypeScalar) {
		std::string id = prefix() + "." + f->getName();
		uint32_t bits = 32;

		fprintf(stdout, "Note: scalar variable %s\n", id.c_str());

		Z3_ast var = Z3_mk_const(m_ctxt,
				Z3_mk_string_symbol(m_ctxt, id.c_str()),
				Z3_mk_bv_sort(m_ctxt, bits));
		m_variables[id] = new Z3ModelVar(id, var);

	} else if (f->getDataType()->getType() == IBaseItem::TypeArray) {

	} else {
		// composite data type?
		fprintf(stdout, "Error: unknown data type\n");
	}

}

void Z3ModelProcessor::push_prefix(const std::string &pref) {
	m_prefix_v.push_back(pref);
	m_prefix_valid = false;
}

void Z3ModelProcessor::pop_prefix() {
	m_prefix_v.pop_back();
	m_prefix_valid = false;
}

const std::string &Z3ModelProcessor::prefix() {

	if (!m_prefix_valid) {
		m_prefix.clear();
		for (uint32_t i=0; i<m_prefix_v.size(); i++) {
			m_prefix.append(m_prefix_v.at(i));
			if (i+1 < m_prefix_v.size()) {
				m_prefix.append(".");
			}
		}
		m_prefix_valid = true;
	}

	return m_prefix;
}
