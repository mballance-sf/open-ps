/*
 * IStringTable.h
 *
 *  Created on: Apr 28, 2018
 *      Author: ballance
 */
#pragma once
#include <stdint.h>
#include <string>

class IStringTable {
public:

	virtual ~IStringTable() { }

	virtual uint32_t str2id(const std::string &str) const = 0;

	virtual const std::string &id2str(uint32_t id) const = 0;

};

