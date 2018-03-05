/*
 * FormattedOutputBase.cpp
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
