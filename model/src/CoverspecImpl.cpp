/*
 * CoverspecImpl.cpp
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
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#include "CoverspecImpl.h"

CoverspecImpl::CoverspecImpl(const std::string &name) :
	BaseItemImpl(IBaseItem::TypeCoverspec), m_name(name) {

}

CoverspecImpl::~CoverspecImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *CoverspecImpl::clone() const {
	CoverspecImpl *ret = new CoverspecImpl(m_name);

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it!=getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	return ret;
}

