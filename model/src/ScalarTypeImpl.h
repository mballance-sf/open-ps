/*
 * ScalarTypeImpl.h
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
 *
 *  Created on: May 6, 2016
 *      Author: ballance
 */

#pragma once
#include "IScalarType.h"
#include "BaseItemImpl.h"



class ScalarTypeImpl:
		public virtual IScalarType,
		public virtual BaseItemImpl {
public:
	ScalarTypeImpl(
			IScalarType::ScalarType			scalar_type,
			IExpr							*msb,
			IExpr							*lsb,
			IOpenRangeList					*domain);

	virtual ~ScalarTypeImpl();

	virtual ScalarType getScalarType() const { return m_scalarType; }

	/**
	 * Returns the MSB of the type for pss_bit and pss_int types
	 */
	virtual IExpr *getMSB() const { return m_msb; }

	/**
	 * Returns the LSB of the type for pss_bit and pss_int types
	 */
	virtual IExpr *getLSB() const { return m_lsb; }

	virtual IOpenRangeList *getDomain() const { return m_domain; }

	virtual IBaseItem *clone() const;


private:
	IScalarType::ScalarType			m_scalarType;
	IExpr							*m_msb;
	IExpr							*m_lsb;
	IOpenRangeList					*m_domain;
};

