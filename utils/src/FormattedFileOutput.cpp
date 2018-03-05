/*
 * FormattedFileOutput.cpp
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
