/*
 * IStartLocation.h
 *
 *  Created on: Mar 22, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ISTARTLOCATION_H_
#define MODEL_SRC_ISTARTLOCATION_H_
#include "ILocation.h"

class IStartLocation {
public:
	virtual ~IStartLocation() { }

	virtual const ILocation *getStart() const = 0;

	virtual void setStart(ILocation *start) = 0;
};




#endif /* MODEL_SRC_ISTARTLOCATION_H_ */
