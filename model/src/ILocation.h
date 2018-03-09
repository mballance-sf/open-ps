/*
 * ILocation.h
 *
 *  Created on: Mar 6, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ILOCATION_H_
#define MODEL_SRC_ILOCATION_H_
#include <string>
#include <stdint.h>

class ILocation {
public:

	virtual ~ILocation() { }

	virtual uint32_t getLine() const = 0;

	virtual const std::string &getFile() const = 0;

};




#endif /* MODEL_SRC_ILOCATION_H_ */
