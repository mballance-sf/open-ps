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

bool Z3ModelProcessor::build(IComponent *comp, IAction *action) {

	m_strtab.build(comp);
	m_strtab.build(action);

	m_cfg = Z3_mk_config();
	m_ctxt = Z3_mk_context(m_cfg);

	m_solver = Z3_mk_solver(m_ctxt);
	Z3_solver_inc_ref(m_ctxt, m_solver);

	push_prefix(action->getName());
	visit_action(action);
	pop_prefix();

	std::vector<Z3ModelVar *> vars;
	for (std::map<std::string,Z3ModelVar *>::iterator it=m_variables.begin();
			it!=m_variables.end(); it++) {
		vars.push_back(it->second);
	}

//	for (uint32_t i=1; i<vars.size(); i++) {
//		Z3_solver_assert(m_ctxt,
//				m_solver,
//				Z3_mk_bvult(m_ctxt,
//						vars.at(i-1)->var(),
//						vars.at(i)->var())
//		);
//	}

//	Z3_lbool result = Z3_solver_check(m_ctxt, m_solver);
//	if (result == Z3_L_FALSE) {
//		fprintf(stdout, "Result: unsat\n");
//	} else if (result == Z3_L_UNDEF) {
//		fprintf(stdout, "Result: unknown\n");
//	} else if (result == Z3_L_TRUE) {
//		fprintf(stdout, "Result: sat\n");
//	}
//	Z3_model m = Z3_solver_get_model(m_ctxt, m_solver);
//	if (m) {
////		Z3_model_eval(m_ctxt, m, t, true, v)
//		Z3_model_inc_ref(m_ctxt, m);
//		fprintf(stdout, "  Model:\n%s\n", Z3_model_to_string(m_ctxt, m));
//		for (uint32_t i=0; i<m_variables.size(); i++) {
//			Z3_ast v;
//			Z3_model_eval(m_ctxt, m,
//					m_variables.at(i),
//					true, &v);
//			fprintf(stdout, "Result kind: %d\n", Z3_get_ast_kind(m_ctxt, v));
//			fprintf(stdout, "  Value: %s\n",
//					Z3_get_numeral_string(m_ctxt, v));
//		}

//		for (uint32_t i=0; i<Z3_model_get_num_consts(m_ctxt, m); i++) {
//			Z3_func_decl cnst = Z3_model_get_const_decl(m_ctxt, m, i);
//			Z3_symbol name = Z3_get_decl_name(m_ctxt, cnst);
//			fprintf(stdout, "Symbol: %s\n",
//					Z3_get_symbol_string(m_ctxt, name));
//
//		}
//		for (std::vector<Z3_ast>::const_iterator it=m_variables.begin();
//				it!=m_variables.end(); it++) {
//			Z3_symbol name = Z3_get_decl_name(m_ctxt, (Z3_func_decl)*it);
//			fprintf(stdout, "Symbol: %s\n",
//					Z3_get_symbol_string(m_ctxt, name));
//		}
//		Z3_model_dec_ref(m_ctxt, m);
//	}
//	Z3_solver_dec_ref(m_ctxt, m_solver);

	return true;
}

bool Z3ModelProcessor::run() {
	bool ret = false;

	Z3_lbool result = Z3_solver_check(m_ctxt, m_solver);
	if (result != Z3_L_TRUE) {
		fprintf(stdout, "Error: system doesn't solve\n");
		return false;
	}


	std::vector<Z3ModelVar *> vars;
	for (std::map<std::string,Z3ModelVar *>::iterator it=m_variables.begin();
			it!=m_variables.end(); it++) {
		vars.push_back(it->second);
	}

	apply_bias(vars);

	Z3_model m = Z3_solver_get_model(m_ctxt, m_solver);
	if (m) {
//		Z3_model_eval(m_ctxt, m, t, true, v)
		Z3_model_inc_ref(m_ctxt, m);
		fprintf(stdout, "  Model:\n%s\n", Z3_model_to_string(m_ctxt, m));
		Z3_model_dec_ref(m_ctxt, m);
	}

	// apply_bias pushes a context
	Z3_solver_pop(m_ctxt, m_solver, 1);

	return true;
}

