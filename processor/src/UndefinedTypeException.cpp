/*
 * UndefinedTypeException.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: ballance
 */

#include "UndefinedTypeException.h"

UndefinedTypeException::UndefinedTypeException(
		IBaseItem			*scope,
		IRefType			*type_ref
		) : m_scope(scope), m_type_ref(type_ref) {

}

UndefinedTypeException::~UndefinedTypeException() {
	// TODO Auto-generated destructor stub
}

