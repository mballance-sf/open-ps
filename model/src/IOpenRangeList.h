/*
 * IOpenRangeList.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IOPENRANGELIST_H_
#define MODEL_SRC_IOPENRANGELIST_H_
#include <vector>
#include "IOpenRangeValue.h"

class IOpenRangeList {
public:

	virtual ~IOpenRangeList() { }

	virtual const std::vector<IOpenRangeValue *> &ranges() const = 0;

	virtual IOpenRangeList *clone() const = 0;
};




#endif /* MODEL_SRC_IOPENRANGELIST_H_ */