void Z3ModelProcessor::visit_field(IField *f) {
	fprintf(stdout, "visit_field: %s\n", f->getName().c_str());

	if (f->getDataType()->getType() == IBaseItem::TypeScalar) {
		std::string id = prefix() + "." + f->getName();
		uint32_t bits = compute_bits(
				dynamic_cast<IScalarType *>(f->getDataType()));

		fprintf(stdout, "Note: scalar variable %s\n", id.c_str());

		Z3_ast var = Z3_mk_const(m_ctxt,
				Z3_mk_string_symbol(m_ctxt, id.c_str()),
				Z3_mk_bv_sort(m_ctxt, bits));
		m_variables[id] = new Z3ModelVar(id, var, bits);

	} else if (f->getDataType()->getType() == IBaseItem::TypeArray) {

	} else if (f->getDataType()->getType() == IBaseItem::TypeRefType) {
		IRefType *ref = dynamic_cast<IRefType *>(f->getDataType());
		push_prefix(f->getName());
		switch (ref->getTargetType()->getType()) {
		case IBaseItem::TypeStruct:
			visit_struct(dynamic_cast<IStruct *>(ref->getTargetType()));
			break;
		case IBaseItem::TypeAction:
			visit_action(dynamic_cast<IAction *>(ref->getTargetType()));
			break;
		default:
			fprintf(stdout, "Error: unknown composite type\n");
		}
		pop_prefix();
	} else {
		// composite data type?
		fprintf(stdout, "Error: unknown data type\n");
	}
}

