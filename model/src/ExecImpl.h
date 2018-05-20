/*
 * ExecImpl.h
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

#pragma once
#include "IExec.h"
#include "BaseItemImpl.h"



class ExecImpl :
		public virtual IExec,
		public virtual BaseItemImpl {
public:
	ExecImpl(
			ExecKind									kind,
			const std::string							&language,
			const std::string							&text,
			const std::vector<IExecReplacementExpr *>	&replacements);

	ExecImpl(
			ExecKind			kind,
			IExecCallback		*exec);

	ExecImpl(
			ExecKind						kind,
			const std::vector<IExecStmt *>	&stmts);

#ifdef PSS_HAVE_CXX_11
	ExecImpl(
			ExecKind							kind,
			std::initializer_list<IExecStmt *>	stmts) :
				ExecImpl(kind, std::vector<IExecStmt *>(stmts)) { }
#endif

	ExecImpl();

	virtual ~ExecImpl();

	virtual ExecKind getExecKind() const { return m_execKind; }

	virtual ExecType getExecType() const { return m_execType; }

	virtual const std::string &getLanguage() const { return m_language; }

	/**
	 * Returns the target-template string for
	 * exec type TargetTemplate
	 */
	virtual const std::string &getTargetTemplate() const {
		return m_targetTemplate;
	}

	virtual const std::vector<IExecReplacementExpr *> &getTargetTemplateReplacements() const {
		return m_template_replacements;
	}

	/**
	 * Returns the inline-exec closure for exec type Inline
	 */
	virtual IExecCallback *getInlineExec() const { return m_inlineExec; }

	/**
	 * Returns the native-exec statement list
	 */
	virtual const std::vector<IExecStmt *> &getStmts() const { return m_stmts; }

	std::vector<IExecStmt *> &getStmtsM();

	void addStmt(IExecStmt *s);

	virtual IBaseItem *clone() const;

private:
	ExecKind					m_execKind;
	ExecType					m_execType;
	std::string					m_language;
	std::string					m_targetTemplate;
	IExecCallback							*m_inlineExec;
	std::vector<IExecStmt *>				m_stmts;
	std::vector<IExecReplacementExpr *>		m_template_replacements;

};


