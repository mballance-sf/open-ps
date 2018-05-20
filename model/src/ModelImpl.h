/*
 * ModelImpl.h
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
 *  Created on: Apr 26, 2016
 *      Author: ballance
 */

#pragma once
#include "PackageImpl.h"
#include "ItemFactoryImpl.h"
#include "ILiteral.h"
#include "IModel.h"
#include "IStruct.h"



class ModelImpl:
		public virtual IModel,
		public virtual BaseItemImpl {

	public:
		ModelImpl();

		virtual ~ModelImpl();

//		virtual const std::vector<IPackage*> &getPackages();

		virtual IBaseItem::ItemType getType() const {
			return IBaseItem::TypeModel;
		}

		virtual IModel *clone() const;

		virtual IBaseItem *clone(IBaseItem *item);

		virtual IBaseItem *getParent() const { return 0; }

		void setParent(IBaseItem *p) { }

		virtual const std::vector<IBaseItem *> &getItems() const;

		virtual void remove(IBaseItem *it);

		virtual void add(IBaseItem *it);

		virtual const ILocation *getStart() const { return 0; }

		virtual void setStart(ILocation *start)  { }

		virtual const ILocation *getEnd() const { return 0; }

		virtual void setEnd(ILocation *start)  { }

		virtual IPackage *findPackage(const std::string &name, bool create=false);

		virtual IItemFactory *getItemFactory() { return &m_factory; }

		virtual ICallbackContext *getCallbackContext();

		virtual void setCallbackContext(ICallbackContext *ctxt);

		/**
		 * Null implementation
		 */
		virtual IField *getField(const std::string &name) { return 0; }

		virtual const std::string &getAttribute(const std::string &key);

		virtual void getAttributes(std::vector<std::string> &keys);

		virtual bool hasAttribute(const std::string &key);

		virtual void setAttribute(const std::string &key, const std::string &val);

		virtual void clearAttribute(const std::string &key);


	private:
		ItemFactoryImpl				m_factory;
		std::vector<IBaseItem *>	m_children;
		PackageImpl					m_global_pkg;
		ICallbackContext			*m_callback_ctxt;
		std::string					m_null_attr;


};

