/*
 * Z3Model.cpp
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#include "Z3Model.h"
#include "SolverErrorException.h"

Z3Model::Z3Model() {

	m_model = 0;
	m_cfg = Z3_mk_config();
	m_ctxt = Z3_mk_context(m_cfg);

	Z3_set_error_handler(m_ctxt, &Z3Model::z3_error_handler);

	m_solver = Z3_mk_solver(m_ctxt);
	Z3_solver_inc_ref(m_ctxt, m_solver);

}

Z3Model::~Z3Model() {
	Z3_del_config(m_cfg);
	Z3_del_context(m_ctxt);
}

bool Z3Model::solve(const std::vector<Z3ModelVar *> &vars) {
	if (m_model) {
		Z3_model_dec_ref(m_ctxt, m_model);
		m_model = 0;
	}
	// First, remove all backtracking scopes
	Z3_solver_pop(m_ctxt, m_solver,
			Z3_solver_get_num_scopes(m_ctxt, m_solver));

	Z3_lbool result = Z3_solver_check(m_ctxt, m_solver);
	if (result != Z3_L_TRUE) {
		return false;
	};

	// Push a scope into which fixed values go
	Z3_solver_push(m_ctxt, m_solver);

	// First, add values for all fixed fields
	for (std::map<std::string,Z3ModelVar *>::iterator it=m_variables.begin();
			it!=m_variables.end(); it++) {
		if (it->second->fixed()) {
			// TODO: need to deal with sign
			Z3ModelVar *var = it->second;
			Z3_ast eq = Z3_mk_eq(m_ctxt, var->var(),
					Z3_mk_int(m_ctxt,
							var->get_val(0, 0).ui, // already know we have a value
							Z3_mk_bv_sort(m_ctxt, var->bits())));
			Z3_solver_assert(m_ctxt, m_solver, eq);
		}
		it->second->invalidate(); // Ensure we query the value from the model
	}

	for (uint32_t i=0; i<100; i++) {
		Z3_solver_push(m_ctxt, m_solver);

		Z3_ast term = 0;

		uint32_t total_bits = 0;

		for (std::vector<Z3ModelVar *>::const_iterator it=vars.begin();
				it!=vars.end(); it++) {
			if (!(*it)->fixed()) {
				uint64_t coeff = m_lfsr.next();

				Z3_ast var_ast = (*it)->var();
				total_bits += (*it)->bits();
				if ((*it)->bits() < 64) {
					if ((*it)->is_signed()) {
						var_ast = Z3_mk_sign_ext(m_ctxt, (64-(*it)->bits()), var_ast);
					} else {
						var_ast = Z3_mk_zero_ext(m_ctxt, (64-(*it)->bits()), var_ast);
					}
				}

				Z3_ast this_t = Z3_mk_bvmul(m_ctxt, var_ast,
						Z3_mk_unsigned_int64(m_ctxt, coeff,
								Z3_mk_bv_sort(m_ctxt, 64)));
				if (term) {
					term = Z3_mk_bvadd(m_ctxt, term, this_t);
				} else {
					term = this_t;
				}
			}
		}

		uint32_t hash_bits = 20;
		if (total_bits == 0) {
			fprintf(stdout, "Error: no random variables\n");
		} else {
			// At least one hash bit, but no more than 20
			hash_bits = ((total_bits-1)/4)+1;

			if (hash_bits > 20) {
				hash_bits = 20;
			}
		}

//		if (total_bits < hash_bits) {
//			hash_bits = total_bits;
//		}

		if (term) {
			Z3_ast sum = Z3_mk_extract(m_ctxt, (hash_bits-1), 0, term);
			uint64_t hash = m_lfsr.next();
//		fprintf(stdout, "Hash[7:0]=0x%08x\n", (uint32_t)(hash & ((1 << hash_bits)-1)));
			Z3_ast eq = Z3_mk_eq(m_ctxt, sum,
				Z3_mk_unsigned_int64(m_ctxt, hash, Z3_mk_bv_sort(m_ctxt, hash_bits)));

			Z3_solver_assert(m_ctxt, m_solver, eq);
		}

		result = Z3_solver_check(m_ctxt, m_solver);
		if (result == Z3_L_TRUE) {
			break;
		} else {
			fprintf(stdout, "Retry\n");
			Z3_solver_pop(m_ctxt, m_solver, 1);
		}
	}

	if (result != Z3_L_TRUE) {
		fprintf(stdout, "Failed to apply bias\n");
		return false;
	}

	// Get a model of the system for later use
	m_model = Z3_solver_get_model(m_ctxt, m_solver);
	Z3_model_inc_ref(m_ctxt, m_model);

	return true;
}

bool Z3Model::check() {
	return (Z3_solver_check(m_ctxt, m_solver) == Z3_L_TRUE);
}

VarVal Z3Model::get_value(const std::string &path) {
	std::map<std::string, Z3ModelVar *>::iterator it =
			m_variables.find(path);
	VarVal ret;

	if (it != m_variables.end()) {
		Z3ModelVar *var = it->second;
		// The return is always the integer value
		ret = var->get_val(m_ctxt, m_model);

		if (ret.type == VarVal_String) {
			ret.s = m_strtab->id2str(ret.ui);
		}
	} else {
		fprintf(stdout, "Error: no variable named %s\n", path.c_str());
	}

	return ret;
}

void Z3Model::z3_error_handler(Z3_context c, Z3_error_code e) {
	throw SolverErrorException(Z3_get_error_msg(c, e));
}

