/*
 * IImport.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IIMPORT_H_
#define MODEL_SRC_IIMPORT_H_
#include "IBaseItem.h"
#include "INamedItem.h"

using namespace psi_api;

class IImport : public virtual IBaseItem {
public:

	virtual ~IImport() { }

	virtual IBaseItem *getTargetType() const = 0;

	virtual bool isWildcard() const = 0;

};




#endif /* MODEL_SRC_IIMPORT_H_ */
