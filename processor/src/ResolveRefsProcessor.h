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




class ResolveRefsProcessor:
		public PSIVisitor,
		public virtual qpssc::IModelProcessor {
public:
	ResolveRefsProcessor();

	virtual ~ResolveRefsProcessor();

	virtual bool process(IModel *model);

protected:

	virtual void visit_ref_type(IRefType *ref) override;

	virtual void visit_variable_ref(IVariableRef *ref) override;

private:

	void resolve_variable_ref(
			IScopeItem					*scope,
			IVariableRef				*full_ref,
			IVariableRef				*ref
			);

	IBaseItem *resolve_variable_ref(
			IScopeItem			*scope,
			const std::string	&id,
			bool				search_upscope=false);

	IBaseItem *resolve_variable_ref_in_ext(
			IScopeItem			*scope,
			const std::string	&id);

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
	uint32_t			m_phase;


};

#endif /* PROCESSOR_SRC_RESOLVEREFSPROCESSOR_H_ */
