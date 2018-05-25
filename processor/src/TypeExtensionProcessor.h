/*
 * TypeExtensionProcessor.h
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

#ifndef SRC_TYPEEXTENSIONPROCESSOR_H_
#define SRC_TYPEEXTENSIONPROCESSOR_H_
#include "IModelProcessor.h"
#include "PSIVisitor.h"



namespace qpssc {

class TypeExtensionProcessor :
		public virtual IModelProcessor,
		public virtual PSIVisitor {

public:
	TypeExtensionProcessor();

	virtual ~TypeExtensionProcessor();

	virtual bool process(IModel *model);

	virtual void visit_package(IPackage *pkg);

	virtual void visit_extend_composite(IExtendComposite *e);

	virtual void visit_extend_enum(IExtendEnum *e);

};

} /* namespace qpssc */

#endif /* SRC_TYPEEXTENSIONPROCESSOR_H_ */
