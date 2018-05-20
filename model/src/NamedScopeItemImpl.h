/*
 * NamedScopeItem.h
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
 *  Created on: May 3, 2016
 *      Author: ballance
 */

#pragma once
#include "INamedItem.h"
#include "IScopeItem.h"



class NamedScopeItemImpl : public virtual INamedItem, public IScopeItem {

public:
	NamedScopeItemImpl(IBaseItem::ItemType t, const std::string &name);

	virtual ~NamedScopeItemImpl();

	virtual IBaseItem::ItemType getType() const { return m_type; }

	virtual const std::string &getName() const { return m_name; }

	virtual const std::vector<IBaseItem *> &getItems() const { return m_children; }

	virtual void add(IBaseItem *item);

protected:
	IBaseItem::ItemType			m_type;
	std::string					m_name;
	std::vector<IBaseItem *>	m_children;

};

