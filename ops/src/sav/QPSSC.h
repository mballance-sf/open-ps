/*
 * QPSSC.h
 *
 *  Created on: Aug 22, 2016
 *      Author: ballance
 */

#ifndef SRC_QPSSC_H_
#define SRC_QPSSC_H_
#include "IModel.h"
#include "IModelProcessor.h"
#include <vector>

using namespace psi_api;

namespace qpssc {

class ExecTraceGenerator;
class QPSSC {
public:
	enum OutputStyle {
		OutputStyle_Rules,
		OutputStyle_SVClass,
		OutputStyle_ExecTrace
	};
	enum ProcessPhase {
		ProcessPhase_TypeExtension,
		ProcessPhase_End
	};
public:

	QPSSC(const std::string &outdir);

	virtual ~QPSSC();

	void setDebug(bool en) { m_debug = en; }

	void add_processor(
			IModelProcessor 		*processor,
			QPSSC::ProcessPhase		phase = QPSSC::ProcessPhase_End);

	const std::string &get_outdir() const { return m_outdir; }

	/**
	 * Processes the model in a destructive fashion
	 */
	bool process(
			IModel 							*model,
			const std::string				&component,
			const std::string				&action,
			const std::vector<std::string>	&pkgs
			);

private:

	void init_chain();

	static IComponent *find_component(
			IScopeItem			*scope,
			const std::string 	&name);

	void error(const char *fmt, ...);

private:
	std::string							m_outdir;
	std::vector<IModelProcessor *>		m_proc_chain;
	bool								m_debug;
	IModelProcessor						*m_type_extension_proc;
	IModelProcessor						*m_activity_inferencing_proc;

};

} /* namespace qpssc */

#endif /* SRC_QPSSC_H_ */
