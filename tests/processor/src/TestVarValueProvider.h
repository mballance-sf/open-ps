/*
 * TestVarValueProvider.h
 *
 *  Created on: Apr 28, 2018
 *      Author: ballance
 */
#pragma once

#include "IVarValueProvider.h"
#include "VarVal.h"
#include <map>
#include <string>

class TestVarValueProvider : public virtual IVarValueProvider {
public:
	TestVarValueProvider(const std::map<std::string, VarVal> &var_map);

	virtual ~TestVarValueProvider();

	void set_value(const std::string &k, const std::string &v);

	void set_value(const std::string &k, uint32_t v);

	virtual VarVal get_value(const std::string &path);

private:
	std::map<std::string, VarVal> 		m_var_map;


};

