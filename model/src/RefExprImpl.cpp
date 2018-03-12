/*
 * RefExprImpl.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: ballance
 */

#include "RefExprImpl.h"

namespace psi_api {

RefExprImpl::RefExprImpl(
		IScopeItem						*scope,
		const std::vector<std::string>	&path) :
				m_scope(scope), m_path(path),
				m_expr(0) {

}

RefExprImpl::~RefExprImpl() {
	// TODO Auto-generated destructor stub
}

IExpr *RefExprImpl::getExpr() const {
	return m_expr;
}

void RefExprImpl::setExpr(IExpr *expr) {
	m_expr = expr;
}

IExpr *RefExprImpl::clone() const {
	RefExprImpl *ret = new RefExprImpl(m_scope, m_path);
	ret->m_expr = m_expr;

	return ret;
}

}
