/*
 * EnumeratorImpl.cpp
 *
 *  Created on: Mar 16, 2018
 *      Author: ballance
 */

#include "EnumeratorImpl.h"

EnumeratorImpl::EnumeratorImpl(
		const std::string		&name,
		IExpr					*value) : m_name(name), m_value(value) {

}

EnumeratorImpl::~EnumeratorImpl() {
	// TODO Auto-generated destructor stub
}

