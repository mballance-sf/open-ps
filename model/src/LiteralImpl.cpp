/*
 * LiteralImpl.cpp
 *
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
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
 *
 *  Created on: May 4, 2016
 *      Author: ballance
 */

#include "LiteralImpl.h"


LiteralImpl::LiteralImpl(int64_t v) :
		m_literalType(LiteralInt), m_intval(v) { }

LiteralImpl::LiteralImpl(uint64_t v) :
		m_literalType(LiteralBit), m_intval(v) { }

LiteralImpl::LiteralImpl(bool v) :
		m_literalType(LiteralBool), m_intval(v) { }

LiteralImpl::LiteralImpl(const std::string &s) :
		m_literalType(LiteralString), m_intval(0), m_strval(s) { }

LiteralImpl::~LiteralImpl() {
	// TODO Auto-generated destructor stub
}

IExpr *LiteralImpl::clone() const {
	switch (getLiteralType()) {
	case ILiteral::LiteralBit:
		return new LiteralImpl(getBit());
	case ILiteral::LiteralInt:
		return new LiteralImpl(getInt());
	case ILiteral::LiteralBool:
		return new LiteralImpl(getBool());
	case ILiteral::LiteralString:
		return new LiteralImpl(getString());
	}
	return 0;
}

