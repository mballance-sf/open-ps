/*
 * IEnumType.h
 *
 *  Created on: Mar 15, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IENUMTYPE_H_
#define MODEL_SRC_IENUMTYPE_H_
#include "IBaseItem.h"
#include "INamedItem.h"
#include <vector>
#include "IEnumerator.h"

using namespace psi_api;

class IEnumType : public virtual IBaseItem, public virtual INamedItem {
public:

	virtual const std::vector<IEnumerator *> &getEnumerators() const = 0;


};




#endif /* MODEL_SRC_IENUMTYPE_H_ */
