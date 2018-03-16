/*
 * RefTypeImpl.h
 *
 *  Created on: Mar 9, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_REFTYPEIMPL_H_
#define MODEL_SRC_REFTYPEIMPL_H_

#include "IRefType.h"
#include "BaseItemImpl.h"


class RefTypeImpl: public psi::BaseItemImpl,
	public virtual IRefType {
public:
	RefTypeImpl(
			IScopeItem 						*scope,
			const std::vector<std::string>	&path);

	virtual ~RefTypeImpl();

	virtual IScopeItem *getScope() const;

	virtual const std::vector<std::string> &getTypeId() const;

	virtual IBaseItem *getTargetType() const;

	virtual void setTargetType(IBaseItem *t);

	virtual IBaseItem *clone() const;

	virtual std::string toString() const;

private:

	IScopeItem					*m_scope;
	std::vector<std::string>	m_typeid;
	IBaseItem					*m_target;

};

#endif /* MODEL_SRC_REFTYPEIMPL_H_ */
