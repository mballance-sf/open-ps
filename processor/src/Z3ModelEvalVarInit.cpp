/*
 * Z3ModelEvalVarInit.cpp
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
 *  Created on: May 23, 2018
 *      Author: ballance
 */

#include "Z3ModelEvalVarInit.h"
#include "Z3ModelEvaluator.h"
#include <vector>

Z3ModelEvalVarInit::Z3ModelEvalVarInit(Z3ModelEvaluator *evaluator) {
	m_evaluator = evaluator;
}

Z3ModelEvalVarInit::~Z3ModelEvalVarInit() {
	// TODO Auto-generated destructor stub
}

void Z3ModelEvalVarInit::init(IAction *action) {

	for (std::vector<IBaseItem *>::const_iterator it=action->getItems().begin();
			it!=action->getItems().end(); it++) {
		IField *field = dynamic_cast<IField *>(*it);
		if (field) {
			init(field);
		}
	}

	if (action->getActivity()) {
		visit_activity(action->getActivity());
	}
}

void Z3ModelEvalVarInit::init(IField *field) {
	m_evaluator->name_provider().enter(field);
	if (dynamic_cast<IScalarType *>(field->getDataType())) {
		// We can directly deal with this
		Z3ModelVar *var = m_evaluator->get_variable(
				m_evaluator->name_provider().name());
		if (var) {
			fprintf(stdout, "Resetting var %s\n", var->name().c_str());
			var->reset();
		} else {
			fprintf(stdout, "Internal Error: failed to find variable %s\n",
					m_evaluator->name_provider().name());
		}
	} else {
		fprintf(stdout, "TODO: initialize composite variable %s\n",
				m_evaluator->name_provider().name().c_str());
	}
	m_evaluator->name_provider().leave(field);
}
