/*
 * IRefExpr.h
 *
 *  Created on: Mar 2, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IREFEXPR_H_
#define MODEL_SRC_IREFEXPR_H_
#include "IBaseItem.h"
#include "IExpr.h"
#include <vector>
#include <string>

namespace psi_api {
class IRefExpr : public virtual IExpr {
public:

	virtual ~IRefExpr() { }

	virtual const std::vector<std::string> &getPath() const = 0;

	virtual void setExpr(IExpr *expr) = 0;

};
}




#endif /* MODEL_SRC_IREFEXPR_H_ */
