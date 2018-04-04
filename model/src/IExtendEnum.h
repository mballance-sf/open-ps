/*
 * IExtendEnum.h
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IEXTENDENUM_H_
#define MODEL_SRC_IEXTENDENUM_H_
#include "IExtend.h"
#include "IEnumerator.h"

using namespace psi_api;

class IExtendEnum : public virtual IExtend {
public:

	virtual ~IExtendEnum() { }

	virtual const std::vector<IEnumerator *> &getEnumerators() const = 0;

};




#endif /* MODEL_SRC_IEXTENDENUM_H_ */
