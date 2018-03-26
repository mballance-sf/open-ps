/*
 * IChildItem.h
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ICHILDITEM_H_
#define MODEL_SRC_ICHILDITEM_H_

namespace psi_api {
class IBaseItem;
}

using namespace psi_api;

class IChildItem {
public:
	virtual ~IChildItem() { }

	virtual IBaseItem *getParent() const = 0;

	/**
	 * Implementation: not called by user code
	 */
	virtual void setParent(IBaseItem *it) = 0;

};




#endif /* MODEL_SRC_ICHILDITEM_H_ */
