/*
 * StringTableBuilder.cpp
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

IStringTable *StringTableBuilder::build(
		IComponent		*c,
		IAction			*a) {
	m_strtab = new StringTable();
	visit_component(c);
	visit_action(a);

	return m_strtab;
}

void StringTableBuilder::visit_literal_expr(ILiteral *l) {
	if (l->getLiteralType() == ILiteral::LiteralString) {
		// See if we have a new string
		const std::string &str = l->getString();
		m_strtab->add(str);
	}
}

