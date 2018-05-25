/*
 * TestExecListener.h
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
 *  Created on: Apr 29, 2018
 *      Author: ballance
 */

#pragma once
#include <string>
#include "IExecListener.h"
#include "IVarValueProvider.h"
#include "TargetTemplateProcessor.h"

class TestExecListener: public virtual IExecListener {
public:
	TestExecListener(IVarValueProvider *vp);

	virtual ~TestExecListener();

	virtual void fork_parallel();

	virtual void start_thread();

	virtual void end_thread();

	virtual void join_parallel();

	virtual void exec(
			const std::string	&context,
			IExec 				*e);

	const std::string &get_result() { return m_result; }

private:

	void inc_indent();

	void dec_indent();

	const std::string &indent();

private:
	TargetTemplateProcessor			m_template_proc;
	std::string						m_result;
	std::string						m_indent;


};

