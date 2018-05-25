/*
 * VarVal.h
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

typedef enum {
	VarVal_Uint,
	VarVal_Int,
	VarVal_Bool,
	VarVal_String
} VarValType;

class VarVal {
public:

	VarVal();

	VarVal(uint64_t v, uint32_t w=64);

	VarVal(int64_t v, uint32_t w=64);

	explicit VarVal(bool v);

	VarVal(const std::string &v);

	VarVal(const char *v);

	VarVal(const VarVal &rhs);

	virtual ~VarVal() { }

	union {
		uint64_t		ui;
		int64_t			si;
		bool			b;
	};
	std::string			s;

	void operator = (const VarVal &rhs);

	VarValType		type;
	uint32_t		w;
};


