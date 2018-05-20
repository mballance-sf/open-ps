/*
 * IChildItem.h
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#pragma once

class IBaseItem;
class IChildItem {
public:
	virtual ~IChildItem() { }

	virtual IBaseItem *getParent() const = 0;

	/**
	 * Implementation: not called by user code
	 */
	virtual void setParent(IBaseItem *it) = 0;

};



