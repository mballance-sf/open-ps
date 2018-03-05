/*
 * StripUnreferencedTypesProcessor.h
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#ifndef SRC_STRIPUNREFERENCEDTYPESPROCESSOR_H_
#define SRC_STRIPUNREFERENCEDTYPESPROCESSOR_H_

#include "IModelProcessor.h"
#include "PSIVisitor.h"

using namespace psi::apps;

namespace qpssc {

class StripUnreferencedTypesProcessor:
		public virtual IModelProcessor,
		public virtual PSIVisitor {
public:
	StripUnreferencedTypesProcessor();

	virtual ~StripUnreferencedTypesProcessor();

	virtual bool process(IModel *model);

	virtual void visit_action(IAction *a);

	virtual void visit_struct(IStruct *str);

	virtual void visit_component(IComponent *c);

	virtual void visit_package(IPackage *pkg);

};

} /* namespace qpssc */

#endif /* SRC_STRIPUNREFERENCEDTYPESPROCESSOR_H_ */
