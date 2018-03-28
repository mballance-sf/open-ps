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
#include <map>
#include <string>
#include <stdint.h>

using namespace psi_api;

class StringTableBuilder: public psi::apps::PSIVisitor {
public:
	StringTableBuilder();

	virtual ~StringTableBuilder();

	void build(IModel *model);

	void build(IAction *action);

	void build(IComponent *component);

	uint32_t str2id(const std::string &str) const {
		return m_str2id.find(str)->second;
	}

	const std::string &id2str(uint32_t id) const {
		return m_id2str.find(id)->second;
	}

	uint32_t size() const {
		return m_str2id.size();
	}

	uint32_t bits() const;

protected:

	virtual void visit_literal_expr(ILiteral *l) override;


private:
	std::map<std::string, uint32_t>			m_str2id;
	std::map<uint32_t, std::string>			m_id2str;
};

#endif /* PROCESSOR_SRC_STRINGTABLEBUILDER_H_ */
