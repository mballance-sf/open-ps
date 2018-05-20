/*
 * UndefinedTypeException.h
 *
 *  Created on: Mar 12, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_UNDEFINEDTYPEEXCEPTION_H_
#define PROCESSOR_SRC_UNDEFINEDTYPEEXCEPTION_H_
#include <exception>
#include "IScopeItem.h"
#include "IRefType.h"



class UndefinedTypeException : public std::exception {
public:

	UndefinedTypeException(
			IBaseItem		*scope,
			IRefType		*type);

	virtual ~UndefinedTypeException();

	IBaseItem *scope() const { return m_scope; }

	IRefType *type_ref() const { return m_type_ref; }

private:
	IBaseItem				*m_scope;
	IRefType				*m_type_ref;

};

#endif /* PROCESSOR_SRC_UNDEFINEDTYPEEXCEPTION_H_ */
