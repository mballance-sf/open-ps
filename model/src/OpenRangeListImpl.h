/*
 * OpenRangeListImpl.h
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

#ifndef MODEL_SRC_OPENRANGELISTIMPL_H_
#define MODEL_SRC_OPENRANGELISTIMPL_H_

#include "IOpenRangeList.h"

class OpenRangeListImpl: public virtual IOpenRangeList {
public:
	OpenRangeListImpl(
			const std::vector<IOpenRangeValue *>	&ranges);

	virtual ~OpenRangeListImpl();

	virtual const std::vector<IOpenRangeValue *> &ranges() const {
		return m_ranges;
	}

	virtual IOpenRangeList *clone() const;

private:
	std::vector<IOpenRangeValue *>				m_ranges;

};

#endif /* MODEL_SRC_OPENRANGELISTIMPL_H_ */
