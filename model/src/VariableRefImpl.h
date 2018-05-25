/*
 * VariableRefImpl.h
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
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_VARIABLEREFIMPL_H_
#define MODEL_SRC_VARIABLEREFIMPL_H_

#include "ExprImpl.h"
#include "IVariableRef.h"

class VariableRefImpl: public ExprImpl, public virtual IVariableRef {
public:
	VariableRefImpl(
			IBaseItem			*scope,
			const std::string 	&id,
			IExpr 				*index_lhs,
			IExpr				*index_rhs);

	virtual ~VariableRefImpl();

	virtual IBaseItem *getScope() const { return m_scope; }

	virtual const std::string &getId() const { return m_id; }

	virtual void setIndexLhs(IExpr *lhs) { m_index_lhs = lhs; }

	virtual IExpr *getIndexLhs() const { return m_index_lhs; }

	virtual IExpr *getIndexRhs() const { return m_index_rhs; }

	virtual IBaseItem *getTarget() const { return m_target; }

	virtual void setTarget(IBaseItem *t) { m_target = t; }

	virtual void setNext(IVariableRef *next) { m_next = next; }

	// Returns the next link in the variable-ref chain
	virtual IVariableRef *getNext() const { return m_next; }

	virtual std::string toString() const;

	virtual IExpr *clone() const;

private:
	IBaseItem				*m_scope;
	IBaseItem				*m_target;
	std::string				m_id;
	IExpr					*m_index_lhs;
	IExpr					*m_index_rhs;
	IVariableRef			*m_next;


};

#endif /* MODEL_SRC_VARIABLEREFIMPL_H_ */
