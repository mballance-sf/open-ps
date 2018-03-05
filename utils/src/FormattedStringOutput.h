/*
 * FormattedStringOutput.h
 *
 *  Created on: Aug 26, 2016
 *      Author: ballance
 */

#ifndef SRC_FORMATTEDSTRINGOUTPUT_H_
#define SRC_FORMATTEDSTRINGOUTPUT_H_

#include "FormattedOutputBase.h"

namespace qpssc {

class FormattedStringOutput: public FormattedOutputBase {
public:
	FormattedStringOutput();

	virtual ~FormattedStringOutput();

	virtual const std::string &content() const;

	virtual void println(const std::string &l);

	virtual void println(const char *fmt, ...);

	virtual void print(const std::string &l);

	virtual void print(const char *fmt, ...);

	virtual void flush();

private:
	std::string			m_content;
	char				m_tmp[4096];

};

} /* namespace qpssc */

#endif /* SRC_FORMATTEDSTRINGOUTPUT_H_ */
