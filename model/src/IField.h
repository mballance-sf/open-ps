/*
 * IField.h
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
 *  Created on: May 5, 2016
 *      Author: ballance
 */

#pragma once
#include <stdint.h>
#include <string>
#include "IBaseItem.h"
#include "INamedItem.h"
#include "IExpr.h"

class IField :
		public virtual IBaseItem,
		public virtual INamedItem {
public:

	// Kind: None, Rand, Input, Inout, Output, Pool, Lock, Share, Action, Comp
	// Protection: Public|Protected|Private
	// Attributes: built-in

	enum FieldAttr {
		FieldAttr_None = 0,
		FieldAttr_Rand,
		FieldAttr_Input,
		FieldAttr_Inout,
		FieldAttr_Lock,
		FieldAttr_Output,
		FieldAttr_Pool,
		FieldAttr_Share,
		FieldAttr_Action,
		FieldAttr_Comp
	};

	enum Visibility {
		Visibility_Public,
		Visibility_Protected,
		Visibility_Private
	};

public:


	virtual ~IField() { }

	virtual IBaseItem *getDataType() const = 0;

	virtual void setDataType(IBaseItem *type) = 0;

	virtual FieldAttr getAttr() const = 0;

	virtual IExpr *getArrayDim() const = 0;

};

