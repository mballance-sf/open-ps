/*
 * OPS.h
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
 *  Created on: Mar 5, 2018
 *      Author: ballance
 */

#ifndef OPS_SRC_OPS_H_
#define OPS_SRC_OPS_H_
#include <vector>
#include <string>
#include "IModel.h"

class OPS {
public:
	OPS();

	virtual ~OPS();

	void add_source_file(const std::string &file);

	bool parse();

private:
	std::vector<std::string>		m_source_files;
	IModel					*m_model;
};

#endif /* OPS_SRC_OPS_H_ */
