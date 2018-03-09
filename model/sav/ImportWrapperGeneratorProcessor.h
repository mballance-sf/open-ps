/*
 * ImportWrapperGeneratorProcessor.h
 *
 *  Created on: Dec 2, 2017
 *      Author: ballance
 */

#ifndef QPSSC_SRC_IMPORTWRAPPERGENERATORPROCESSOR_H_
#define QPSSC_SRC_IMPORTWRAPPERGENERATORPROCESSOR_H_

#include "IModelProcessor.h"
#include "PSIVisitor.h"
#include <string>

class ImportWrapperGeneratorProcessor:
		public qpssc::IModelProcessor, public psi::apps::PSIVisitor {
public:
	ImportWrapperGeneratorProcessor(const std::string &outdir);

	virtual ~ImportWrapperGeneratorProcessor();

	virtual bool process(psi_api::IModel *model);

	virtual void visit_import_func(IImportFunc *f);

private:
	std::string				m_outdir;
	std::string				m_wrappers;
	std::string				m_registration;
	FILE					*m_out;


};

#endif /* QPSSC_SRC_IMPORTWRAPPERGENERATORPROCESSOR_H_ */
