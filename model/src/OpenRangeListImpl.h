/*
 * OpenRangeListImpl.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_OPENRANGELISTIMPL_H_
#define MODEL_SRC_OPENRANGELISTIMPL_H_

#include "IOpenRangeList.h"

class OpenRangeListImpl: public virtual IOpenRangeList {
public:
	OpenRangeListImpl(
			const std::vector<IOpenRangeValue *>	&ranges);

	virtual ~OpenRangeListImpl();

	virtual const std::vector<IOpenRangeValue *> &ranges() const {
		return m_ranges;
	}

	virtual IOpenRangeList *clone() const;

private:
	std::vector<IOpenRangeValue *>				m_ranges;

};

#endif /* MODEL_SRC_OPENRANGELISTIMPL_H_ */
