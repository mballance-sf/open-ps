/*
 * StringTable.cpp
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
 *  Created on: May 19, 2018
 *      Author: ballance
 */

#include "StringTable.h"

StringTable::StringTable() {
	// TODO Auto-generated constructor stub

}

StringTable::~StringTable() {
	// TODO Auto-generated destructor stub
}

void StringTable::add(const std::string &str) {
	std::map<std::string, uint32_t>::iterator fr = m_str2id.find(str);

	// This is new
	if (fr == m_str2id.end()) {
		uint32_t sz = m_str2id.size();
		m_str2id[str] = sz;
		m_id2str[sz] = str;
	}
}

uint32_t StringTable::bits() const {
	uint32_t bits = 1;

	while ((1 << bits) < m_str2id.size()) {
		bits++;
	}

	return bits;
}

