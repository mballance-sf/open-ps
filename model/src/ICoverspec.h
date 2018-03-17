/*
 * ICoverspec.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ICOVERSPEC_H_
#define MODEL_SRC_ICOVERSPEC_H_
#include "IBaseItem.h"
#include "INamedItem.h"
#include "IScopeItem.h"

using namespace psi_api;

class ICoverspec : public virtual IBaseItem, public virtual INamedItem, public virtual IScopeItem {
public:

	virtual ~ICoverspec() { }

};




#endif /* MODEL_SRC_ICOVERSPEC_H_ */
