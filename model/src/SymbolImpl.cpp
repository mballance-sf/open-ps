/*
 * SymbolImpl.cpp
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#include "SymbolImpl.h"

SymbolImpl::SymbolImpl(
		const std::string			&name,
		const std::vector<IField *>	&params,
		IActivityBlockStmt				*body) :
		BaseItemImpl(IBaseItem::TypeSymbol), m_name(name),
		m_params(params), m_body(body){

	for (std::vector<IField *>::const_iterator it=params.begin();
			it!=params.end(); it++) {
		add(*it);
	}

}

SymbolImpl::~SymbolImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *SymbolImpl::clone() const {
	// TODO: deep clone?
	return new SymbolImpl(m_name, m_params, m_body);
}
