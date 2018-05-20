/*
 * Z3ModelBuilder.cpp
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#include "Z3ModelBuilder.h"

Z3ModelBuilder::Z3ModelBuilder() : m_prefix_valid(false) {
	m_expr_builder = new Z3ModelExprBuilder(this);


}

Z3ModelBuilder::~Z3ModelBuilder() {
	delete m_expr_builder;
}

void Z3ModelBuilder::push_prefix(const std::string &pref) {
	m_prefix_v.push_back(pref);
	m_prefix_valid = false;
}

void Z3ModelBuilder::pop_prefix() {
	m_prefix_v.pop_back();
	m_prefix_valid = false;
}

const std::string &Z3ModelBuilder::prefix() {

	if (!m_prefix_valid) {
		m_prefix.clear();
		for (uint32_t i=0; i<m_prefix_v.size(); i++) {
			m_prefix.append(m_prefix_v.at(i));
			if (i+1 < m_prefix_v.size()) {
				m_prefix.append(".");
			}
		}
		m_prefix_valid = true;
	}

	return m_prefix;
}


