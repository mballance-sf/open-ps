/*
 * StringTableBuilder.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: ballance
 */

#include "StringTableBuilder.h"

StringTableBuilder::StringTableBuilder() {
	// TODO Auto-generated constructor stub

}

StringTableBuilder::~StringTableBuilder() {
	// TODO Auto-generated destructor stub
}

void StringTableBuilder::build(IModel *model) {
	visit_model(model);
}

void StringTableBuilder::build(IAction *action) {
	visit_action(action);
}

void StringTableBuilder::build(IComponent *component) {
	visit_component(component);
}

void StringTableBuilder::visit_literal_expr(ILiteral *l) {
	if (l->getLiteralType() == ILiteral::LiteralString) {
		// See if we have a new string
		const std::string &str = l->getString();
		std::map<std::string, uint32_t>::iterator fr = m_str2id.find(str);

		// This is new
		if (fr == m_str2id.end()) {
			uint32_t sz = m_str2id.size();
			m_str2id[str] = sz;
			m_id2str[sz] = str;
		}
	}
}

uint32_t StringTableBuilder::bits() const {
	uint32_t bits = 1;

	while ((1 << bits) < m_str2id.size()) {
		bits++;
	}

	return bits;
}
