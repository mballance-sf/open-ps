/*
 * VarVal.h
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


