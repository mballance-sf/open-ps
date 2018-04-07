/*
 * IActivityDoActionStmt.h
 *
 *  Created on: Mar 21, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IACTIVITYDOACTIONSTMT_H_
#define MODEL_SRC_IACTIVITYDOACTIONSTMT_H_
#include "IActivityStmt.h"
#include "IBaseItem.h"
#include "IConstraintBlock.h"

using namespace psi_api;

class IActivityDoActionStmt : public virtual IActivityStmt {
public:

	virtual IBaseItem *getTargetType() const = 0;

	virtual IConstraintBlock *getConstraint() const = 0;

};




#endif /* MODEL_SRC_IACTIVITYDOACTIONSTMT_H_ */
