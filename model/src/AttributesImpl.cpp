/*
 * AttributesImpl.cpp
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
 *  Created on: Aug 16, 2016
 *      Author: ballance
 */

#include "AttributesImpl.h"

AttributesImpl::AttributesImpl() {
	// TODO Auto-generated constructor stub

}

AttributesImpl::~AttributesImpl() {
	// TODO Auto-generated destructor stub
}

const std::string &AttributesImpl::getAttribute(const std::string &key) {
	std::map<std::string,std::string>::iterator it;

	if ((it=m_attr.find(key)) != m_attr.end()) {
		return it->second;
	} else {
		return m_null_attr;
	}
}

void AttributesImpl::getAttributes(std::vector<std::string> &keys) {
	std::map<std::string,std::string>::iterator it;
	keys.clear();

	for (it=m_attr.begin(); it!=m_attr.end(); it++) {
		keys.push_back(it->first);
	}
}

bool AttributesImpl::hasAttribute(const std::string &key) {
	return (m_attr.find(key) != m_attr.end());
}

void AttributesImpl::setAttribute(const std::string &key, const std::string &val) {
	std::map<std::string,std::string>::iterator it;

	it=m_attr.find(key);

	if (it != m_attr.end()) {
		m_attr.erase(it);
	}

	m_attr[key] = val;
}

void AttributesImpl::clearAttribute(const std::string &key) {
	std::map<std::string,std::string>::iterator it;

	it=m_attr.find(key);

	if (it != m_attr.end()) {
		m_attr.erase(it);
	}
}

std::string AttributesImpl::m_null_attr;

