/*
 * Z3ModelProcessor.cpp
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#include "Z3ModelProcessor.h"
#include <limits.h>


Z3ModelProcessor::Z3ModelProcessor() : m_cfg(0), m_ctxt(0) {
	m_prefix_valid = false;
	m_expr_depth = 0;
	m_solver = 0;
	m_ctxt = 0;
	m_exec_listener = 0;
	m_model = 0;

//	m_lfsr.seed(0);
//	m_lfsr.next();
//	m_lfsr.next();
//	m_lfsr.next();
//	m_lfsr.next();
}

Z3ModelProcessor::~Z3ModelProcessor() {
	// TODO Auto-generated destructor stub
}

bool Z3ModelProcessor::build(IComponent *comp, IAction *action) {
	m_root_comp   = comp;
	m_root_action = action;

	m_strtab.build(comp);
	m_strtab.build(action);

	m_cfg = Z3_mk_config();
	m_ctxt = Z3_mk_context(m_cfg);

	Z3_set_error_handler(m_ctxt, &Z3ModelProcessor::z3_error_handler);

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

	return true;
}

bool Z3ModelProcessor::run() {
	bool ret = false;

	Z3_lbool result = Z3_solver_check(m_ctxt, m_solver);
	if (result != Z3_L_TRUE) {
		fprintf(stdout, "Error: system doesn't solve\n");
		return false;
	}

	exec_action(
			m_root_action->getName(),
			m_root_action);

//	std::vector<Z3ModelVar *> vars;
//	for (std::map<std::string,Z3ModelVar *>::iterator it=m_variables.begin();
//			it!=m_variables.end(); it++) {
//		it->second->reset();
//		vars.push_back(it->second);
//	}
//
//	for (uint32_t i=0; i<vars.size(); i++) {
//		fprintf(stdout, "--> solve %d\n", i);
//		solve(vars);
//		fprintf(stdout, "<-- solve %d\n", i);
//
////		m_model = Z3_solver_get_model(m_ctxt, m_solver);
////		if (m_model) {
////			Z3_model_inc_ref(m_ctxt, m_model);
////
////			for (std::map<std::string, Z3ModelVar *>::iterator it=m_variables.begin();
////					it!=m_variables.end(); it++) {
////				Z3ModelVar *v = it->second;
////				VarVal val = v->get_val(m_ctxt, m_model);
////				switch (val.type) {
////				case VarVal_Int:
////					fprintf(stdout, "%s: %d\n", v->name().c_str(), val.si);
////					break;
////				case VarVal_Uint:
////					fprintf(stdout, "%s: 0x%08x\n", v->name().c_str(), val.ui);
////					break;
////				case VarVal_Bool:
////					fprintf(stdout, "%s: %s\n", v->name().c_str(),
////							(val.b)?"true":"false");
////					break;
////				case VarVal_String:
////					fprintf(stdout, "%s: %s\n", v->name().c_str(),
////							m_strtab.id2str(val.ui).c_str());
////					break;
////				}
////			}
////		}
////
////		uint32_t val = vars.at(i)->get_val(m_ctxt, m_model).ui;
////		fprintf(stdout, "Fix %d to %lld\n", i, val);
////		vars.at(i)->set_val(val);
////		Z3_model_dec_ref(m_ctxt, m_model);
//	}

	return true;
}

void Z3ModelProcessor::set_exec_listener(IExecListener *l) {
	m_exec_listener = l;
}

void Z3ModelProcessor::visit_field(IField *f) {
	fprintf(stdout, "visit_field: %s\n", f->getName().c_str());

	if (f->getDataType()->getType() == IBaseItem::TypeScalar) {
		std::string id = prefix() + "." + f->getName();
		uint32_t bits = compute_bits(
				dynamic_cast<IScalarType *>(f->getDataType()));
		VarValType type;
		IScalarType *st = dynamic_cast<IScalarType *>(f->getDataType());

		switch (st->getScalarType()) {
		case IScalarType::ScalarType_Bit: type = VarVal_Uint; break;
		case IScalarType::ScalarType_Int: type = VarVal_Int; break;
		case IScalarType::ScalarType_Bool: type = VarVal_Bool; break;
		case IScalarType::ScalarType_String: type = VarVal_String; break;
		}

		fprintf(stdout, "Note: scalar variable %s\n", id.c_str());

		Z3_ast var = Z3_mk_const(m_ctxt,
				Z3_mk_string_symbol(m_ctxt, id.c_str()),
				Z3_mk_bv_sort(m_ctxt, bits));
		m_variables[id] = new Z3ModelVar(id, var, bits, type);

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
	} else if (f->getDataType()->getType() == IBaseItem::TypeComponent) {
		if (dynamic_cast<IAction *>(f->getParent())) {
			// Ignore component-type fields in action, since this
			// is 'comp'
		} else {
			fprintf(stdout, "TODO: determine whether component-type field should be ignored\n");
		}
	} else {
		// composite data type?
		fprintf(stdout, "Error: unknown data type %d\n",
				f->getDataType()->getType());
	}
}

VarVal Z3ModelProcessor::get_value(const std::string &path) {
	std::map<std::string, Z3ModelVar *>::iterator it =
			m_variables.find(path);
	VarVal ret;

	if (it != m_variables.end()) {
		Z3ModelVar *var = it->second;
		// The return is always the integer value
		ret = var->get_val(m_ctxt, m_model);

		if (ret.type == VarVal_String) {
			ret.s = m_strtab.id2str(ret.ui);
		}
	} else {
		fprintf(stdout, "Error: no variable named %s\n", path.c_str());
	}

	return ret;
}

bool Z3ModelProcessor::solve(const std::vector<Z3ModelVar *> &vars) {

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

		if (total_bits == 0) {
			fprintf(stdout, "Error: no random variables\n");
		}

		uint32_t hash_bits = 20;

		if (total_bits < hash_bits) {
			hash_bits = total_bits;
		}

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

void Z3ModelProcessor::exec_action(
		const std::string		&context,
		IAction 				*action) {
	std::vector<Z3ModelVar *> reset_vars;
	std::vector<Z3ModelVar *> rand_vars;
	IExec *pre_solve=0;
	IExec *post_solve=0;
	IExec *body=0;

	fprintf(stdout, "--> exec_action %s\n", context.c_str());

	for (std::vector<IBaseItem *>::const_iterator it=action->getItems().begin();
			it!=action->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeExec) {
			IExec *exec = dynamic_cast<IExec *>(*it);
			switch (exec->getExecKind()) {
			case IExec::Body: body = exec; break;
			case IExec::PreSolve: pre_solve = exec; break;
			case IExec::PostSolve: post_solve = exec; break;
			default:
				fprintf(stdout, "TODO: unsupported exec kind %d\n", exec->getExecKind());
				break;
			}
		}
	}

	if (pre_solve) {
		fprintf(stdout, "TODO: support pre-solve exec\n");
	}

	collect_variables(reset_vars, context, action);

	// Collect variables that will be randomized in this action
	collect_rand_variables(rand_vars, context, action);

	// Reset all context actions
	for (std::vector<Z3ModelVar *>::const_iterator it=reset_vars.begin();
			it!=reset_vars.end(); it++) {
		(*it)->reset();
	}

	// Solve for the random variables
	if (rand_vars.size() > 0) {
		if (!solve(rand_vars)) {
			fprintf(stdout, "Error: solve failed\n");
		}
		// Now, lock down the values of the random variables
		for (std::vector<Z3ModelVar *>::const_iterator it=rand_vars.begin();
				it!=rand_vars.end(); it++) {
			const VarVal &val = (*it)->get_val(m_ctxt, m_model);
			fprintf(stdout, "%s = %lld\n",
					(*it)->name().c_str(),
					val.ui);
			(*it)->set_val(val.ui);
		}
	}

	if (post_solve) {
		fprintf(stdout, "TODO: support post-solve exec\n");
	}

	if (body) {
		if (m_exec_listener) {
			m_exec_listener->exec(context, body);
		} else {
			fprintf(stdout, "Warning: no exec listener\n");
		}
	} else if (action->getGraph()) {
		exec_activity_stmt(context, action->getGraph());
	} else {
		fprintf(stdout, "Note: this is a boring action with no exec and no activity\n");
	}

	fprintf(stdout, "<-- exec_action\n");
}

void Z3ModelProcessor::collect_variables(
		std::vector<Z3ModelVar *>		&vars,
		const std::string				&context,
		IAction							*action) {
	for (uint32_t i=0; i<action->getItems().size(); i++) {
		IBaseItem *item = action->getItems().at(i);

		if (dynamic_cast<IField *>(item)) {
			IField *field = dynamic_cast<IField *>(item);

			if (dynamic_cast<IScalarType *>(field->getDataType())) {
				// scalar field
				std::string varname = context;
				varname.append(".");
				varname.append(field->getName());
			} else {
				// composite or user-defined field

			}
		}
	}
}

// In this case, we only collect variables that will
// be randomized at the beginning of this action
void Z3ModelProcessor::collect_rand_variables(
		std::vector<Z3ModelVar *>		&vars,
		const std::string				&context,
		IAction							*action) {
	for (uint32_t i=0; i<action->getItems().size(); i++) {
		IBaseItem *item = action->getItems().at(i);

		if (dynamic_cast<IField *>(item)) {
			IField *field = dynamic_cast<IField *>(item);

			if (dynamic_cast<IScalarType *>(field->getDataType())) {
				// scalar field
				std::string varname = context;
				varname.append(".");
				varname.append(field->getName());
				std::map<std::string,Z3ModelVar *>::iterator it =
						m_variables.find(varname);

				if (it != m_variables.end()) {
					vars.push_back(it->second);
				}
			} else {
				// composite or user-defined field

			}
		}
	}
}

void Z3ModelProcessor::exec_activity_stmt(
		const std::string			&context,
		IActivityStmt				*stmt) {
	switch (stmt->getStmtType()) {
	case IActivityStmt::ActivityStmt_Block: {
		IActivityBlockStmt *block = dynamic_cast<IActivityBlockStmt *>(stmt);
		fprintf(stdout, "block=%p\n", block);
		fflush(stdout);
		for (std::vector<IActivityStmt *>::const_iterator it=block->getStmts().begin();
				it!=block->getStmts().end(); it++) {
			exec_activity_stmt(context, *it);
		}
	} break;
	case IActivityStmt::ActivityStmt_Traverse: {
		exec_activity_traverse_stmt(context,
				dynamic_cast<IActivityTraverseStmt *>(stmt));
	} break;

	default: {
		fprintf(stdout, "TODO: unhandled activity-stmt %d\n",
				stmt->getStmtType());
	}
	}
}

void Z3ModelProcessor::exec_activity_traverse_stmt(
			const std::string				&context,
			IActivityTraverseStmt			*stmt) {
	IVariableRef *ref = stmt->getAction();
	fprintf(stdout, "ref=%p target=%p\n", ref, ref->getTarget());
	fflush(stdout);

	IField *action_f = dynamic_cast<IField *>(ref->getTarget());
	fprintf(stdout, "action_f=%p\n", action_f);
	fflush(stdout);
	IRefType *type_r = dynamic_cast<IRefType *>(action_f->getDataType());
	IAction *action = dynamic_cast<IAction *>(type_r->getTargetType());

	fprintf(stdout, "action_f.type=%d\n", action_f->getDataType()->getType());
	fflush(stdout);

	std::string this_ctxt = context + "." + ref->getId();

	exec_action(this_ctxt, action);
}

void Z3ModelProcessor::z3_error_handler(Z3_context c, Z3_error_code e) {
	throw SolverErrorException(Z3_get_error_msg(c, e));
}

