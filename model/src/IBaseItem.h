/*
 * IBaseItem.h
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

#ifndef SRC_API_IBASEITEM_H_
#define SRC_API_IBASEITEM_H_
#include "IAttributes.h"
#include "IStartLocation.h"
#include "IChildItem.h"

namespace psi_api {
	class IItemFactory;

	/**
	 * Class: IBaseItem
	 *
	 * Common base API to PSI objects
	 */
	class IBaseItem :
			public virtual IChildItem,
			public virtual IAttributes,
			public virtual IStartLocation {

	public:

		enum ItemType {
			TypeAction, // 0
			TypeActivityStmt,
			TypeArray,
			TypeBind,
			TypeComponent,
			TypeConstraint, // 5
			TypeCoverspec,
			TypeField,
			TypeImport,
			TypeEnum,
			TypeEnumerator, // 10
			TypeExec,
			TypeExtend,
			TypeGraphStmt,
			TypeImportFunc,
			TypeModel,		// 15
			TypeObject,
			TypePackage,
			TypePool,
			TypeScalar,
			TypeStruct,
			TypeSymbol,
			TypeRefType,	// 22
			TypeVendor
		};

		public:
			virtual ~IBaseItem() { }

			/**
			 * Method: getType()
			 * Returns the type of this object.
			 */
			virtual ItemType getType() const = 0;

			virtual IBaseItem *clone() const = 0;

	};


}




#endif /* SRC_API_IBASEITEM_H_ */
