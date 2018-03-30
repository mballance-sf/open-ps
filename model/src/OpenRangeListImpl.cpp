/*
 * OpenRangeListImpl.cpp
 *
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#include "OpenRangeListImpl.h"

OpenRangeListImpl::OpenRangeListImpl(
		const std::vector<IOpenRangeValue *>	&ranges) : m_ranges(ranges) {
	// TODO Auto-generated constructor stub

}

OpenRangeListImpl::~OpenRangeListImpl() {
	// TODO Auto-generated destructor stub
}

IOpenRangeList *OpenRangeListImpl::clone() const {
	// TODO: should clone ranges
	return new OpenRangeListImpl(m_ranges);
}
