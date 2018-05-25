/*
 * FormattedStringOutput.cpp
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
