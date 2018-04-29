/*
 * TestExecListener.h
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

