/*
 * Model2XML.h
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
 * 
 * Traverses a Semantic Model description using the IModel API
 * and writes an XML file as output
 *
 *  Created on: May 26, 2018
 *      Author: ballance
 */
#pragma once
#include <string>
#include <vector>
#include <stdint.h>

#include "ModelVisitor.h"
#include "IModel.h"
#include "IBaseItem.h"



class Model2XML : public virtual ModelVisitor {
public:
	Model2XML();

	virtual ~Model2XML();

	void setUseFixedInlineAddr() { m_fixed_inline_addr = true; }

	virtual const std::string &traverse(IModel *model);


private:

	void process_pkg(IPackage *pkg);

	void process_action(IAction *a);

	void process_bind(IBind *b);

	void process_body(
			const std::vector<IBaseItem *>  &items,
			const std::string				&ctxt);

	void process_exec(IExec *exec);

	void process_struct(IStruct *str);

	void process_component(IComponent *c);

	void process_comp_pkg_body(const std::vector<IBaseItem *> &items);

	void process_constraint(IConstraint *c);

	void process_constraint_set(IConstraint *c, const char *tag=0);

	void process_constraint_block(IConstraintBlock *block);

	void process_expr(IExpr *e, const char *tag=0);

	void process_extend(IExtend *e);

	void process_field(IField *f);

	void process_graph(IActivityStmt *activity);

	void process_graph_stmt(IActivityStmt *stmt, const char *tag=0);

	void process_graph_block_stmt(IActivityBlockStmt *block, const char *tag=0);

	void process_import_func(IImportFunc *f, const std::string &tag="import_function");

	static std::string type2string(IBaseItem *it);

	void type2hierarchical_id(IBaseItem *it, const std::string &tag="pss:type");

	void type2data_type(IBaseItem *it, const std::string &tag="pss:type");

	void process_fieldref(IFieldRef *ref, const std::string &tag="ref");

	void to_hierarchical_id(const std::vector<IBaseItem *> &path, const char *tag=0);

	static std::string path2string(IFieldRef *f);

	void println(const std::string &str);

	void inc_indent();
	void dec_indent();

	void enter(const std::string &str);

	void exit(const std::string &str);

	void error(const char *fmt, ...);

	static INamedItem *toNamedItem(IBaseItem *it);

private:
	std::string					m_ind;
	uint32_t					m_ind_incr;
	std::string					m_content;
	bool						m_fixed_inline_addr;


};


