/*
 * FormattedFileOutput.h
 *
 *  Created on: Aug 26, 2016
 *      Author: ballance
 */

#ifndef SRC_FORMATTEDFILEOUTPUT_H_
#define SRC_FORMATTEDFILEOUTPUT_H_
#include <stdio.h>
#include "FormattedOutputBase.h"

namespace qpssc {

class FormattedFileOutput: public FormattedOutputBase {
public:
	FormattedFileOutput(FILE *fp);

	virtual ~FormattedFileOutput();

	virtual void println(const std::string &l);

	virtual void println(const char *fmt, ...);

	virtual void print(const std::string &l);

	virtual void print(const char *fmt, ...);

	virtual void flush();

private:
	FILE				*m_fp;

};

} /* namespace qpssc */

#endif /* SRC_FORMATTEDFILEOUTPUT_H_ */
