/*
 * XMLWriterProcessor.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#include "XMLWriterProcessor.h"
#include "PSI2XML.h"
#include <stdio.h>

namespace qpssc {

XMLWriterProcessor::XMLWriterProcessor(const std::string &filename) {
	m_filename = filename;
}

XMLWriterProcessor::~XMLWriterProcessor() {
	// TODO Auto-generated destructor stub
}

bool XMLWriterProcessor::process(IModel *model) {
	FILE *fp = fopen(m_filename.c_str(), "w");
	psi::apps::PSI2XML psi2xml;

	if (!fp) {
		fprintf(stdout, "Error: failed to open %s\n", m_filename.c_str());
		return false;
	}

	std::string content = psi2xml.traverse(model);

	fputs(content.c_str(), fp);

	fclose(fp);

	return true;
}

} /* namespace qpssc */
