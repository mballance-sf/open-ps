/*
 * MetaActionImportItem.h
 *
 *  Created on: Aug 16, 2016
 *      Author: ballance
 */

#ifndef SRC_METAACTIONIMPORTITEM_H_
#define SRC_METAACTIONIMPORTITEM_H_

#include "RulesItem.h"
#include "INamedItem.h"
#include "BaseItemImpl.h"
#include "NamedItemImpl.h"
#include "FieldImpl.h"

using namespace psi;

namespace qpssc {

class MetaActionImportItem:
		public RulesItem,
		public virtual NamedItemImpl {
public:
	MetaActionImportItem(const std::string &name);

	virtual ~MetaActionImportItem();
};

} /* namespace psi */

#endif /* SRC_METAACTIONIMPORTITEM_H_ */
