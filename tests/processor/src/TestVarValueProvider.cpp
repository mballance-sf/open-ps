/*
 * TestVarValueProvider.cpp
 *
 *  Created on: Apr 28, 2018
 *      Author: ballance
 */

#include "TestVarValueProvider.h"

TestVarValueProvider::TestVarValueProvider(const std::map<std::string, VarVal> &var_map) : m_var_map(var_map) {

}

TestVarValueProvider::~TestVarValueProvider() {
	// TODO Auto-generated destructor stub
}

void TestVarValueProvider::set_value(const std::string &k, const std::string &v) {
	VarVal val;
	val.s = v;
	val.type = VarVal_String;

	if (m_var_map.find(k) != m_var_map.end()) {
		m_var_map[k] = val;
	}
}

void TestVarValueProvider::set_value(const std::string &k, uint32_t v) {
	VarVal val;
	val.ui = v;
	val.type = VarVal_Uint;

	if (m_var_map.find(k) != m_var_map.end()) {
		m_var_map[k] = val;
	}
}

VarVal TestVarValueProvider::get_value(const std::string &path) {
	VarVal ret;

	std::map<std::string, VarVal>::iterator it = m_var_map.find(path);

	if (it != m_var_map.end()) {
		ret = it->second;
		fprintf(stdout, "Var: %s is type %d\n",
				path.c_str(), it->second.type);
	} else {
		fprintf(stdout, "Error: unknown variable %s\n", path.c_str());
	}

	return ret;
}
