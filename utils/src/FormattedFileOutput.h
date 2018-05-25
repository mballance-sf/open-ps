/*
 * FormattedFileOutput.h
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
