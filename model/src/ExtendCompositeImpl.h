/*
 * ExtendCompositeImpl.h
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_EXTENDCOMPOSITEIMPL_H_
#define MODEL_SRC_EXTENDCOMPOSITEIMPL_H_

#include "BaseItemImpl.h"
#include "ScopeItemImpl.h"
#include "IExtendComposite.h"



class ExtendCompositeImpl:
		public virtual IExtendComposite,
		public virtual BaseItemImpl,
		public virtual ScopeItemImpl {
public:
	ExtendCompositeImpl(
			IExtend::ExtendType	type,
			IBaseItem			*target);

	virtual ~ExtendCompositeImpl();

	virtual ExtendType getExtendType() const { return m_extend_type; }

	virtual IBaseItem *getTarget() const { return m_target; }

	virtual IBaseItem *clone() const;

private:
	IExtend::ExtendType				m_extend_type;
	IBaseItem						*m_target;
};

#endif /* MODEL_SRC_EXTENDCOMPOSITEIMPL_H_ */
