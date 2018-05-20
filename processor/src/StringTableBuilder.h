/*
 * StringTableBuilder.h
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

	IStringTable *build(IComponent *c, IAction *a);

protected:

	virtual void visit_literal_expr(ILiteral *l) override;

private:
	StringTable				*m_strtab;

};

#endif /* PROCESSOR_SRC_STRINGTABLEBUILDER_H_ */
