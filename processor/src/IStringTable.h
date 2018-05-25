/*
 * IStringTable.h
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
#include <stdint.h>
#include <string>
#include <memory>

class IStringTable {
public:

	virtual ~IStringTable() { }

	virtual uint32_t str2id(const std::string &str) const = 0;

	virtual const std::string &id2str(uint32_t id) const = 0;

	virtual uint32_t size() const = 0;

	virtual uint32_t bits() const = 0;

};

typedef std::shared_ptr<IStringTable> IStringTableH;


