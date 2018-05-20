/*
 * ConstraintImpliesImpl.h
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
 *  Created on: Aug 12, 2016
 *      Author: ballance
 */

#pragma once
#include "IConstraintImplies.h"
#include "BaseItemImpl.h"



class ConstraintImpliesImpl:
		public virtual IConstraintImplies,
		public virtual BaseItemImpl {
public:
	ConstraintImpliesImpl(
			IExpr			*cond,
			IConstraint		*imp
			);

	virtual ~ConstraintImpliesImpl();

	virtual ConstraintType getConstraintType() const {
		return IConstraint::ConstraintType_Implies;
	}

	virtual IExpr *getCond() const { return m_cond; }

	virtual IConstraint *getImp() const { return m_imp; }

	virtual IBaseItem *clone() const;

private:

	IExpr					*m_cond;
	IConstraint				*m_imp;
};


