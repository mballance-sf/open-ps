/*
 * ResolveRefsProcessor.h
 *
 *  Created on: Mar 12, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_RESOLVEREFSPROCESSOR_H_
#define PROCESSOR_SRC_RESOLVEREFSPROCESSOR_H_

#include "IModelProcessor.h"
#include "PSIVisitor.h"
#include <vector>
#include <string>

using namespace psi_api;
using namespace psi::apps;

class ResolveRefsProcessor:
		public PSIVisitor,
		public virtual qpssc::IModelProcessor {
public:
	ResolveRefsProcessor();

	virtual ~ResolveRefsProcessor();

	virtual bool process(IModel *model);

protected:

	virtual void visit_ref_type(IRefType *ref) override;

private:
	IBaseItem *find_type(const std::vector<std::string> &type);

	IBaseItem *find_type(const std::string &name);

	IBaseItem *find_type(IScopeItem *scope, const std::string &name);

	static bool is_type(IBaseItem *it);

	static bool is_type(IScopeItem *it);

	void debug(const char *fmt, ...);

	void todo(const char *fmt, ...);

	std::string type2string(const std::vector<std::string> &type);

private:

	bool				m_debug;


};

#endif /* PROCESSOR_SRC_RESOLVEREFSPROCESSOR_H_ */