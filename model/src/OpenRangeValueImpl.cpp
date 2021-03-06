/*
 * OpenRangeValueImpl.cpp
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
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#include "OpenRangeValueImpl.h"

OpenRangeValueImpl::OpenRangeValueImpl(
		IExpr				*lhs,
		IExpr				*rhs,
		bool				domain_bound) :
		m_lhs(lhs), m_rhs(rhs), m_domain_bound(domain_bound) {

}

OpenRangeValueImpl::~OpenRangeValueImpl() {
	// TODO Auto-generated destructor stub
}

