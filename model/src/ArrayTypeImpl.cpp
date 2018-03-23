/*
 * ArrayTypeImpl.cpp
 *
 *  Created on: Mar 21, 2018
 *      Author: ballance
 */

#include "ArrayTypeImpl.h"
#include "FieldImpl.h"
#include "ScalarTypeImpl.h"
#include "LiteralImpl.h"

using namespace psi;

ArrayTypeImpl::ArrayTypeImpl(
		IBaseItem				*target,
		bool					has_sum,
		IExpr					*lhs,
		IExpr					*rhs) : BaseItemImpl(IBaseItem::TypeArray) {
	m_target = target;
	if (has_sum) {
		m_sum = new FieldImpl("sum",
			new ScalarTypeImpl(IScalarType::ScalarType_Int,
					new LiteralImpl((uint64_t)63),
					new LiteralImpl((uint64_t)0)),
			IField::FieldAttr_None,
			0
		);
		add(m_sum);
	}
	m_size = new FieldImpl("size",
			new ScalarTypeImpl(IScalarType::ScalarType_Bit,
					new LiteralImpl((uint64_t)31),
					new LiteralImpl((uint64_t)0)),
			IField::FieldAttr_None,
			0
			);
	add(m_size);
	m_lhs = lhs;
	m_rhs = rhs;
}

ArrayTypeImpl::~ArrayTypeImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ArrayTypeImpl::clone() const {
	return new ArrayTypeImpl(
			m_target,
			hasSum(),
			m_lhs,
			m_rhs);
}
