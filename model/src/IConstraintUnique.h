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
#include "IExpr.h"

using namespace psi_api;

class IConstraintUnique : public virtual IConstraint {
public:

	virtual ~IConstraintUnique() { }

	virtual const std::vector<IExpr *> &getTerms() const = 0;

};




#endif /* MODEL_SRC_ICONSTRAINTUNIQUE_H_ */