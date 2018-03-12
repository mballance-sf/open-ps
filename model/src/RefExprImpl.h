/*
 * RefExprImpl.h
 *
 *  Created on: Mar 2, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_REFEXPRIMPL_H_
#define MODEL_SRC_REFEXPRIMPL_H_
#include "IRefExpr.h"

namespace psi_api {

class RefExprImpl: public virtual psi_api::IRefExpr {
public:
	RefExprImpl(
			IScopeItem						*scope,
			const std::vector<std::string>	&path);

	virtual ExprType getType() const { return IExpr::ExprType_Ref; }

	virtual ~RefExprImpl();

	virtual const std::vector<std::string> &getPath() const { return m_path; }

	virtual IExpr *getExpr() const;

	virtual void setExpr(IExpr *expr);

	virtual IExpr *clone() const;

private:
	IScopeItem					*m_scope;
	std::vector<std::string>	m_path;
	IExpr						*m_expr;

};

}

#endif /* MODEL_SRC_REFEXPRIMPL_H_ */
