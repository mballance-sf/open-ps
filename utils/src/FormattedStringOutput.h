/*
 * FormattedStringOutput.h
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
