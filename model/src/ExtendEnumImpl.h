/*
 * ExtendEnumImpl.h
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_EXTENDENUMIMPL_H_
#define MODEL_SRC_EXTENDENUMIMPL_H_

#include "BaseItemImpl.h"
#include "IExtendEnum.h"



class ExtendEnumImpl:
		public virtual BaseItemImpl,
		public virtual IExtendEnum {
public:
	ExtendEnumImpl(
			IBaseItem							*target,
			const std::vector<IEnumerator *> 	&enumerators);

	virtual ~ExtendEnumImpl();

	virtual ExtendType getExtendType() const {
		return IExtend::ExtendType_Enum;
	}

	virtual IBaseItem *getTarget() const { return m_target; }

	virtual const std::vector<IEnumerator *> &getEnumerators() const {
		return m_enumerators;
	}

	virtual IBaseItem *clone() const;

private:
	IBaseItem							*m_target;
	std::vector<IEnumerator *>			m_enumerators;
};

#endif /* MODEL_SRC_EXTENDENUMIMPL_H_ */
