/*
 * CoverspecImpl.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_COVERSPECIMPL_H_
#define MODEL_SRC_COVERSPECIMPL_H_
#include "BaseItemImpl.h"
#include "ScopeItemImpl.h"
#include "ICoverspec.h"



class CoverspecImpl: public virtual BaseItemImpl, public virtual ScopeItemImpl, public virtual ICoverspec {
public:
	CoverspecImpl(const std::string &name);

	virtual ~CoverspecImpl();

	virtual const std::string &getName() const { return m_name; }

	virtual IBaseItem *clone() const;


private:
	std::string					m_name;
};

#endif /* MODEL_SRC_COVERSPECIMPL_H_ */
