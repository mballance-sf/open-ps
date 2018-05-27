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

using namespace antlr4;

OPSC::OPSC() :
	m_model(new ModelImpl()) {

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

	return rp.process(m_model.get());
}

bool OPSC::elab(
		const std::string		&comp,
		const std::string		&action) {
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
	fprintf(stdout, "TODO: QPSC::write\n");

	return false;
}
