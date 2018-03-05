/*
 * NamedScopeItem.cpp
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
 *  Created on: May 3, 2016
 *      Author: ballance
 */

#include "NamedScopeItemImpl.h"

namespace psi {

NamedScopeItemImpl::NamedScopeItemImpl(IBaseItem::ItemType t, const std::string &name) :
		m_type(t), m_name(name) {
	// TODO Auto-generated constructor stub

}

NamedScopeItemImpl::~NamedScopeItemImpl() {
	// TODO Auto-generated destructor stub
}

void NamedScopeItemImpl::add(IBaseItem *item) {
	if (item) {
		m_children.push_back(item);
		item->setParent(dynamic_cast<IBaseItem *>(this));
	}
}

}
