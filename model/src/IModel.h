/*
 * IModel.h
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
 *  Created on: Apr 26, 2016
 *      Author: ballance
 */

#pragma once
#include <vector>
#include <string>
#include <memory>

#include "IAction.h"
#include "IActivityTraverseStmt.h"
#include "IBaseItem.h"
#include "IBinaryExpr.h"
#include "IBind.h"
#include "IComponent.h"
#include "IConstraint.h"
#include "IConstraintBlock.h"
#include "IConstraintExpr.h"
#include "IConstraintIf.h"
#include "IExec.h"
#include "IExecExprStmt.h"
#include "IExtend.h"
#include "IField.h"
#include "IActivityBlockStmt.h"
#include "IActivityIfElseStmt.h"
#include "IActivityRepeatStmt.h"
#include "ILiteral.h"
#include "IPackage.h"
#include "IScalarType.h"
#include "IScopeItem.h"
#include "IStruct.h"
#include "ICallbackContext.h"
#include "IItemFactory.h"
#include "IMethodCallExpr.h"
#include "IRefType.h"
#include "IVariableRef.h"

	class IModel :
			public virtual IBaseItem,
			public virtual IScopeItem {
		public:

			virtual ~IModel() { }

			virtual IPackage *findPackage(const std::string &name, bool create=false) = 0;

			virtual IItemFactory *getItemFactory() = 0;

			virtual ICallbackContext *getCallbackContext() = 0;

	};

	typedef std::shared_ptr<IModel> IModelPtr;

