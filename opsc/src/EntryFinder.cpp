/*
 * EntryFinder.cpp
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
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#include "EntryFinder.h"

bool EntryFinder::find(
		IModel									*model,
		const std::string						&component,
		const std::string						&action,
		std::tuple<IComponent *, IAction *>		&result) {
	IComponent *comp_h = 0;
	IAction *action_h = 0;

	// First, find the component
	std::string comp_name = component;
	if (comp_name == "") {
		comp_name = "pss_top";
	}

	for (std::vector<IBaseItem *>::const_iterator it=model->getItems().begin();
			it!=model->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeComponent &&
				dynamic_cast<IComponent *>(*it)->getName() == comp_name) {
			comp_h = dynamic_cast<IComponent *>(*it);
			break;
		}
	}

	// Next, find the action
	if (comp_h) {
		for (std::vector<IBaseItem *>::const_iterator it=comp_h->getItems().begin();
				it!=comp_h->getItems().end(); it++) {
			if ((*it)->getType() == IBaseItem::TypeAction &&
					dynamic_cast<IAction *>(*it)->getName() == action) {
				action_h = dynamic_cast<IAction *>(*it);
				break;
			}
		}
	}

	result = std::tuple<IComponent *, IAction *>(comp_h, action_h);

	return (comp_h && action_h);
}