void Z3ModelProcessor::apply_bias(const std::vector<Z3ModelVar *> &vars) {
	Z3_lbool result;

	Z3_solver_push(m_ctxt, m_solver);
	for (uint32_t i=0; i<100; i++) {

		// TODO: For each variable, select a bit to force
		for (uint32_t j=0; j<vars.size(); j++) {
			if (vars.at(j)->bits() < 4) {
				continue;
			}
			uint32_t bits = vars.at(j)->bits();
			uint32_t num_sel_bits = (bits/4)?(bits/4):1; // always need to set at least one bit
			uint64_t mask = 0;
			for (uint32_t k=0; k<num_sel_bits; k++) {
				uint64_t rv = m_lfsr.next();
				uint32_t bit = ((rv >> 10) % bits);
				uint32_t val = (rv & (1 << bit))?1:0;
				if (mask & (1 << bit)) {
					k--;
					continue;
				}
				mask |= (1 << bit);

				fprintf(stdout, "%s: Set bit %d to %d (0x%08llx)\n",
						vars.at(j)->name().c_str(),
						bit, val, rv);

				Z3_solver_assert(m_ctxt, m_solver,
					Z3_mk_eq(m_ctxt,
							Z3_mk_extract(m_ctxt, bit, bit, vars.at(j)->var()),
							(val)?
									Z3_mk_numeral(m_ctxt, "1", Z3_mk_bv_sort(m_ctxt, 1)):
									Z3_mk_numeral(m_ctxt, "0", Z3_mk_bv_sort(m_ctxt, 1))
					)
				);
			}
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
	}
}

uint32_t Z3ModelProcessor::compute_bits(IScalarType *t) {
	uint32_t ret = 32;

	switch (t->getScalarType()) {
		case IScalarType::ScalarType_Bool:
			ret = 1;
			break;
		case IScalarType::ScalarType_Bit: {
			if (!t->getLSB() && !t->getMSB()) {
				ret = 1;
			} else if (t->getLSB() && t->getMSB()) {
				fprintf(stdout, "TODO: MSB:LSB bit-width\n");
			} else {
				// Assume bit-width is specified
				if (t->getLSB()->getType() == IExpr::ExprType_Literal) {
					ILiteral *l = dynamic_cast<ILiteral *>(t->getLSB());
					ret = l->getBit();
				} else {
					fprintf(stdout, "Error: unsupported bit-width\n");
				}
			}
		} break;
		case IScalarType::ScalarType_Int: {
			if (!t->getLSB() && !t->getMSB()) {
				ret = 32;
			} else if (t->getLSB() && t->getMSB()) {
				fprintf(stdout, "TODO: MSB:LSB bit-width\n");
			} else {
				// Assume bit-width is specified
				if (t->getLSB()->getType() == IExpr::ExprType_Literal) {
					ILiteral *l = dynamic_cast<ILiteral *>(t->getLSB());
					ret = l->getBit();
				} else {
					fprintf(stdout, "Error: unsupported bit-width\n");
				}
			}
		} break;

		case IScalarType::ScalarType_String: {
			ret = m_strtab.bits();
			fprintf(stdout, "String bits: %d\n", ret);
		} break;

		default:
			fprintf(stdout, "Error: unhandled compute-bits case %d\n",
					t->getScalarType());
			break;
	}

	return ret;
}

void Z3ModelProcessor::visit_binary_expr(IBinaryExpr *be) {
	visit_expr(be->getLHS());
	Z3_ast lhs = m_expr;
	visit_expr(be->getRHS());
	Z3_ast rhs = m_expr;

	switch (be->getBinOpType()) {
	case IBinaryExpr::BinOp_And:
		m_expr = Z3_mk_bvand(m_ctxt, lhs, rhs);
		break;
	case IBinaryExpr::BinOp_AndAnd:
		// TODO:
		m_expr = Z3_mk_bvand(m_ctxt, lhs, rhs);
		break;
	case IBinaryExpr::BinOp_ArrayRef:
		break;
	case IBinaryExpr::BinOp_Divide:
		// TODO: figure out signing
		m_expr = Z3_mk_bvudiv(m_ctxt, lhs, rhs);
		break;
	case IBinaryExpr::BinOp_Eq:
		fprintf(stdout, "TODO: '=' not supported\n");
		break;
	case IBinaryExpr::BinOp_EqEq:
		m_expr = Z3_mk_eq(m_ctxt, lhs, rhs);
		break;
	case IBinaryExpr::BinOp_GE:
		m_expr = Z3_mk_bvsge(m_ctxt, lhs, rhs);
		break;
	case IBinaryExpr::BinOp_GT:
		m_expr = Z3_mk_bvsgt(m_ctxt, lhs, rhs);
		break;
	case IBinaryExpr::BinOp_LE:
		m_expr = Z3_mk_bvsle(m_ctxt, lhs, rhs);
		break;
	case IBinaryExpr::BinOp_LT:
		m_expr = Z3_mk_bvslt(m_ctxt, lhs, rhs);
		break;
	default:
		fprintf(stdout, "Error: unhandled binary expr %d\n",
				be->getBinOpType());
	}
}

void Z3ModelProcessor::visit_constraint_expr_stmt(IConstraintExpr *c) {
	m_expr = 0;
	visit_expr(c->getExpr());
	Z3_solver_assert(m_ctxt, m_solver, m_expr);

	fprintf(stdout, "constraint statement: %p\n", m_expr);
}


void Z3ModelProcessor::visit_literal_expr(ILiteral *l) {
	switch (l->getLiteralType()) {
	case ILiteral::LiteralString:
		m_expr = Z3_mk_int(m_ctxt,
				m_strtab.str2id(l->getString()),
				Z3_mk_bv_sort(m_ctxt, m_strtab.bits()));
		break;
	case ILiteral::LiteralBool:
		m_expr = Z3_mk_int(m_ctxt,
				l->getBool(),
				Z3_mk_bv_sort(m_ctxt, 2));
		break;
	case ILiteral::LiteralInt:
		m_expr = Z3_mk_int(m_ctxt,
				l->getInt(),
				Z3_mk_bv_sort(m_ctxt, 32));
		break;
	case ILiteral::LiteralBit:
		m_expr = Z3_mk_int(m_ctxt,
				l->getBit(),
				Z3_mk_bv_sort(m_ctxt, 32));
		break;
	default:
		fprintf(stdout, "Error: unhandled literal case %d\n",
				l->getLiteralType());
		break;

	}
}

void Z3ModelProcessor::visit_variable_ref(IVariableRef *ref) {
	// Determine the full name of the variable
	std::string name = ref->toString();
	fprintf(stdout, "Ref: %s prefix: %s\n",
			name.c_str(), prefix().c_str());
	std::string full_name = prefix() + "." + name;

	std::map<std::string, Z3ModelVar *>::iterator v =
			m_variables.find(full_name);

	m_expr = v->second->var();
	fprintf(stdout, "m_expr=%p\n", m_expr);
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
