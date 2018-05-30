/*
 * StructImpl.cpp
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
 *  Created on: May 3, 2016
 *      Author: ballance
 */

#include "StructImpl.h"
#include "FieldImpl.h"
#include "ScalarTypeImpl.h"
#include "LiteralImpl.h"
#include "RefTypeImpl.h"

StructImpl::StructImpl(
		const std::string 		&name,
		IStruct::StructType		t,
		IBaseItem 				*super_type) :
				BaseItemImpl(IBaseItem::TypeStruct),
				NamedItemImpl(name),
				m_struct_type(t), m_super_type(super_type) {

	if (t == IStruct::State) {
		std::vector<std::string> type;
		type.push_back(name);

		RefTypeImpl *this_t = new RefTypeImpl(0, type);
		this_t->setTargetType(this);

		IField *field = new FieldImpl("initial",
				new ScalarTypeImpl(IScalarType::ScalarType_Bool, 0, 0, 0),
				IField::FieldAttr_Rand, 0);
		IField *prev = new FieldImpl("prev",
				this_t, IField::FieldAttr_Rand, 0);

		add(field);
		add(prev);
	} else if (t == IStruct::Resource) {
		IField *field = new FieldImpl("instance_id",
				new ScalarTypeImpl(IScalarType::ScalarType_Bit,
						0, new LiteralImpl((uint64_t)32), 0),
				IField::FieldAttr_Rand, 0);
		add(field);
	}

}

StructImpl::~StructImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *StructImpl::clone() const {
	StructImpl *ret = new StructImpl(getName(),
			getStructType(),
			getSuperType());

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it!=getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	return ret;
}

