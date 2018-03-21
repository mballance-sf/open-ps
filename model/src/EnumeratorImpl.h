/*
 * EnumeratorImpl.h
 *
 *  Created on: Mar 16, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ENUMERATORIMPL_H_
#define MODEL_SRC_ENUMERATORIMPL_H_
#include "BaseItemImpl.h"
#include "IEnumerator.h"

using namespace psi;

class EnumeratorImpl: public BaseItemImpl, public virtual IEnumerator {
public:

	EnumeratorImpl(
			const std::string		&name,
			IExpr					*value);

	virtual ~EnumeratorImpl();

	virtual const std::string &getName() const { return m_name; }

	virtual IExpr *getValue() const { return m_value; }

	virtual IBaseItem *clone() const;

private:
	std::string						m_name;
	IExpr							*m_value;
};

#endif /* MODEL_SRC_ENUMERATORIMPL_H_ */
