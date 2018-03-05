/*
 * MarkReferencedTypesProcessor.h
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#ifndef SRC_MARKREFERENCEDTYPESPROCESSOR_H_
#define SRC_MARKREFERENCEDTYPESPROCESSOR_H_

#include "IModelProcessor.h"
#include "PSIVisitor.h"

using namespace psi_api;
using namespace psi::apps;

namespace qpssc {

class MarkReferencedTypesProcessor:
		public virtual IModelProcessor,
		public virtual PSIVisitor {
public:
	MarkReferencedTypesProcessor();

	virtual ~MarkReferencedTypesProcessor();

	virtual bool process(IModel *model);

	virtual void visit_action(IAction *a);

	virtual void visit_struct(IStruct *str);

	virtual void visit_component(IComponent *c);

	virtual void visit_field(IField *f);

};

} /* namespace qpssc */

#endif /* SRC_MARKREFERENCEDTYPESPROCESSOR_H_ */
