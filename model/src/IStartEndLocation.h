/*
 * IEndLocation.h
 *
 *  Created on: Mar 22, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ISTARTENDLOCATION_H_
#define MODEL_SRC_ISTARTENDLOCATION_H_
#include "ILocation.h"
#include "IStartLocation.h"

class IStartEndLocation : public virtual IStartLocation {
public:
	virtual ~IStartEndLocation() { }

	virtual const ILocation *getEnd() const = 0;

	virtual void setEnd(ILocation *end) = 0;
};




#endif /* MODEL_SRC_ISTARTENDLOCATION_H_ */
