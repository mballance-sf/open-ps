/*
 * ActivityBlockStmtImpl.cpp
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
 *  Created on: Jun 8, 2016
 *      Author: ballance
 */

#include "ActivityBlockStmtImpl.h"

ActivityBlockStmtImpl::ActivityBlockStmtImpl(IActivityBlockStmt::ActivityStmtType type) :
		BaseItemImpl(IBaseItem::TypeActivityStmt), m_type(type) {

}

ActivityBlockStmtImpl::~ActivityBlockStmtImpl() {
	// TODO Auto-generated destructor stub
}

const std::vector<IActivityStmt *> &ActivityBlockStmtImpl::getStmts() const {
	return m_stmts;
}

std::vector<IActivityStmt *> &ActivityBlockStmtImpl::getStmtsM() {
	return m_stmts;
}

void ActivityBlockStmtImpl::add(IActivityStmt *stmt) {
	m_stmts.push_back(stmt);
}

void ActivityBlockStmtImpl::insert(std::vector<IActivityStmt *>::iterator it, IActivityStmt *stmt) {
	m_stmts.insert(it, stmt);
}

IActivityStmt *ActivityBlockStmtImpl::clone() const {
	ActivityBlockStmtImpl *ret = new ActivityBlockStmtImpl(getStmtType());

	for (std::vector<IActivityStmt *>::const_iterator it=getStmts().begin();
			it != getStmts().end(); it++) {
		ret->add(((*it)->clone()));
	}

	return ret;
}
