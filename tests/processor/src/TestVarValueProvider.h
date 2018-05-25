/*
 * TestVarValueProvider.h
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
 *  Created on: Apr 28, 2018
 *      Author: ballance
 */
#pragma once

#include "IVarValueProvider.h"
#include "VarVal.h"
#include <map>
#include <string>

class TestVarValueProvider : public virtual IVarValueProvider {
public:
	TestVarValueProvider(const std::map<std::string, VarVal> &var_map);

	virtual ~TestVarValueProvider();

	void set_value(const std::string &k, const std::string &v);

	void set_value(const std::string &k, uint32_t v);

	virtual VarVal get_value(const std::string &path);

private:
	std::map<std::string, VarVal> 		m_var_map;


};

