/*
 * TestExecListener.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: ballance
 */

#include "TestExecListener.h"

TestExecListener::TestExecListener(IVarValueProvider *vp) :
	m_template_proc(vp) {
}

TestExecListener::~TestExecListener() {
	// TODO Auto-generated destructor stub
}

void TestExecListener::fork_parallel() {
	m_result.append(indent());
	m_result.append("fork_parallel\n");
	inc_indent();
}

void TestExecListener::start_thread() {
	m_result.append(indent());
	m_result.append("start_thread\n");
	inc_indent();
}

void TestExecListener::end_thread() {
	dec_indent();
	m_result.append(indent());
	m_result.append("start_thread\n");
}

void TestExecListener::join_parallel() {
	dec_indent();
	m_result.append(indent());
	m_result.append("join_parallel\n");
}

void TestExecListener::exec(
		const std::string	&context,
		IExec 				*e) {
	switch (e->getExecType()) {
	case IExec::TargetTemplate: {
		std::string tt = m_template_proc.process(
				context,
				e->getTargetTemplate(),
				e->getTargetTemplateReplacements());
		m_result.append(indent());
		m_result.append("begin_exec:\n");
		m_result.append(tt);
		m_result.append("\n");
		m_result.append("end_exec:\n");
	} break;
	default:
		m_result.append(indent());
		m_result.append("exec: unhandled type\n");
		break;
	}
}

void TestExecListener::inc_indent() {
	m_indent.append("    ");
}

void TestExecListener::dec_indent() {
	if (m_indent.size() > 4) {
		m_indent = m_indent.substr(0, m_indent.size()-4);
	} else {
		m_indent.clear();
	}
}

const std::string &TestExecListener::indent() {
	return m_indent;
}
