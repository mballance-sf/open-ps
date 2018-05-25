/*
 * StringTableBuilder.h
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
 *  Created on: Mar 27, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_STRINGTABLEBUILDER_H_
#define PROCESSOR_SRC_STRINGTABLEBUILDER_H_
#include "PSIVisitor.h"
#include "IModel.h"
#include "IStringTable.h"
#include <map>
#include <string>
#include <stdint.h>




class StringTable;

class StringTableBuilder: public virtual PSIVisitor {
public:
	StringTableBuilder();

	virtual ~StringTableBuilder();

	IStringTableH build(IComponent *c, IAction *a);

protected:

	virtual void visit_literal_expr(ILiteral *l) override;

private:
	StringTable				*m_strtab;

};

#endif /* PROCESSOR_SRC_STRINGTABLEBUILDER_H_ */
