/*
 * OpenRangeValueImpl.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_OPENRANGEVALUEIMPL_H_
#define MODEL_SRC_OPENRANGEVALUEIMPL_H_

#include "IOpenRangeValue.h"

class OpenRangeValueImpl: public virtual IOpenRangeValue {
public:

	OpenRangeValueImpl(
			IExpr		*lhs,
			IExpr		*rhs,
			bool		domain_bound
			);

	virtual ~OpenRangeValueImpl();

	virtual IExpr *getLHS() const { return m_lhs; }

	virtual IExpr *getRHS() const { return m_rhs; }

	virtual bool isDomainBound() const { return m_domain_bound; }


private:
	IExpr						*m_lhs;
	IExpr						*m_rhs;
	bool						m_domain_bound;
};

#endif /* MODEL_SRC_OPENRANGEVALUEIMPL_H_ */
