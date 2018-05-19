/*
 * StringTable.h
 *
 *  Created on: May 19, 2018
 *      Author: ballance
 */

#pragma once
#include "IStringTable.h"
#include <stdint.h>
#include <map>
#include <string>

class StringTable : public virtual IStringTable {
public:
	StringTable();

	virtual ~StringTable();

	virtual uint32_t str2id(const std::string &str) const {
		return m_str2id.find(str)->second;
	}

	virtual const std::string &id2str(uint32_t id) const {
		return m_id2str.find(id)->second;
	}

	virtual uint32_t size() const {
		return m_str2id.size();
	}

	virtual uint32_t bits() const;

	void add(const std::string &str);

private:
	std::map<std::string, uint32_t>			m_str2id;
	std::map<uint32_t, std::string>			m_id2str;
};

