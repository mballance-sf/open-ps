/*
 * IExtendComposite.h
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IEXTENDCOMPOSITE_H_
#define MODEL_SRC_IEXTENDCOMPOSITE_H_
#include "IExtend.h"
#include "IScopeItem.h"

using namespace psi_api;

class IExtendComposite :
		public virtual IExtend,
		public virtual IScopeItem {
public:

	virtual ~IExtendComposite() { }

};




#endif /* MODEL_SRC_IEXTENDCOMPOSITE_H_ */
