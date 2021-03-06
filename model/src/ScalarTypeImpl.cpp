/*
 * ScalarTypeImpl.cpp
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
 * 
 *  Created on: May 6, 2016
 *      Author: ballance
 */

#include "ScalarTypeImpl.h"
#include "IExpr.h"

ScalarTypeImpl::ScalarTypeImpl(
		IScalarType::ScalarType			scalar_type,
		IExpr							*msb,
		IExpr							*lsb,
		IOpenRangeList					*domain) :
				BaseItemImpl(IBaseItem::TypeScalar),
				m_scalarType(scalar_type), m_msb(msb), m_lsb(lsb),
				m_domain(domain) { }

ScalarTypeImpl::~ScalarTypeImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ScalarTypeImpl::clone() const {
	return new ScalarTypeImpl(
			getScalarType(),
			(getMSB())?getMSB()->clone():0,
			(getLSB())?getMSB()->clone():0,
			(getDomain()?getDomain()->clone():0));
}

