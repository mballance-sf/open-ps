/*
 * IActivityBlockStmt.h
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
 *
 *  Created on: May 8, 2016
 *      Author: ballance
 */

#ifndef INCLUDED_ACTIVITY_BLOCK_STMT_H
#define INCLUDED_ACTIVITY_BLOCK_STMT_H
#include <vector>
#include "IActivityStmt.h"

namespace psi_api {

class IActivityBlockStmt : public virtual IActivityStmt {
public:
	virtual ~IActivityBlockStmt() { }

	virtual const std::vector<IActivityStmt *> &getStmts() const = 0;

	virtual void add(IActivityStmt *stmt) = 0;

};

}




#endif /* INCLUDED_ACTIVITY_BLOCK_STMT_H */