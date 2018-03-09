/*
 * ImportWrapperGeneratorProcessor.cpp
 *
 *  Created on: Dec 2, 2017
 *      Author: ballance
 */

#include "ImportWrapperGeneratorProcessor.h"
#include <stdio.h>

ImportWrapperGeneratorProcessor::ImportWrapperGeneratorProcessor(const std::string &outdir) {
	m_outdir = outdir;
	m_out = 0;
}

ImportWrapperGeneratorProcessor::~ImportWrapperGeneratorProcessor() {
	// TODO Auto-generated destructor stub
}

bool ImportWrapperGeneratorProcessor::process(psi_api::IModel *model) {
	m_out = fopen((m_outdir + "/pss_pi_wrapper.c").c_str(), "w");


	// TODO: define types

	visit_model(model);

	fwrite(m_wrappers.c_str(), 1, m_wrappers.size(), m_out);

	fprintf(m_out, "typedef void (*pss_pi_func)(pss_val_t *, pss_val_t **, unsigned int argc);\n");
	fprintf(m_out, "static struct pss_pi_function_t {\n");
	fprintf(m_out, "  const char *name;\n");
	fprintf(m_out, "  pss_pi_func func;\n");
	fprintf(m_out, "} funcs[] = {\n");
	fwrite(m_registration.c_str(), 1, m_registration.size(), m_out);
	fprintf(m_out, "  {0, 0}\n");
	fprintf(m_out, "};\n");

	// TODO: define plugin functions
	// Finish up with the plugin function

	fclose(m_out);

	// TODO: compile

	return true;
}

void ImportWrapperGeneratorProcessor::visit_import_func(IImportFunc *f) {
	std::string wrapper, registration;

	wrapper += "static void " + f->getName() + "(pss_val_t *ret, pss_val_t **argv, unsigned int argc) {\n";
	if (f->getReturn()) {
		// Insert assignment statements to 'ret'
	}
	// Call function, unpacking arguments
	wrapper += "  \n";
	wrapper += "}\n\n";

	registration += "  {\"" + f->getName() + "\", &" + f->getName() + "},\n";

	m_wrappers += wrapper;
	m_registration += registration;



	fprintf(stdout, "visit_import_func: %s\n", f->getName().c_str());

}

