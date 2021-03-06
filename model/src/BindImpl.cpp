/*
 * BindImpl.cpp
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
 *  Created on: May 23, 2016
 *      Author: ballance
 */

#include "BindImpl.h"

BindImpl::BindImpl(const std::vector<IBindPath *> &targets) :
		BaseItemImpl(IBaseItem::TypeBind), m_targets(targets) {

}

BindImpl::~BindImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *BindImpl::clone() const {
	std::vector<IBindPath *> targets;

	for (std::vector<IBindPath *>::const_iterator it=getTargets().begin();
			it!=getTargets().end(); it++) {
		targets.push_back(dynamic_cast<IBindPath *>((*it)->clone()));
	}

	return new BindImpl(targets);
}
