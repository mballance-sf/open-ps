/*
 * EntryFinder.cpp
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

