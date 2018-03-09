/*
 * Rules2AutProcessor.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: ballance
 */

#include "Rules2AutProcessor.h"
#include "QPSSCUtils.h"
#include <stdio.h>
#include <vector>

namespace qpssc {

Rules2AutProcessor::Rules2AutProcessor(const std::string &outdir) {
	m_outdir = outdir;
}

Rules2AutProcessor::~Rules2AutProcessor() {
	// TODO Auto-generated destructor stub
}

bool Rules2AutProcessor::process(IModel *model) {
	std::string rules_name;
	std::vector<std::string> cmdline;

	IComponent *comp;
	IAction *action;

	if (!QPSSCUtils::get_roots(model, &comp, &action)) {
		return false;
	}

	rules_name = comp->getName() + "_" + action->getName();

	// Create a do script to actually convert rules2aut
	FILE *fp = fopen(std::string(m_outdir + "/rules2cpp.do").c_str(), "w");
	fprintf(fp, "infact cmd rules2cpp %s.rules\n", + rules_name.c_str());
	fprintf(fp, "exec infactg++ -I$env(INFACT_HOME)/include %s_rules.cpp "
			"-L$env(INFACT_LIBDIR) -lautomataCompiler -o %s_rules.exe\n",
			rules_name.c_str(), rules_name.c_str());
	fprintf(fp, "exec ./%s_rules.exe -autname=%s -autfilename=%s.aut\n",
			rules_name.c_str(), rules_name.c_str(), rules_name.c_str());
	fclose(fp);

	cmdline.push_back("infact");
	cmdline.push_back("cmd");
	cmdline.push_back("do");
	cmdline.push_back(m_outdir + "/rules2cpp.do");

	if (QPSSCUtils::execvp(cmdline, m_outdir) != 0) {
		fprintf(stdout, "Error: execvp returned error\n");
		return false;
	}

	// Open up the aut file and encode it
//	char tmp[64];
//	char line[128];
//	size_t len;
//	fp = fopen(std::string(m_outdir + "/" + rules_name + ".aut").c_str(), "rb");
//
//	while ((len = fread(tmp, 1, 45, fp)) > 0) {
//		uint32_t lp = 0;
//		line[lp++] = len+32; // length
//
//		for (uint32_t i=0; i<len; i+=3) {
//			uint32_t td = tmp[i];
//
//			if (i+1 < len) {
//				td |= (tmp[i+1] << 8);
//			}
//			if (i+2 < len) {
//				td |= (tmp[i+2] << 8);
//			}
//
//			// Now, break down into four characters
//			for (uint32_t j=0; j<3; j++) {
//				line[lp++] = ((td >> 6*j) & 0x3F) + 32;
//				if (line[lp-1] == '"') {
//					line[lp] = line[lp-1];
//					line[lp-1] = '\\';
//					lp++;
//				}
//			}
//		}
//
//		line[lp] = 0;
//		fprintf(stdout, "Line: |%s|\n", line);
//	}
//	fclose(fp);

	return true;
}

} /* namespace qpssc */
