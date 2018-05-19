/*
 * StringTable.cpp
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

