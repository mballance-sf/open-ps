/*
 * SymbolImpl.h
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_SYMBOLIMPL_H_
#define MODEL_SRC_SYMBOLIMPL_H_

#include "BaseItemImpl.h"
#include "ScopeItemImpl.h"
#include "ISymbol.h"

using namespace psi_api;
using namespace psi;

class SymbolImpl: public ISymbol,
	public virtual BaseItemImpl,
	public virtual ScopeItemImpl {
public:
	SymbolImpl(
			const std::string				&name,
			const std::vector<IField *>		&params,
			IActivityBlockStmt					*body
			);

	virtual ~SymbolImpl();

	virtual const std::string &getName() const { return m_name; }

	virtual const std::vector<IField *> &getParameters() const { return m_params; }

	virtual IActivityBlockStmt *getBody() const { return m_body; }

	virtual IBaseItem *clone() const;

private:
	std::string						m_name;
	std::vector<IField *>			m_params;
	IActivityBlockStmt					*m_body;

};

#endif /* MODEL_SRC_SYMBOLIMPL_H_ */
