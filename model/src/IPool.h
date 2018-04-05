/*
 * IPool.h
 *
 *  Created on: Apr 4, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IPOOL_H_
#define MODEL_SRC_IPOOL_H_
#include "IField.h"

using namespace psi_api;

class IPool : public virtual IField {
public:

	virtual ~IPool() { }

	// Returns the pool size
	virtual IExpr *getSize() const = 0;

};




#endif /* MODEL_SRC_IPOOL_H_ */
