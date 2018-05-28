/*
 * ModelNameProvider.cpp
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
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#include "ModelNameProvider.h"

ModelNameProvider::ModelNameProvider() {
	m_name_valid = false;
}

ModelNameProvider::~ModelNameProvider() {
	// TODO Auto-generated destructor stub
}

void ModelNameProvider::enter(const std::string &name) {
	m_name_valid = false;
	m_scopes.push_back(ScopeInfo(name));
}

void ModelNameProvider::leave(const std::string &name) {
	leave();
}

void ModelNameProvider::enter(IActivitySelectStmt *s) {
	m_name_valid = false;
	if (m_scopes.back().m_idx != -1) {
		m_scopes.back().m_idx++;
	}
	m_scopes.push_back(ScopeInfo("select", 0));
}

void ModelNameProvider::leave(IActivitySelectStmt *s) {
	leave();
}

void ModelNameProvider::enter(IActivitySelectBranchStmt *s) {
	// TODO: need uniquified id
	m_name_valid = false;
	m_scopes.back().m_idx++;
	m_scopes.push_back(ScopeInfo("branch"));
}

void ModelNameProvider::leave(IActivitySelectBranchStmt *s) {
	leave();
}

void ModelNameProvider::enter(IField *field) {
	m_name_valid = false;
	m_scopes.push_back(ScopeInfo(field->getName()));
}

void ModelNameProvider::leave(IField *field) {
	leave();
}

const std::string &ModelNameProvider::name() {
	if (!m_name_valid) {
		int32_t last_idx = -1;
		m_name.clear();
		for (uint32_t i=0; i<m_scopes.size(); i++) {
			if (last_idx != -1) {
				char buf[64];
				sprintf(buf, "%d_");
				m_name.append(buf);
			}
			m_name.append(m_scopes.at(i).m_name);
			if (i+1 < m_scopes.size()) {
				m_name.append(".");
			}
			last_idx = m_scopes.at(i).m_idx;
		}
		m_name_valid = true;
	}
	return m_name;
}

void ModelNameProvider::leave() {
	if (m_scopes.size() > 0) {
		m_name_valid = false;
		m_scopes.pop_back();
	} else {
		fprintf(stdout, "Error: leave with 0-size stack\n");
	}
}
