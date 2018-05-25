/*
 * FormattedOutputBase.cpp
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
 *  Created on: Aug 26, 2016
 *      Author: ballance
 */

#include "FormattedOutputBase.h"

namespace qpssc {

FormattedOutputBase::FormattedOutputBase() {
	m_indent_inc = 4;
}

FormattedOutputBase::~FormattedOutputBase() {
	// TODO Auto-generated destructor stub
}

const std::string &FormattedOutputBase::indent() const {
	return m_ind;
}


void FormattedOutputBase::inc_indent() {
	for (uint32_t i=0; i<m_indent_inc; i++) {
		m_ind.push_back(' ');
	}
}

void FormattedOutputBase::dec_indent() {
	if (m_ind.size() > m_indent_inc) {
		m_ind.resize(m_ind.size()-m_indent_inc);
	} else {
		m_ind.clear();
	}
}

} /* namespace qpssc */
