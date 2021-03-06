/*
 * OPSC.cpp
 *
 *  Created on: May 26, 2018
 *      Author: ballance
 */

#include "OPSC.h"
#include "ModelImpl.h"
#include "PSSLexer.h"
#include "PSSParser.h"
#include "PSSBaseVisitor.h"
#include <cstdio>
#include <iostream>
#include "PSS2ModelVisitor.h"
#include "ResolveRefsProcessor.h"
#include "EntryFinder.h"
#include "Model2XML.h"

using namespace antlr4;

OPSC::OPSC() :
	m_model(new ModelImpl()) {
	m_debug = false;

}

OPSC::~OPSC() {
	// TODO Auto-generated destructor stub
}

bool OPSC::parse(
		std::istream		&in,
		const std::string	&path) {
	PSS2ModelVisitor visitor(m_model.get(), path);
	ANTLRInputStream input(in);
	PSSLexer lexer(&input);

	visitor.set_debug(m_debug);

	CommonTokenStream tokens(&lexer);

	PSSParser parser(&tokens);

	if (parser.getNumberOfSyntaxErrors() > 0) {
		return false;
	}

	PSSParser::ModelContext *ctxt = parser.model();

	try {
		visitor.visitModel(ctxt);
	} catch (std::string &m) {
		fprintf(stdout, "Error: %s\n", m.c_str());
		return false;
	}

	return true;
}

bool OPSC::load(
			std::istream 		&in,
			const std::string	&path) {
	fprintf(stdout, "TODO: QPSC::load\n");

	return false;
}

bool OPSC::link() {
	ResolveRefsProcessor rp;

	rp.set_debug(m_debug);

	return rp.process(m_model.get());
}

bool OPSC::elab(
		const std::string		&comp,
		const std::string		&action) {
	return elab(comp, action, std::vector<std::string>());
}

bool OPSC::elab(
		const std::string				&comp,
		const std::string				&action,
		const std::vector<std::string>	&packages) {
	fprintf(stdout, "TODO: QPSC::elab\n");

	std::tuple<IComponent *, IAction *> entry;
	if (!EntryFinder::find(m_model.get(), comp, action, entry)) {
		fprintf(stdout, "Error: failed to find root component:action %s:%s\n",
				comp.c_str(), action.c_str());

		return false;
	}

	m_root_comp = std::get<0>(entry);
	m_root_action = std::get<1>(entry);

	return true;
}

bool OPSC::write(std::ostream &out) {
	Model2XML toxml;

	toxml.convert(out, m_model.get());

	return true;
}
