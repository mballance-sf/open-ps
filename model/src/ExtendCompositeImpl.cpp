/*
 * ExtendCompositeImpl.cpp
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
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#include "ExtendCompositeImpl.h"

ExtendCompositeImpl::ExtendCompositeImpl(
		IExtend::ExtendType		type,
		IBaseItem				*target) :
		BaseItemImpl(IBaseItem::TypeExtend),
		m_extend_type(type),
		m_target(target) {

}

ExtendCompositeImpl::~ExtendCompositeImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ExtendCompositeImpl::clone() const {
	ExtendCompositeImpl *ret = new ExtendCompositeImpl(getExtendType(), getTarget());

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it!=getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	return ret;
}

