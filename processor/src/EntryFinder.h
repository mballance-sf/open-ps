/*
 * EntryFinder.h
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_ENTRYFINDER_H_
#define PROCESSOR_SRC_ENTRYFINDER_H_
#include "IModel.h"
#include "IComponent.h"
#include "IAction.h"
#include <string>
#include <tuple>

class EntryFinder {
public:

	static bool find(
			IModel									*model,
			const std::string						&component,
			const std::string						&action,
			std::tuple<IComponent *, IAction *> 	&result);


};

#endif /* PROCESSOR_SRC_ENTRYFINDER_H_ */
