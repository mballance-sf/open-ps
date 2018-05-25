/*
 * StringTableBuilder.cpp
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
 *  Created on: Mar 27, 2018
 *      Author: ballance
 */

#include "StringTableBuilder.h"
#include "StringTable.h"

StringTableBuilder::StringTableBuilder() {
	m_strtab = 0;
}

StringTableBuilder::~StringTableBuilder() {
}

IStringTableH StringTableBuilder::build(
		IComponent		*c,
		IAction			*a) {
	m_strtab = new StringTable();
	visit_component(c);
	visit_action(a);

	return IStringTableH(m_strtab);
}

void StringTableBuilder::visit_literal_expr(ILiteral *l) {
	if (l->getLiteralType() == ILiteral::LiteralString) {
		// See if we have a new string
		const std::string &str = l->getString();
		m_strtab->add(str);
	}
}

