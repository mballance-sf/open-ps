/*
 * QPSSC.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: ballance
 */

#include "QPSSC.h"
#include "PSI2Rules.h"
#include "CreateBuiltinFuncsProcessor.h"
#include "RepeatImpProcessor.h"
#include "StripUnreferencedTypesProcessor.h"
#include "MarkReferencedTypesProcessor.h"
#include "TypeExtensionProcessor.h"
#include "InsertVarInitStmtsProcessor.h"
#include "SVClassGenerator.h"
#include "XMLWriterProcessor.h"
#include "Rules2AutProcessor.h"
#include "ExecTraceGenerator.h"
#include "ActivityInferrencingProcessor.h"
#include "FlattenParallelProcessor.h"
#include "ImportWrapperGeneratorProcessor.h"
#include "ParallelImpProcessor.h"
#include "PSIFileUtils.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <stdio.h>
#include "PSI2XML.h"

using namespace psi::apps;

namespace qpssc {

QPSSC::QPSSC(const std::string &outdir) {
	m_outdir = outdir;
	m_debug = false;
	m_type_extension_proc = 0;

	init_chain();
}

QPSSC::~QPSSC() {
	// TODO Auto-generated destructor stub
}

void QPSSC::add_processor(
		IModelProcessor 		*processor,
		QPSSC::ProcessPhase		phase) {
	switch (phase) {
	case ProcessPhase_TypeExtension:
		for (std::vector<IModelProcessor *>::iterator it=m_proc_chain.begin();
				it!=m_proc_chain.end(); it++) {
			if (*it == m_type_extension_proc) {
				m_proc_chain.insert(it+1, processor);
				break;
			}
		}
		break;
	case ProcessPhase_End:
		m_proc_chain.push_back(processor);
		break;

	default:
		fprintf(stdout, "Error: unknown process phase %d\n", phase);
	}
}

bool QPSSC::process(
		IModel 							*model,
		const std::string				&component,
		const std::string				&action,
		const std::vector<std::string>	&pkgs
		) {
	bool ret = false;
	IComponent *component_h = find_component(model, component);
	IAction *action_h = 0;

	if (!component_h) {
		error("Failed to find component \"%s\"", component.c_str());
		return false;
	}

	// Mark the component as referenced
	component_h->setAttribute("REFERENCED", "true");
	component_h->setAttribute("ROOT", "true");

	// Now, locate the target action
	for (std::vector<IBaseItem *>::const_iterator it=component_h->getItems().begin();
			it!=component_h->getItems().end(); it++) {
		IBaseItem *item = *it;
		if (item->getType() == IBaseItem::TypeAction &&
				dynamic_cast<IAction *>(item)->getName() == action) {
			action_h = dynamic_cast<IAction *>(item);
			break;
		}
	}

	if (!action_h) {
		error("Failed to find action %s", action.c_str());
		return false;
	}

	// Mark the action as 'referenced'
	action_h->setAttribute("REFERENCED", "true");
	action_h->setAttribute("ROOT", "true");

	// Next, mark the specified packages as 'referenced'
	for (std::vector<std::string>::const_iterator pit=pkgs.begin();
			pit!=pkgs.end(); pit++) {
		const std::string &pkg = *pit;
		IPackage *pkg_h = 0;
		for (std::vector<IBaseItem *>::const_iterator it=model->getItems().begin();
				it!=model->getItems().end(); it++) {
			IBaseItem *item = *it;
			if (item->getType() == IBaseItem::TypePackage &&
					dynamic_cast<IPackage *>(item)->getName() == pkg) {
				pkg_h = dynamic_cast<IPackage *>(item);
				break;
			}
		}

		if (!pkg_h) {
			error("Failed to find package %s", pkg.c_str());
			return false;
		}
		pkg_h->setAttribute("REFERENCED", "true");
	}

	for (uint32_t i=0; i<m_proc_chain.size(); i++) {
		IModelProcessor *p = m_proc_chain.at(i);
//		fprintf(stdout, "--> running chain %d\n", i);
		ret = p->process(model);
//		fprintf(stdout, "<-- running chain %d\n", i);

		if (!ret) {
			fprintf(stdout, "Processor %d returned false\n", i);
			return false;
		}
	}

	return true;
}

void QPSSC::init_chain() {
	m_proc_chain.clear();

	// Mark referenced items
	m_proc_chain.push_back(new MarkReferencedTypesProcessor());

	// Add the inFact package with built-ins
	m_proc_chain.push_back(new CreateBuiltinFuncsProcessor());

	// Apply type extension
	m_type_extension_proc = new TypeExtensionProcessor();
	m_proc_chain.push_back(m_type_extension_proc);

	// Add the processor that builds out inferred actions
	// Add activity-inferincing processor
	m_activity_inferencing_proc = new ActivityInferrencingProcessor();
	m_proc_chain.push_back(m_activity_inferencing_proc);

	// ?
//	m_proc_chain.push_back(new ImportWrapperGeneratorProcessor(m_outdir));

	// Remove unreferenced and empty elements
//	m_proc_chain.push_back(new StripUnreferencedTypesProcessor());


	// Add the processor that inserts repeat-implementation artifacts
	m_proc_chain.push_back(new RepeatImpProcessor());

	if (m_debug) {
		m_proc_chain.push_back(new XMLWriterProcessor(
				m_outdir + "/03_model_post_unref.xml"));
	}

	// Add the processor that inserts code in the 'pre_solve' blocks
	m_proc_chain.push_back(new InsertVarInitStmtsProcessor());

	// Add the processor that implements parallel statements
	m_proc_chain.push_back(new ParallelImpProcessor());

	// Add the processor to transform parallel specification
	// to a flat representation
	m_proc_chain.push_back(new FlattenParallelProcessor());

	// Add the Rules output generator
	PSI2Rules *rules_gen = new PSI2Rules(m_outdir);
	m_proc_chain.push_back(rules_gen);

	// Add a transform to compile the rules file
	m_proc_chain.push_back(new Rules2AutProcessor(m_outdir));

//	switch (style) {
//	case OutputStyle_Rules:
//		// Already output rules, so no dedicated output
//		break;
//
//	case OutputStyle_SVClass:
//		m_proc_chain.push_back(new SVClassGenerator(m_outdir));
//		break;
//
//	case OutputStyle_ExecTrace:
//		m_proc_chain.push_back(new ExecTraceGenerator(m_outdir));
//		break;
//
//	default:
//		return false;
//	}

//	if (m_debug) {
//		m_proc_chain.push_back(new XMLWriterProcessor(
//				m_outdir + "/04_model_final.xml"));
//	}
}

IComponent *QPSSC::find_component(IScopeItem *scope, const std::string &name) {
	IComponent *ret = 0;

	// First, locate the named component and action and mark them 'referenced'
	for (std::vector<IBaseItem *>::const_iterator it=scope->getItems().begin();
			it!=scope->getItems().end(); it++) {
		IBaseItem *item = *it;
		if (item->getType() == IBaseItem::TypePackage) {
			if ((ret=find_component(dynamic_cast<IPackage *>(item), name))) {
				break;
			}
		} else if (item->getType() == IBaseItem::TypeComponent &&
				dynamic_cast<IComponent *>(item)->getName() == name) {
			ret = dynamic_cast<IComponent *>(item);
			break;
		}
	}

	return ret;
}
void QPSSC::error(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	fprintf(stdout, "Error: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");

	va_end(ap);
}

} /* namespace qpssc */
