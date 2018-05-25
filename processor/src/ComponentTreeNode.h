/*
 * ComponentTreeNode.h
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
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

