/*
 * CreateBuiltinFuncsProcessor.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: ballance
 */

#include "CreateBuiltinFuncsProcessor.h"

namespace qpssc {

CreateBuiltinFuncsProcessor::CreateBuiltinFuncsProcessor() {
	// TODO Auto-generated constructor stub

}

CreateBuiltinFuncsProcessor::~CreateBuiltinFuncsProcessor() {
	// TODO Auto-generated destructor stub
}

bool CreateBuiltinFuncsProcessor::process(IModel *model) {

	// Create a new package
	IPackage *pkg = model->findPackage("inFact", true);

	std::vector<IField *> parameters;
	IImportFunc *stop_loop_expansion = model->mkImportFunc(
			"stop_loop_expansion", 0, parameters);
	stop_loop_expansion->setAttribute("BUILTIN", "true");

	pkg->add(stop_loop_expansion);

	IImportFunc *thread_context_create = model->mkImportFunc(
			"thread_context_create", 0, parameters);
	thread_context_create->setAttribute("BUILTIN", "true");
	IImportFunc *thread_context_join = model->mkImportFunc(
			"thread_context_join", 0, parameters);
	thread_context_join->setAttribute("BUILTIN", "true");
	IImportFunc *thread_begin = model->mkImportFunc(
			"thread_begin", 0, parameters);
	thread_begin->setAttribute("BUILTIN", "true");
	IImportFunc *thread_end = model->mkImportFunc(
			"thread_end", 0, parameters);
	thread_end->setAttribute("BUILTIN", "true");

	IImportFunc *repeat_begin = model->mkImportFunc(
			"repeat_begin", 0, parameters);
	repeat_begin->setAttribute("BUILTIN", "true");
	IImportFunc *repeat_end = model->mkImportFunc(
			"repeat_end", 0, parameters);
	repeat_end->setAttribute("BUILTIN", "true");

	pkg->add(thread_context_create);
	pkg->add(thread_context_join);
	pkg->add(thread_begin);
	pkg->add(thread_end);
	pkg->add(repeat_begin);
	pkg->add(repeat_end);

	return true;
}

} /* namespace qpssc */
