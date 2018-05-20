/*
 * EnumTypeImpl.h
 *
 *  Created on: Mar 16, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ENUMTYPEIMPL_H_
#define MODEL_SRC_ENUMTYPEIMPL_H_
#include <string>
#include "BaseItemImpl.h"
#include "IEnumType.h"



class EnumTypeImpl : public BaseItemImpl, public virtual IEnumType {
public:
	EnumTypeImpl(
			const std::string					&name,
			const std::vector<IEnumerator *>	&enumerators);

	virtual ~EnumTypeImpl();

	virtual const std::string &getName() const { return m_name; }

	virtual const std::vector<IEnumerator *> &getEnumerators() const { return m_enumerators; }

	virtual IBaseItem *clone() const;

private:
	std::string					m_name;
	std::vector<IEnumerator *>	m_enumerators;
};

#endif /* MODEL_SRC_ENUMTYPEIMPL_H_ */
