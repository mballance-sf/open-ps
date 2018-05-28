/*
 * Z3ModelBuildExpr.cpp
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
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#include "Z3ModelBuilder.h"
#include "Z3ModelBuildExpr.h"

Z3ModelBuildExpr::Z3ModelBuildExpr(Z3ModelBuilder *builder) : m_builder(builder) {
	// TODO Auto-generated constructor stub

}

Z3ModelBuildExpr::~Z3ModelBuildExpr() {
	// TODO Auto-generated destructor stub
}

Z3ExprTerm Z3ModelBuildExpr::build(IExpr *expr) {
	visit_expr(expr);
	return m_expr;
}

void Z3ModelBuildExpr::visit_binary_expr(IBinaryExpr *be) {
	visit_expr(be->getLHS());
	Z3ExprTerm lhs = m_expr;
	visit_expr(be->getRHS());
	Z3ExprTerm rhs = m_expr;

	switch (be->getBinOpType()) {
	case IBinaryExpr::BinOp_And: {
		m_expr = mk_bitwise_and(lhs, rhs);

	} break;
	case IBinaryExpr::BinOp_AndAnd: {
		m_expr = mk_logical_and(lhs, rhs);

	} break;
//	case IBinaryExpr::BinOp_ArrayRef:
//		break;
//	case IBinaryExpr::BinOp_Divide:
//		// TODO: figure out signing
//		m_expr = Z3_mk_bvudiv(m_builder->ctxt(), lhs, rhs);
//		break;
//	case IBinaryExpr::BinOp_Eq:
//		fprintf(stdout, "TODO: '=' not supported\n");
//		break;
	case IBinaryExpr::BinOp_EqEq: {
		m_expr = mk_eq(lhs, rhs);

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
				Z3_mk_not(m_builder->ctxt(),
						Z3_mk_eq(m_builder->ctxt(),
								lhs.expr(),
								rhs.expr())
				),
				lhs.size(),
				lhs.is_signed());
	} break;

	case IBinaryExpr::BinOp_GE: {
		m_expr = mk_ge(lhs, rhs);

	} break;

	case IBinaryExpr::BinOp_GT: {
		m_expr = mk_gt(lhs, rhs);

	} break;
	case IBinaryExpr::BinOp_LE: {
		m_expr = mk_le(lhs, rhs);

	} break;

	case IBinaryExpr::BinOp_LT: {
		m_expr = mk_lt(lhs, rhs);

	} break;

	case IBinaryExpr::BinOp_OrOr: {
		m_expr = mk_logical_or(lhs, rhs);


	} break;
	default:
		fprintf(stdout, "Error: unhandled binary expr %d\n",
				be->getBinOpType());
	}
}

void Z3ModelBuildExpr::visit_in_expr(IInExpr *in) {
	std::vector<Z3_ast> terms;

	visit_expr(in->getLhs());
	Z3ExprTerm var = m_expr;

	for (uint32_t i=0; i<in->getRhs()->ranges().size(); i++) {
		IOpenRangeValue *rv = in->getRhs()->ranges().at(i);

		visit_expr(rv->getLHS());
		Z3ExprTerm lhs = m_expr;

		if (rv->getRHS()) {
			// bi-directional
			Z3ExprTerm var_l = var;
			Z3ExprTerm var_r = var;
			visit_expr(rv->getRHS());
			Z3ExprTerm rhs = m_expr;
			Z3_ast t[2];

			size_terms(var_l, lhs);
			size_terms(var_r, rhs);

			if (var_l.is_signed() || lhs.is_signed()) {
				t[0] = Z3_mk_bvsge(m_builder->ctxt(), var_l.expr(), lhs.expr());
			} else {
				t[0] = Z3_mk_bvuge(m_builder->ctxt(), var_l.expr(), lhs.expr());
			}

			if (var_r.is_signed() || rhs.is_signed()) {
				t[1] = Z3_mk_bvsle(m_builder->ctxt(), var_r.expr(), rhs.expr());
			} else {
				t[1] = Z3_mk_bvule(m_builder->ctxt(), var_r.expr(), rhs.expr());
			}

			terms.push_back(Z3_mk_and(m_builder->ctxt(), 2, t));
		} else {
			// unidirectional
			Z3ExprTerm var_t = var;
			size_terms(var_t, lhs);

			terms.push_back(Z3_mk_eq(m_builder->ctxt(),
					var_t.expr(),
					lhs.expr()));
		}
	}

	if (terms.size() > 1) {
		m_expr = Z3ExprTerm(
				Z3_mk_or(m_builder->ctxt(),
						terms.size(), terms.data()),
				1,
				false);
	} else {
		// Just move the single term over
		m_expr = Z3ExprTerm(
				terms.at(0),
				1,
				false);
	}
}

void Z3ModelBuildExpr::visit_literal_expr(ILiteral *l) {
	switch (l->getLiteralType()) {
	case ILiteral::LiteralString:
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_builder->ctxt(),
						m_builder->strtab().str2id(l->getString()),
						Z3_mk_bv_sort(m_builder->ctxt(),
								m_builder->strtab().bits())),
				m_builder->strtab().bits(),
				false);
		break;
	case ILiteral::LiteralBool:
		m_expr = mk_bool(l->getBool());
		break;
	case ILiteral::LiteralInt:
		// TODO: determine size from the user
		m_expr = mk_int(l->getInt(), 32);

		break;
	case ILiteral::LiteralBit:
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_builder->ctxt(),
				l->getBit(),
				Z3_mk_bv_sort(m_builder->ctxt(), 32)),
				32,
				false);
		break;
	default:
		fprintf(stdout, "Error: unhandled literal case %d\n",
				l->getLiteralType());
		break;

	}
}

void Z3ModelBuildExpr::visit_variable_ref(IVariableRef *ref) {
	// Determine the full name of the variable
	std::string name = ref->toString();
	fprintf(stdout, "Ref: %s prefix: %s\n",
			name.c_str(), m_builder->name_provider().name().c_str());
	std::string full_name = m_builder->name_provider().name() + "." + name;

	Z3ModelVar *var = m_builder->get_variable(full_name);

	m_expr = mk_var(var);

	fprintf(stdout, "m_expr=%p\n", m_expr);
}

Z3ExprTerm Z3ModelBuildExpr::mk_bitwise_and(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	uint32_t bits;
	if (lhs.size() != rhs.size()) {
		Z3ExprTerm lhs_t = size_lhs(lhs, rhs);
		Z3ExprTerm rhs_t = size_rhs(lhs, rhs);

		return Z3ExprTerm(
				Z3_mk_bvand(m_builder->ctxt(),
						lhs_t.expr(),
						rhs_t.expr()),
				lhs_t.size(),
				lhs_t.is_signed());
	} else {
		return Z3ExprTerm(
				Z3_mk_bvand(m_builder->ctxt(),
						lhs.expr(),
						rhs.expr()),
				lhs.size(),
				lhs.is_signed());
	}
}

Z3ExprTerm Z3ModelBuildExpr::mk_logical_and(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	// TODO: bool AND?
	uint32_t bits;
	Z3_ast terms[2];
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
		terms[0] = lhs.expr();
		terms[1] = rhs.expr();
		return Z3ExprTerm(
				Z3_mk_and(m_builder->ctxt(),
						2,
						terms),
				1,
				false);
	}


}

Z3ExprTerm Z3ModelBuildExpr::mk_bitwise_or(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	if (lhs.size() != rhs.size()) {
		lhs = size_lhs(lhs, rhs);
		rhs = size_rhs(lhs, rhs);
	}

	return Z3ExprTerm(
			Z3_mk_bvor(m_builder->ctxt(),
					lhs.expr(),
					rhs.expr()),
					lhs.size(),
					lhs.is_signed());
}

Z3ExprTerm Z3ModelBuildExpr::mk_logical_or(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	Z3_ast args[] = {lhs.expr(), rhs.expr()};
	return Z3ExprTerm(
			Z3_mk_or(m_builder->ctxt(), 2, args),
			1,
			lhs.is_signed());
}

Z3ExprTerm Z3ModelBuildExpr::mk_eq(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	if (lhs.size() != rhs.size()) {
		Z3ExprTerm lhs_t = size_lhs(lhs, rhs);
		Z3ExprTerm rhs_t = size_rhs(lhs, rhs);
		return Z3ExprTerm(
				Z3_mk_eq(m_builder->ctxt(),
						lhs_t.expr(),
						rhs_t.expr()),
				lhs_t.size(),
				lhs_t.is_signed());
	} else {
		return Z3ExprTerm(
				Z3_mk_eq(m_builder->ctxt(),
						lhs.expr(),
						rhs.expr()),
				lhs.size(),
				lhs.is_signed());
	}
}

Z3ExprTerm Z3ModelBuildExpr::mk_ge(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	if (lhs.size() != rhs.size()) {
		Z3ExprTerm lhs_t = size_lhs(lhs, rhs);
		Z3ExprTerm rhs_t = size_rhs(lhs, rhs);
		if (lhs_t.is_signed() && rhs_t.is_signed()) {
			return Z3ExprTerm(
					Z3_mk_bvsge(m_builder->ctxt(),
							lhs_t.expr(),
							rhs_t.expr()),
							lhs_t.size(),
							lhs_t.is_signed());
		} else {
			return Z3ExprTerm(
					Z3_mk_bvuge(m_builder->ctxt(),
							lhs_t.expr(),
							rhs_t.expr()),
							lhs_t.size(),
							lhs_t.is_signed());
		}
	} else {
		if (lhs.is_signed() && rhs.is_signed()) {
			return Z3ExprTerm(
					Z3_mk_bvsge(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							lhs.size(),
							lhs.is_signed());
		} else {
			return Z3ExprTerm(
					Z3_mk_bvuge(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							lhs.size(),
							lhs.is_signed());
		}
	}

}

Z3ExprTerm Z3ModelBuildExpr::mk_gt(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	if (lhs.size() != rhs.size()) {
		Z3ExprTerm lhs_t = size_lhs(lhs, rhs);
		Z3ExprTerm rhs_t = size_rhs(lhs, rhs);
		if (lhs_t.is_signed() && rhs_t.is_signed()) {
			return Z3ExprTerm(
					Z3_mk_bvsgt(m_builder->ctxt(),
							lhs_t.expr(),
							rhs_t.expr()),
							lhs_t.size(),
							lhs_t.is_signed());
		} else {
			return Z3ExprTerm(
					Z3_mk_bvugt(m_builder->ctxt(),
							lhs_t.expr(),
							rhs_t.expr()),
							lhs_t.size(),
							lhs_t.is_signed());
		}
	} else {

		if (lhs.is_signed() && rhs.is_signed()) {
			return Z3ExprTerm(
					Z3_mk_bvsgt(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							lhs.size(),
							lhs.is_signed());
		} else {
			return Z3ExprTerm(
					Z3_mk_bvugt(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							lhs.size(),
							lhs.is_signed());
		}
	}
}

Z3ExprTerm Z3ModelBuildExpr::mk_le(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	if (lhs.size() != rhs.size()) {
		Z3ExprTerm lhs_t = size_lhs(lhs, rhs);
		Z3ExprTerm rhs_t = size_rhs(lhs, rhs);
		if (lhs_t.is_signed() && rhs_t.is_signed()) {
			return Z3ExprTerm(
					Z3_mk_bvsle(m_builder->ctxt(),
							lhs_t.expr(),
							rhs_t.expr()),
							lhs_t.size(),
							lhs_t.is_signed());
		} else {
			return Z3ExprTerm(
					Z3_mk_bvule(m_builder->ctxt(),
							lhs_t.expr(),
							rhs_t.expr()),
							lhs_t.size(),
							lhs_t.is_signed());
		}
	} else {
		if (lhs.is_signed() && rhs.is_signed()) {
			return Z3ExprTerm(
					Z3_mk_bvsle(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							lhs.size(),
							lhs.is_signed());
		} else {
			return Z3ExprTerm(
					Z3_mk_bvule(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							lhs.size(),
							lhs.is_signed());
		}
	}
}

Z3ExprTerm Z3ModelBuildExpr::mk_lt(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs) {
	if (lhs.size() != rhs.size()) {
		lhs = size_lhs(lhs, rhs);
		rhs = size_rhs(lhs, rhs);
	}

	if (lhs.is_signed() && rhs.is_signed()) {
		return Z3ExprTerm(
				Z3_mk_bvslt(m_builder->ctxt(),
						lhs.expr(),
						rhs.expr()),
						lhs.size(),
						lhs.is_signed());
	} else {
		return Z3ExprTerm(
				Z3_mk_bvult(m_builder->ctxt(),
						lhs.expr(),
						rhs.expr()),
						lhs.size(),
						lhs.is_signed());
	}
}

Z3ExprTerm Z3ModelBuildExpr::mk_bool(bool v) {
	return Z3ExprTerm(
			Z3_mk_int(
					m_builder->ctxt(),
					v,
					Z3_mk_bv_sort(m_builder->ctxt(), 1)
			),
			1,
			false);
}

Z3ExprTerm Z3ModelBuildExpr::mk_int(int32_t v, uint32_t bits) {
	return Z3ExprTerm(
			Z3_mk_int(m_builder->ctxt(), v,
					Z3_mk_bv_sort(m_builder->ctxt(), bits)),
			bits,
			true);
}

Z3ExprTerm Z3ModelBuildExpr::mk_var(Z3ModelVar *var) {
	return Z3ExprTerm(var->var(), var->bits(), var->is_signed());
}

void Z3ModelBuildExpr::size_terms(
		Z3ExprTerm &lhs,
		Z3ExprTerm &rhs) {

	if (lhs.size() != rhs.size()) {
		if (lhs.size() < rhs.size()) {
			// upsize lhs
			lhs = upsize(lhs, rhs.size());
		} else {
			// upsize rhs
			rhs = upsize(rhs, lhs.size());
		}
	}
}

Z3ExprTerm Z3ModelBuildExpr::size_lhs(
		const Z3ExprTerm &lhs,
		const Z3ExprTerm &rhs) {

	if (lhs.size() < rhs.size()) {
		// upsize lhs
		return upsize(lhs, rhs.size());
	} else {
		return lhs;
	}
}

Z3ExprTerm Z3ModelBuildExpr::size_rhs(
		const Z3ExprTerm &lhs,
		const Z3ExprTerm &rhs) {

	if (lhs.size() > rhs.size()) {
		// upsize rhs
		return upsize(rhs, lhs.size());
	} else {
		return rhs;
	}
}

Z3ExprTerm Z3ModelBuildExpr::upsize(
		const Z3ExprTerm		&target,
		uint32_t				bits) {

	Z3_ast upsized;

	fprintf(stdout, "Upsize to %d\n", bits);

	if (target.is_signed()) {
		upsized = Z3_mk_sign_ext(
				m_builder->ctxt(),
				(bits-target.size()), target.expr());
	} else {
		upsized = Z3_mk_zero_ext(
				m_builder->ctxt(),
				(bits-target.size()), target.expr());
	}

	return Z3ExprTerm(
			upsized,
			bits,
			target.is_signed()
			);
}



