/*
 * ComponentTreeNode.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: ballance
 */

#include "ComponentTreeNode.h"

namespace qpssc {

ComponentTreeNode::ComponentTreeNode(
		ComponentTreeNode			*parent,
		const std::string			&name,
		IComponent					*comp,
		uint32_t					id) {
	m_parent = parent;
	m_name = name;
	m_component = comp;
	m_id = id;
}

ComponentTreeNode::~ComponentTreeNode() {
	// TODO Auto-generated destructor stub
}


std::string ComponentTreeNode::toString() {
	std::string ret;
	toString(ret, "");

	return ret;
}

void ComponentTreeNode::toString(
		std::string			&str,
		const std::string	&ind) {

	str.append(ind + m_name + " : " + m_component->getName() + "\n");

	for (std::vector<ComponentTreeNode *>::const_iterator it=m_children.begin();
			it!=m_children.end(); it++) {
		(*it)->toString(str, (ind + "    "));
	}
}

ComponentTreeNode *ComponentTreeNode::build(
		IModel			*model,
		IComponent		*comp) {
	uint32_t id = 0;
	return build(model, 0, comp->getName(), comp, id);
}

ComponentTreeNode *ComponentTreeNode::build(
		IModel					*model,
		ComponentTreeNode		*parent,
		const std::string		&name,
		IComponent				*comp,
		uint32_t				&id) {
	ComponentTreeNode *node = new ComponentTreeNode(
			parent, name, comp, id++);

	if (parent) {
		parent->m_children.push_back(node);
	}

	// TODO: find binds and assess their impact

	// TODO: iterate through actions at this level

	// Traverse sub-component instances
	for (std::vector<IBaseItem *>::const_iterator it=comp->getItems().begin();
			it != comp->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeField) {
			IField *field = dynamic_cast<IField *>(*it);

			if (dynamic_cast<IComponent *>(field->getDataType())) {
				// This is a component-type field
				build(model, node,
						field->getName(),
						dynamic_cast<IComponent *>(field->getDataType()),
						id);
			}
		}
	}

	return node;
}

} /* namespace qpssc */
