/*
 * ExecImpl.cpp
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
 *  Created on: Jun 1, 2016
 *      Author: ballance
 */

#include "ExecImpl.h"

ExecImpl::ExecImpl(
		ExecKind									kind,
		const std::string							&language,
		const std::string							&text,
		const std::vector<IExecReplacementExpr *>	&replacements) :
				BaseItemImpl(IBaseItem::TypeExec),
				m_execKind(kind), m_execType(IExec::TargetTemplate),
				m_language(language), m_targetTemplate(text),
				m_inlineExec(0), m_stmts(0),
				m_template_replacements(replacements) { }

ExecImpl::ExecImpl(
		ExecKind			kind,
		IExecCallback		*exec) :
				BaseItemImpl(IBaseItem::TypeExec),
				m_execKind(kind), m_execType(IExec::Inline),
				m_inlineExec(exec), m_stmts(0) { }

ExecImpl::ExecImpl(
		ExecKind						kind,
		const std::vector<IExecStmt *>	&stmts) :
				BaseItemImpl(IBaseItem::TypeExec),
				m_execKind(kind), m_execType(IExec::Native),
				m_inlineExec(0), m_stmts(stmts) { }

ExecImpl::ExecImpl() : BaseItemImpl(IBaseItem::TypeExec),
		m_execKind(IExec::Body),
		m_execType(IExec::Native),
		m_inlineExec(0) { }

ExecImpl::~ExecImpl() {
	// TODO Auto-generated destructor stub
}

std::vector<IExecStmt *> &ExecImpl::getStmtsM() {
	return m_stmts;
}

void ExecImpl::addStmt(IExecStmt *s) {
	m_stmts.push_back(s);
}

IBaseItem *ExecImpl::clone() const {
	IBaseItem *ret = 0;
	ExecImpl *impl;
	switch (getExecType()) {
	case IExec::TargetTemplate:
		impl = new ExecImpl(getExecKind(), getLanguage(), getTargetTemplate(),
				getTargetTemplateReplacements());
		break;

	case IExec::Native: {
		std::vector<IExecStmt *> stmts;
		for (std::vector<IExecStmt *>::const_iterator it=getStmts().begin();
				it!=getStmts().end(); it++) {
			stmts.push_back((*it)->clone());
		}

		impl = new ExecImpl(getExecKind(), stmts);
	} break;

	case IExec::Inline:
		impl = new ExecImpl(getExecKind(), getInlineExec());
		break;
	}

	ret = impl;

	return ret;
}

