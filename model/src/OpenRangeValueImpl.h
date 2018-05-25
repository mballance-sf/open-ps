/*
 * OpenRangeValueImpl.h
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
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_OPENRANGEVALUEIMPL_H_
#define MODEL_SRC_OPENRANGEVALUEIMPL_H_

#include "IOpenRangeValue.h"

class OpenRangeValueImpl: public virtual IOpenRangeValue {
public:

	OpenRangeValueImpl(
			IExpr		*lhs,
			IExpr		*rhs,
			bool		domain_bound
			);

	virtual ~OpenRangeValueImpl();

	virtual IExpr *getLHS() const { return m_lhs; }

	virtual IExpr *getRHS() const { return m_rhs; }

	virtual bool isDomainBound() const { return m_domain_bound; }


private:
	IExpr						*m_lhs;
	IExpr						*m_rhs;
	bool						m_domain_bound;
};

#endif /* MODEL_SRC_OPENRANGEVALUEIMPL_H_ */
