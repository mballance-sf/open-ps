/*
 * VarVal.cpp
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
#include "VarVal.h"

VarVal::VarVal() {
	si = -1;
	type = VarVal_Int;
	w = 0;
//	s = "unknown";
}

VarVal::VarVal(uint64_t v, uint32_t w) {
	ui = v;
	type = VarVal_Uint;
	this->w = w;
}

VarVal::VarVal(int64_t v, uint32_t w) {
	si = v;
	type = VarVal_Int;
	this->w = w;
}

VarVal::VarVal(bool v) {
	fprintf(stdout, "VarVal(bool)\n");
	b = v;
	type = VarVal_Bool;
	this->w = 1;
}

VarVal::VarVal(const std::string &v) {
	s = v;
	type = VarVal_String;
	w = 0;
	fprintf(stdout, "VarVal(string) type=%d\n", type);
}

VarVal::VarVal(const char *v) {
	s = v;
	type = VarVal_String;
	w = 0;
	fprintf(stdout, "VarVal(string) type=%d\n", type);
}

VarVal::VarVal(const VarVal &rhs) {
	type = rhs.type;
	w = rhs.w;
	switch (type) {
	case VarVal_Bool:	b  = rhs.b;  break;
	case VarVal_Int:	si = rhs.si; break;
	case VarVal_Uint:	ui = rhs.ui; break;
	case VarVal_String:	s  = rhs.s; break;
	}
}

void VarVal::operator = (const VarVal &rhs) {
	type = rhs.type;
	w = rhs.w;
	switch (type) {
	case VarVal_Bool:	b  = rhs.b;  break;
	case VarVal_Int:	si = rhs.si; break;
	case VarVal_Uint:	ui = rhs.ui; break;
	case VarVal_String:	s  = rhs.s;  break;
	}
}



