/*
 * RefExprImpl.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: ballance
 */

#include "RefExprImpl.h"

namespace psi_api {

RefExprImpl::RefExprImpl(
		IBaseItem						*scope,
		const std::vector<std::string>	&path) :
				m_scope(scope), m_path(path), m_expr_ref(0) {
	// TODO Auto-generated constructor stub

}

RefExprImpl::~RefExprImpl() {
	// TODO Auto-generated destructor stub
}

void RefExprImpl::setExprRef(IExpr **ref) {
	m_expr_ref = ref;
}

void RefExprImpl::setExpr(IExpr *expr) {
	// Replace the reference
	*m_expr_ref = expr;
}

IExpr *RefExprImpl::clone() const {
	RefExprImpl *ret = new RefExprImpl(m_scope, m_path);
	ret->setExprRef(m_expr_ref);

	return ret;
}

}
