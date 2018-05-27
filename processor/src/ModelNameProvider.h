/*
 * ModelNameProvider.h
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

#pragma once
#include <string>
#include <vector>
#include <stdint.h>
#include "IField.h"
#include "IAction.h"
#include "IActivitySelectStmt.h"


class ModelNameProvider {
public:
	ModelNameProvider();

	virtual ~ModelNameProvider();

	void enter(const std::string &name);
	void leave(const std::string &name);

	void enter(IActivitySelectStmt *s);
	void leave(IActivitySelectStmt *s);

	void enter(IActivitySelectBranchStmt *s);
	void leave(IActivitySelectBranchStmt *s);

	void enter(IField *field);
	void leave(IField *field);

	const std::string &name();

private:
	void leave();

	class ScopeInfo {
	public:
		ScopeInfo(const std::string name, int32_t idx=-1) {
			m_name = name;
			m_idx = idx;
		}
	public:
		std::string		m_name;
		int32_t			m_idx;
	};

private:
	std::string							m_name;
	bool								m_name_valid;
	std::vector<ScopeInfo>				m_scopes;

};

