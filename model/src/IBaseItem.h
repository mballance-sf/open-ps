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
#include "ILocation.h"

namespace psi_api {
	class IItemFactory;

	/**
	 * Class: IBaseItem
	 *
	 * Common base API to PSI objects
	 */
	class IBaseItem : public virtual IAttributes {

	public:

		enum ItemType {
			TypeAction,
			TypeBind,
			TypeComponent,
			TypeConstraint,
			TypeCoverspec,
			TypeField,		// 5
			TypeImport,
			TypeEnum,
			TypeEnumerator,
			TypeExec,
			TypeExtend,		// 10
			TypeGraphStmt,
			TypeImportFunc,
			TypeModel,
			TypeObject,
			TypePackage,	// 15
			TypeScalar,
			TypeStruct,
			TypeSymbol,
			TypeRefType,
			TypeVendor
		};

		public:
			virtual ~IBaseItem() { }

			/**
			 * Method: getType()
			 * Returns the type of this object.
			 */
			virtual ItemType getType() const = 0;

			virtual IBaseItem *getParent() const = 0;

			/**
			 * Implementation: not called by user code
			 */
			virtual void setParent(IBaseItem *it) = 0;

			virtual IBaseItem *clone() const = 0;

			virtual const ILocation *getStart() const = 0;

			virtual void setStart(ILocation *start) = 0;
	};


}




#endif /* SRC_API_IBASEITEM_H_ */
