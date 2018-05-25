/*
 * FormattedFileOutput.cpp
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

#include "FormattedFileOutput.h"
#include <stdarg.h>

namespace qpssc {

FormattedFileOutput::FormattedFileOutput(FILE *fp) {
	m_fp = fp;
}

FormattedFileOutput::~FormattedFileOutput() {
	// TODO Auto-generated destructor stub
}

void FormattedFileOutput::println(const std::string &l) {
	fputs(indent().c_str(), m_fp);
	fputs(l.c_str(), m_fp);
	fputs("\n", m_fp);
}

void FormattedFileOutput::println(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);

	fputs(indent().c_str(), m_fp);
	vfprintf(m_fp, fmt, ap);
	fputs("\n", m_fp);

	va_end(ap);
}

void FormattedFileOutput::print(const std::string &l) {
	fputs(l.c_str(), m_fp);
}

void FormattedFileOutput::print(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vfprintf(m_fp, fmt, ap);
	va_end(ap);
}

void FormattedFileOutput::flush() {
	fflush(m_fp);
}


} /* namespace qpssc */
