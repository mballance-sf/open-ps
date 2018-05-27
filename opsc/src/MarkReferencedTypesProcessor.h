/*
 * MarkReferencedTypesProcessor.h
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
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#ifndef SRC_MARKREFERENCEDTYPESPROCESSOR_H_
#define SRC_MARKREFERENCEDTYPESPROCESSOR_H_

#include "IModelProcessor.h"
#include "ModelVisitor.h"




namespace qpssc {

class MarkReferencedTypesProcessor:
		public virtual IModelProcessor,
		public virtual ModelVisitor {
public:
	MarkReferencedTypesProcessor();

	virtual ~MarkReferencedTypesProcessor();

	virtual bool process(IModel *model) override;

	virtual void visit_action(IAction *a) override;

	virtual void visit_struct(IStruct *str) override;

	virtual void visit_component(IComponent *c) override;

	virtual void visit_field(IField *f) override;

};

} /* namespace qpssc */

#endif /* SRC_MARKREFERENCEDTYPESPROCESSOR_H_ */
