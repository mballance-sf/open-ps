/*
 * ComponentImpl.cpp
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

#include "ComponentImpl.h"
#include "FieldImpl.h"

namespace psi {

ComponentImpl::ComponentImpl(const std::string &name) :
		BaseItemImpl(IBaseItem::TypeComponent),
		NamedItemImpl(name) {

}

ComponentImpl::~ComponentImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ComponentImpl::clone() const {
	ComponentImpl *ret = new ComponentImpl(getName());

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it!=getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	return ret;
}

} /* namespace psi */
