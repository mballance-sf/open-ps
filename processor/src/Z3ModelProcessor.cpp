/*
 * Z3ModelProcessor.cpp
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#include "Z3ModelProcessor.h"
#include <limits.h>


Z3ModelProcessor::Z3ModelProcessor() : m_cfg(0), m_ctxt(0), m_hash(0) {
	m_prefix_valid = false;
	m_expr_depth = 0;

}

Z3ModelProcessor::~Z3ModelProcessor() {
	// TODO Auto-generated destructor stub
}

bool Z3ModelProcessor::build(IComponent *comp, IAction *action) {

	m_strtab.build(comp);
	m_strtab.build(action);

	m_cfg = Z3_mk_config();
	m_ctxt = Z3_mk_context(m_cfg);

	Z3_set_error_handler(m_ctxt, &Z3ModelProcessor::z3_error_handler);

	m_solver = Z3_mk_solver(m_ctxt);
	Z3_solver_inc_ref(m_ctxt, m_solver);

	m_hash = Z3_mk_const(m_ctxt,
			Z3_mk_string_symbol(m_ctxt, "__hash"),
			Z3_mk_bv_sort(m_ctxt, 4));

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
		it->second->reset();
		vars.push_back(it->second);
	}

	apply_bias(vars);

	Z3_model m = Z3_solver_get_model(m_ctxt, m_solver);
	if (m) {
//		Z3_model_eval(m_ctxt, m, t, true, v)
		Z3_model_inc_ref(m_ctxt, m);
//		fprintf(stdout, "  Model:\n%s\n", Z3_model_to_string(m_ctxt, m));

		for (std::map<std::string, Z3ModelVar *>::iterator it=m_variables.begin();
				it!=m_variables.end(); it++) {
			Z3ModelVar *v = it->second;
			fprintf(stdout, "%s: 0x%08llx\n",
					v->name().c_str(), v->get_val(m_ctxt, m));
		}
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
		bool is_signed = compute_sign(
				dynamic_cast<IScalarType *>(f->getDataType()));

		fprintf(stdout, "Note: scalar variable %s\n", id.c_str());

		Z3_ast var = Z3_mk_const(m_ctxt,
				Z3_mk_string_symbol(m_ctxt, id.c_str()),
				Z3_mk_bv_sort(m_ctxt, bits));
		m_variables[id] = new Z3ModelVar(id, var, bits, is_signed);

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

	for (uint32_t i=0; i<100; i++) {
		Z3_solver_push(m_ctxt, m_solver);

//		uint32_t n_vars = (vars.size()*30)/100;
		uint32_t n_vars = vars.size();
		if (n_vars < 1) {
			n_vars = 1;
		}

		std::vector<Z3ModelVar *> vars_t(vars);
		Z3_ast term = 0;

		for (uint32_t j=0; j<n_vars; j++) {
			uint32_t var_idx = 0; // (m_lfsr.value() % vars_t.size());
			uint64_t coeff = m_lfsr.next();
//			fprintf(stdout, "Force idx %d (%s) %llx\n",
//					var_idx, vars_t.at(var_idx)->name().c_str(), coeff);
			Z3ModelVar *var = vars_t.at(var_idx);
			Z3_ast var_ast = var->var();
			if (var->bits() < 64) {
				if (var->is_signed()) {
					var_ast = Z3_mk_sign_ext(m_ctxt, (64-var->bits()), var_ast);
				} else {
					var_ast = Z3_mk_zero_ext(m_ctxt, (64-var->bits()), var_ast);
				}
			}
			vars_t.erase(vars_t.begin()+var_idx);
			Z3_ast this_t = Z3_mk_bvmul(m_ctxt, var_ast,
					Z3_mk_unsigned_int64(m_ctxt, coeff,
							Z3_mk_bv_sort(m_ctxt, 64)));
			if (term) {
				term = Z3_mk_bvadd(m_ctxt, term, this_t);
			} else {
				term = this_t;
			}
		}

		uint32_t hash_bits = 20;
		Z3_ast sum = Z3_mk_extract(m_ctxt, (hash_bits-1), 0, term);
		uint64_t hash = m_lfsr.next();
//		fprintf(stdout, "Hash[7:0]=0x%08x\n", (uint32_t)(hash & ((1 << hash_bits)-1)));
		Z3_ast eq = Z3_mk_eq(m_ctxt, sum,
				Z3_mk_unsigned_int64(m_ctxt, hash, Z3_mk_bv_sort(m_ctxt, hash_bits)));

		Z3_solver_assert(m_ctxt, m_solver, eq);

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

bool Z3ModelProcessor::compute_sign(IScalarType *t) {
	bool is_signed = true;

	switch (t->getScalarType()) {
		case IScalarType::ScalarType_Bool:
			is_signed = false;
			break;
		case IScalarType::ScalarType_Bit:
			is_signed = false;
		break;
		case IScalarType::ScalarType_Int:
			is_signed = true;
		break;

		case IScalarType::ScalarType_String:
			is_signed = false;
		break;

		default:
			fprintf(stdout, "Error: unhandled compute-bits case %d\n",
					t->getScalarType());
			break;
	}

	return is_signed;
}

void Z3ModelProcessor::visit_binary_expr(IBinaryExpr *be) {
	visit_expr(be->getLHS());
	Z3ExprTerm lhs = m_expr;
	visit_expr(be->getRHS());
	Z3ExprTerm rhs = m_expr;

	switch (be->getBinOpType()) {
	case IBinaryExpr::BinOp_And: {
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		m_expr = Z3ExprTerm(
				Z3_mk_bvand(m_ctxt,
						lhs.expr(),
						rhs.expr()),
				lhs.size(),
				lhs.is_signed());
	} break;
	case IBinaryExpr::BinOp_AndAnd: {
		// TODO: bool AND?
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		m_expr = Z3ExprTerm(
				Z3_mk_bvand(m_ctxt,
						lhs.expr(),
						rhs.expr()),
				lhs.size(),
				lhs.is_signed());
	} break;
//	case IBinaryExpr::BinOp_ArrayRef:
//		break;
//	case IBinaryExpr::BinOp_Divide:
//		// TODO: figure out signing
//		m_expr = Z3_mk_bvudiv(m_ctxt, lhs, rhs);
//		break;
//	case IBinaryExpr::BinOp_Eq:
//		fprintf(stdout, "TODO: '=' not supported\n");
//		break;
	case IBinaryExpr::BinOp_EqEq: {
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		m_expr = Z3ExprTerm(
				Z3_mk_eq(m_ctxt,
						lhs.expr(),
						rhs.expr()),
				lhs.size(),
				lhs.is_signed());
	} break;

	case IBinaryExpr::BinOp_NotEq: {
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		m_expr = Z3ExprTerm(
				Z3_mk_not(m_ctxt,
						Z3_mk_eq(m_ctxt,
								lhs.expr(),
								rhs.expr())
				),
				lhs.size(),
				lhs.is_signed());
	} break;

	case IBinaryExpr::BinOp_GE: {
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		if (lhs.is_signed() && rhs.is_signed()) {
			m_expr = Z3ExprTerm(
					Z3_mk_bvsge(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvuge(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		}
	} break;

	case IBinaryExpr::BinOp_GT: {
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		if (lhs.is_signed() && rhs.is_signed()) {
			m_expr = Z3ExprTerm(
					Z3_mk_bvsgt(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvugt(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		}
	} break;
	case IBinaryExpr::BinOp_LE: {
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		if (lhs.is_signed() && rhs.is_signed()) {
			m_expr = Z3ExprTerm(
					Z3_mk_bvsle(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvule(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		}
	} break;

	case IBinaryExpr::BinOp_LT: {
		uint32_t bits;
		if (lhs.size() != rhs.size()) {
			if (lhs.size() < rhs.size()) {
				// upsize lhs
				lhs = upsize(lhs, rhs.size());
				bits = rhs.size();
			} else {
				// upsize rhs
				rhs = upsize(rhs, lhs.size());
				bits = lhs.size();
			}
		} else {
			bits = lhs.size();
		}
		if (lhs.is_signed() && rhs.is_signed()) {
			m_expr = Z3ExprTerm(
					Z3_mk_bvslt(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvult(m_ctxt,
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		}
	} break;
	case IBinaryExpr::BinOp_OrOr: {
		Z3_ast args[] = {lhs.expr(), rhs.expr()};
		m_expr = Z3ExprTerm(
				Z3_mk_or(m_ctxt, 2, args),
				1,
				lhs.is_signed());
	} break;
	default:
		fprintf(stdout, "Error: unhandled binary expr %d\n",
				be->getBinOpType());
	}
}

void Z3ModelProcessor::visit_constraint_expr_stmt(IConstraintExpr *c) {
	m_expr = Z3ExprTerm();
	visit_expr(c->getExpr());
	if (m_expr_depth == 0) {
	if (m_expr.expr()) {
//		fprintf(stdout, "AST:\n%s\n",
//				Z3_ast_to_string(m_ctxt, m_expr.expr()));
		Z3_solver_assert(m_ctxt, m_solver,
				m_expr.expr());
	} else {
		fprintf(stdout, "Error: expr resulted in null term\n");
	}
	}

//	fprintf(stdout, "constraint statement: %p\n", m_expr);
}

void Z3ModelProcessor::visit_constraint_if_stmt(IConstraintIf *c) {
	m_expr_depth++;
	visit_expr(c->getCond());
	Z3ExprTerm iff = m_expr;
	visit_constraint_stmt(c->getTrue());
	Z3ExprTerm case_true = m_expr;

	Z3_ast iff_ast = iff.expr();

	if (c->getFalse()) {
		m_if_else_conds.push_back(iff_ast);

		// Create a NOT of
		visit_constraint_stmt(c->getFalse());
		Z3ExprTerm case_false = m_expr;

		m_expr = Z3ExprTerm(
				Z3_mk_ite(m_ctxt,
						iff.expr(),
						case_true.expr(),
						case_false.expr()),
						case_true.size(),
						case_true.is_signed()
				);
		m_if_else_conds.pop_back();
	} else {
		Z3_ast ast = Z3_mk_implies(m_ctxt,
						iff.expr(),
						case_true.expr());
//		fprintf(stdout, "if AST:\n%s\n",
//				Z3_ast_to_string(m_ctxt, ast));
		Z3_solver_assert(m_ctxt, m_solver, ast);
	}

	m_expr_depth--;

	m_expr = Z3ExprTerm();
}

void Z3ModelProcessor::visit_literal_expr(ILiteral *l) {
	switch (l->getLiteralType()) {
	case ILiteral::LiteralString:
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_ctxt,
				m_strtab.str2id(l->getString()),
				Z3_mk_bv_sort(m_ctxt, m_strtab.bits())),
				m_strtab.bits(),
				false);
		break;
	case ILiteral::LiteralBool:
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_ctxt,
				l->getBool(),
				Z3_mk_bv_sort(m_ctxt, 2)),
				2,
				false);
		break;
	case ILiteral::LiteralInt:
		// TODO: determine size from the user
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_ctxt,
						l->getInt(),
						Z3_mk_bv_sort(m_ctxt, 32)),
				32,
				true);
		break;
	case ILiteral::LiteralBit:
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_ctxt,
				l->getBit(),
				Z3_mk_bv_sort(m_ctxt, 32)),
				32,
				false);
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

	m_expr = Z3ExprTerm(
			v->second->var(),
			v->second->bits(),
			true); // TODO: signed nature?
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


Z3ExprTerm Z3ModelProcessor::upsize(
		const Z3ExprTerm		&target,
		uint32_t				bits) {

	Z3_ast upsized;

	fprintf(stdout, "Upsize to %d\n", bits);

	if (target.is_signed()) {
		upsized = Z3_mk_sign_ext(m_ctxt,
				(bits-target.size()), target.expr());
	} else {
		upsized = Z3_mk_zero_ext(m_ctxt,
				(bits-target.size()), target.expr());
	}

	return Z3ExprTerm(
			upsized,
			bits,
			target.is_signed()
			);
}

void Z3ModelProcessor::compute_domain(Z3ModelVar &var) {
	if (var.is_signed()) {
		int64_t min = LLONG_MIN;
		int64_t max = LLONG_MAX;


	} else {

	}
}

void Z3ModelProcessor::z3_error_handler(Z3_context c, Z3_error_code e) {
	throw SolverErrorException(Z3_get_error_msg(c, e));
}

