/*
 * IConstraintUnique.h
 *
 *  Created on: Mar 31, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ICONSTRAINTUNIQUE_H_
#define MODEL_SRC_ICONSTRAINTUNIQUE_H_
#include <vector>
#include "IConstraint.h"
#include "IOpenRangeList.h"
#include "IExpr.h"



class IConstraintUnique : public virtual IConstraint {
public:

	virtual ~IConstraintUnique() { }

	virtual IOpenRangeList *getTarget() const = 0;

};




#endif /* MODEL_SRC_ICONSTRAINTUNIQUE_H_ */
