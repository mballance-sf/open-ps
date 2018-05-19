/*
 * Z3ModelExprBuilder.cpp
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#include "Z3ModelExprBuilder.h"
#include "Z3ModelBuilder.h"

Z3ModelExprBuilder::Z3ModelExprBuilder(Z3ModelBuilder *builder) : m_builder(builder) {
	// TODO Auto-generated constructor stub

}

Z3ModelExprBuilder::~Z3ModelExprBuilder() {
	// TODO Auto-generated destructor stub
}

Z3ExprTerm Z3ModelExprBuilder::build(IExpr *expr) {
	visit_expr(expr);
	return m_expr;
}

void Z3ModelExprBuilder::visit_binary_expr(IBinaryExpr *be) {
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
				Z3_mk_bvand(m_builder->ctxt(),
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
				Z3_mk_bvand(m_builder->ctxt(),
						lhs.expr(),
						rhs.expr()),
				lhs.size(),
				lhs.is_signed());
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
				Z3_mk_eq(m_builder->ctxt(),
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
				Z3_mk_not(m_builder->ctxt(),
						Z3_mk_eq(m_builder->ctxt(),
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
					Z3_mk_bvsge(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvuge(m_builder->ctxt(),
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
					Z3_mk_bvsgt(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvugt(m_builder->ctxt(),
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
					Z3_mk_bvsle(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvule(m_builder->ctxt(),
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
					Z3_mk_bvslt(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		} else {
			m_expr = Z3ExprTerm(
					Z3_mk_bvult(m_builder->ctxt(),
							lhs.expr(),
							rhs.expr()),
							bits,
							lhs.is_signed());
		}
	} break;
	case IBinaryExpr::BinOp_OrOr: {
		Z3_ast args[] = {lhs.expr(), rhs.expr()};
		m_expr = Z3ExprTerm(
				Z3_mk_or(m_builder->ctxt(), 2, args),
				1,
				lhs.is_signed());
	} break;
	default:
		fprintf(stdout, "Error: unhandled binary expr %d\n",
				be->getBinOpType());
	}
}

void Z3ModelExprBuilder::visit_literal_expr(ILiteral *l) {
	switch (l->getLiteralType()) {
	case ILiteral::LiteralString:
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_builder->ctxt(),
				m_strtab.str2id(l->getString()),
				Z3_mk_bv_sort(m_builder->ctxt(), m_strtab.bits())),
				m_strtab.bits(),
				false);
		break;
	case ILiteral::LiteralBool:
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_builder->ctxt(),
				l->getBool(),
				Z3_mk_bv_sort(m_builder->ctxt(), 2)),
				2,
				false);
		break;
	case ILiteral::LiteralInt:
		// TODO: determine size from the user
		m_expr = Z3ExprTerm(
				Z3_mk_int(m_builder->ctxt(),
						l->getInt(),
						Z3_mk_bv_sort(m_builder->ctxt(), 32)),
				32,
				true);
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

void Z3ModelExprBuilder::visit_variable_ref(IVariableRef *ref) {
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

Z3ExprTerm Z3ModelExprBuilder::upsize(
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



