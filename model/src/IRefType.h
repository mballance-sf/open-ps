/*
 * IRefType.h
 *
 *  Created on: Mar 9, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IREFTYPE_H_
#define MODEL_SRC_IREFTYPE_H_
#include <vector>
#include <string>
#include "IBaseItem.h"
#include "IScopeItem.h"

using namespace psi_api;

class IRefType : public virtual IBaseItem {
public:

	virtual ~IRefType() { }

	virtual IScopeItem *getScope() const = 0;

	virtual const std::vector<std::string> &getTypeId() const = 0;

	virtual IBaseItem *getTargetType() const = 0;

	virtual void setTargetType(IBaseItem *) = 0;

	virtual std::string toString() const = 0;

};




#endif /* MODEL_SRC_IREFTYPE_H_ */
