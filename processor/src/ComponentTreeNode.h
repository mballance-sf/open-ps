/*
 * ComponentTreeNode.h
 *
 *  Created on: Jan 31, 2018
 *      Author: ballance
 */

#pragma once
#include "IComponent.h"
#include "IModel.h"
#include <stdint.h>
#include <string>
#include <vector>



namespace qpssc {

class ComponentTreeNode {
public:
	ComponentTreeNode(
			ComponentTreeNode		*parent,
			const std::string		&name,
			IComponent				*comp,
			uint32_t				id);

	virtual ~ComponentTreeNode();

	ComponentTreeNode *parent() const { return m_parent; }

	const std::string &name() const { return m_name; }

	IComponent *comp() const { return m_component; }

	uint32_t id() const { return m_id; }

	const std::vector<ComponentTreeNode *> &children() const { return m_children; }

	std::string toString();

	static ComponentTreeNode *build(
			IModel 			*model,
			IComponent		*comp);

private:

	void toString(
			std::string			&str,
			const std::string	&ind);

	static ComponentTreeNode *build(
			IModel					*model,
			ComponentTreeNode		*parent,
			const std::string		&name,
			IComponent				*comp,
			uint32_t				&id
			);

private:
	// Parent for this element
	ComponentTreeNode	*m_parent;
	// Instance name for this element
	std::string			m_name;

	IComponent			*m_component;

	// Component ID for this instance
	uint32_t			m_id;

	std::vector<ComponentTreeNode *>		m_children;

};

} /* namespace qpssc */

