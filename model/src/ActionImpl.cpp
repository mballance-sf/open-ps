/*
 * ActionImpl.cpp
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

#include "ActionImpl.h"
#include "FieldImpl.h"

ActionImpl::ActionImpl(
		const std::string 	&name,
		IBaseItem 			*super_type) :
				BaseItemImpl(IBaseItem::TypeAction),
				NamedItemImpl(name), m_super_type(super_type), m_graph(0) {
	m_comp = new FieldImpl("comp", 0, IField::FieldAttr_Comp, 0);
	add(m_comp);
}

ActionImpl::~ActionImpl() {
	// TODO Auto-generated destructor stub
}

void ActionImpl::setActivity(IActivityStmt *activity) {
	m_graph = activity;
}

void ActionImpl::setParent(IBaseItem *it) {
	BaseItemImpl::setParent(it);
	if (dynamic_cast<IComponent *>(it)) {
		dynamic_cast<FieldImpl *>(m_comp)->setDataType(it);
	}
}
IBaseItem *ActionImpl::clone() const {
	ActionImpl *ret = new ActionImpl(getName(), getSuperType());

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it!=getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	if (getActivity()) {
		ret->setActivity(getActivity()->clone());
	}

	return ret;
}

