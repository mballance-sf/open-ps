/*
 * TypeExtensionProcessor.h
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#ifndef SRC_TYPEEXTENSIONPROCESSOR_H_
#define SRC_TYPEEXTENSIONPROCESSOR_H_
#include "IModelProcessor.h"
#include "PSIVisitor.h"

using namespace psi::apps;

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
