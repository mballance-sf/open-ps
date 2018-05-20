/*
 * ImportImpl.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IMPORTIMPL_H_
#define MODEL_SRC_IMPORTIMPL_H_

#include "IImport.h"
#include "BaseItemImpl.h"



class ImportImpl: public BaseItemImpl, public virtual IImport {
public:

	ImportImpl(IBaseItem *target, bool is_wildcard);

	virtual ~ImportImpl();

	virtual IBaseItem *getTargetType() const { return m_target; }

	virtual bool isWildcard() const { return m_is_wildcard; }

	virtual IBaseItem *clone() const;

private:
	IBaseItem					*m_target;
	bool						m_is_wildcard;
};

#endif /* MODEL_SRC_IMPORTIMPL_H_ */
