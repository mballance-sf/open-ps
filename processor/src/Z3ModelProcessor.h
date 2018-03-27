/*
 * Z3ModelProcessor.h
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_Z3MODELPROCESSOR_H_
#define PROCESSOR_SRC_Z3MODELPROCESSOR_H_
#include <string>
#include <vector>
#include <map>
#include "Z3ModelVar.h"
#include "PSIVisitor.h"
#include "z3.h"

using namespace psi::apps;

class Z3ModelProcessor: public virtual PSIVisitor {
public:

	Z3ModelProcessor();

	virtual ~Z3ModelProcessor();

	virtual bool process(
			IComponent 		*comp,
			IAction			*action);

	virtual void visit_field(IField *f) override;

protected:

	void push_prefix(const std::string &pref);

	void pop_prefix();

	const std::string &prefix();

private:

	Z3_config								m_cfg;
	Z3_context								m_ctxt;
	Z3_solver								m_solver;
	std::map<std::string, Z3ModelVar *>		m_variables;
	std::vector<std::string>				m_prefix_v;
	std::string								m_prefix;
	bool									m_prefix_valid;

};

#endif /* PROCESSOR_SRC_Z3MODELPROCESSOR_H_ */
