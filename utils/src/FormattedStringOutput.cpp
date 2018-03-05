/*
 * FormattedStringOutput.cpp
 *
 *  Created on: Aug 26, 2016
 *      Author: ballance
 */

#include "FormattedStringOutput.h"
#include <stdarg.h>
#include <stdio.h>

namespace qpssc {

FormattedStringOutput::FormattedStringOutput() {
	// TODO Auto-generated constructor stub

}

FormattedStringOutput::~FormattedStringOutput() {
	// TODO Auto-generated destructor stub
}

const std::string &FormattedStringOutput::content() const {
	return m_content;
}

void FormattedStringOutput::println(const std::string &l) {
	m_content.append(indent());
	m_content.append(l);
	m_content.push_back('\n');
}

void FormattedStringOutput::println(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(m_tmp, sizeof(m_tmp), fmt, ap);
	m_content.append(indent());
	m_content.append(m_tmp);
	m_content.push_back('\n');

	va_end(ap);
}

void FormattedStringOutput::print(const std::string &l) {
	m_content.append(l);
}

void FormattedStringOutput::print(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsprintf(m_tmp, fmt, ap);
	m_content.append(m_tmp);

	va_end(ap);
}

void FormattedStringOutput::flush() {
	// nop
}

} /* namespace qpssc */
