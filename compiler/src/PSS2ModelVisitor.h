/*
 * PSS2ModelVisitor.h
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
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#pragma once
#include "PSSBaseVisitor.h"
#include "IModel.h"
#include <vector>



class PSS2ModelVisitor : public PSSBaseVisitor {
public:

	PSS2ModelVisitor(IModel *model, const std::string &path);

	virtual ~PSS2ModelVisitor();

	antlrcpp::Any visitModel(PSSParser::ModelContext *ctx) override;
	antlrcpp::Any visitPackage_declaration(PSSParser::Package_declarationContext *ctx) override;
	antlrcpp::Any visitNull_stmt(PSSParser::Null_stmtContext *ctx) override;
	antlrcpp::Any visitConst_field_declaration(PSSParser::Const_field_declarationContext *ctx) override;
	antlrcpp::Any visitStatic_const_field_declaration(PSSParser::Static_const_field_declarationContext *ctx) override;
	antlrcpp::Any visitExtend_stmt(PSSParser::Extend_stmtContext *ctx);
	antlrcpp::Any visitImport_stmt(PSSParser::Import_stmtContext *ctx) override;
	antlrcpp::Any visitAction_declaration(PSSParser::Action_declarationContext *ctx) override;
	antlrcpp::Any visitAbstract_action_declaration(PSSParser::Abstract_action_declarationContext *ctx) override;
	antlrcpp::Any visitActivity_declaration(PSSParser::Activity_declarationContext *ctx) override;
	antlrcpp::Any visitAttr_field(PSSParser::Attr_fieldContext *ctx) override;
	antlrcpp::Any visitAttr_group(PSSParser::Attr_groupContext *ctx) override;
//	antlrcpp::Any visitAction_field_declaration(PSSParser::Action_field_declarationContext *ctx) override;
	antlrcpp::Any visitSub_action_field(PSSParser::Sub_action_fieldContext *ctx) override;
	antlrcpp::Any visitFlow_ref_field(PSSParser::Flow_ref_fieldContext *ctx) override;
	antlrcpp::Any visitResource_ref_field(PSSParser::Resource_ref_fieldContext *ctx) override;
	antlrcpp::Any visitActivity_data_field(PSSParser::Activity_data_fieldContext *ctx) override;
	antlrcpp::Any visitScheduling_constraint(PSSParser::Scheduling_constraintContext *ctx) override;
	antlrcpp::Any visitExec_block(PSSParser::Exec_blockContext *ctx) override;
	antlrcpp::Any visitStruct_declaration(PSSParser::Struct_declarationContext *ctx) override;
	antlrcpp::Any visitFunction_decl(PSSParser::Function_declContext *ctx) override;
	antlrcpp::Any visitMethod_prototype(PSSParser::Method_prototypeContext *ctx);
	antlrcpp::Any visitImport_method_phase_qualifiers(PSSParser::Import_method_phase_qualifiersContext *ctx) override;
	antlrcpp::Any visitImport_class_decl(PSSParser::Import_class_declContext *ctx) override;
	antlrcpp::Any visitExport_action(PSSParser::Export_actionContext *ctx) override;
	antlrcpp::Any visitComponent_declaration(PSSParser::Component_declarationContext *ctx) override;
	antlrcpp::Any visitComponent_field_declaration(PSSParser::Component_field_declarationContext *ctx) override;
	antlrcpp::Any visitComponent_data_declaration(PSSParser::Component_data_declarationContext *ctx) override;
	antlrcpp::Any visitComponent_pool_declaration(PSSParser::Component_pool_declarationContext *ctx) override;
	antlrcpp::Any visitObject_bind_stmt(PSSParser::Object_bind_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_labeled_stmt(PSSParser::Activity_labeled_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_bind_stmt(PSSParser::Activity_bind_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_if_else_stmt(PSSParser::Activity_if_else_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_select_stmt(PSSParser::Activity_select_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_match_stmt(PSSParser::Activity_match_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_action_traversal_stmt(PSSParser::Activity_action_traversal_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_schedule_stmt(PSSParser::Activity_schedule_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_repeat_stmt(PSSParser::Activity_repeat_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_constraint_stmt(PSSParser::Activity_constraint_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_parallel_stmt(PSSParser::Activity_parallel_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_foreach_stmt(PSSParser::Activity_foreach_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_sequence_block_stmt(PSSParser::Activity_sequence_block_stmtContext *ctx) override;
	antlrcpp::Any visitSymbol_declaration(PSSParser::Symbol_declarationContext *ctx) override;
	antlrcpp::Any visitOverrides_declaration(PSSParser::Overrides_declarationContext *ctx) override;
	antlrcpp::Any visitData_type(PSSParser::Data_typeContext *ctx) override;
	antlrcpp::Any visitScalar_data_type(PSSParser::Scalar_data_typeContext *ctx) override;
	antlrcpp::Any visitEnum_declaration(PSSParser::Enum_declarationContext *ctx) override;
	antlrcpp::Any visitTypedef_declaration(PSSParser::Typedef_declarationContext *ctx) override;

	antlrcpp::Any visitDomain_open_range_value(PSSParser::Domain_open_range_valueContext *ctx) override;
	antlrcpp::Any visitDomain_open_range_list(PSSParser::Domain_open_range_listContext *ctx) override;
	antlrcpp::Any visitOpen_range_value(PSSParser::Open_range_valueContext *ctx) override;
	antlrcpp::Any visitOpen_range_list(PSSParser::Open_range_listContext *ctx) override;

	antlrcpp::Any visitConstraint_declaration(PSSParser::Constraint_declarationContext *ctx) override;
	antlrcpp::Any visitExpression_constraint_item(PSSParser::Expression_constraint_itemContext *ctx) override;
	antlrcpp::Any visitIf_constraint_item(PSSParser::If_constraint_itemContext *ctx) override;
	antlrcpp::Any visitForeach_constraint_item(PSSParser::Foreach_constraint_itemContext *ctx) override;
	antlrcpp::Any visitConstraint_set(PSSParser::Constraint_setContext *ctx) override;
	antlrcpp::Any visitConstraint_block(PSSParser::Constraint_blockContext *ctx) override;
	antlrcpp::Any visitUnique_constraint_item(PSSParser::Unique_constraint_itemContext *ctx) override;
	antlrcpp::Any visitCovergroup_declaration(PSSParser::Covergroup_declarationContext *ctx) override;
	antlrcpp::Any visitInline_covergroup(PSSParser::Inline_covergroupContext *ctx) override;
	antlrcpp::Any visitCovergroup_option(PSSParser::Covergroup_optionContext *ctx) override;
	antlrcpp::Any visitCovergroup_type_option(PSSParser::Covergroup_type_optionContext *ctx) override;
	antlrcpp::Any visitCovergroup_coverpoint(PSSParser::Covergroup_coverpointContext *ctx) override;

	antlrcpp::Any visitExpression(PSSParser::ExpressionContext *ctx);
	antlrcpp::Any visitPrimary(PSSParser::PrimaryContext *ctx);
	antlrcpp::Any visitExec_body_stmt(PSSParser::Exec_body_stmtContext *ctx) override;
	antlrcpp::Any visitTarget_code_exec_block(PSSParser::Target_code_exec_blockContext *ctx) override;
	antlrcpp::Any visitTarget_file_exec_block(PSSParser::Target_file_exec_blockContext *ctx) override;
	antlrcpp::Any visitUser_defined_datatype(PSSParser::User_defined_datatypeContext *ctx);

	antlrcpp::Any visitType_identifier(PSSParser::Type_identifierContext *ctx) override;

//	IFieldRef *elaborate_field_ref(
//			const std::vector<PSSParser::Variable_refContext *> &path);

	IImportFunc *find_import_func(
			PSSParser::Function_symbol_callContext *func
			);

	IBaseItem *find_type(PSSParser::Type_identifierContext *type);


	virtual IExpr *expression(PSSParser::ExpressionContext *ctxt);

	void push_scope(IScopeItem *scope);

	IScopeItem *scope() const;

	IScopeItem *pop_scope();

	static IScopeItem *getSuperType(IScopeItem *it);

	void set_debug(bool d) { m_debug = d; }

private:

	std::pair<IBaseItem *, IScopeItem *> find_type(const std::string &name);

	static IBaseItem *find_type(IScopeItem *scope, const std::string &name);

	std::vector<std::string> type2vector(PSSParser::Type_identifierContext *type);

	IRefType *mk_ref_type(PSSParser::Type_identifierContext *ctx);

	void enter(const char *fmt, ...);

	void leave(const char *fmt, ...);

	void fatal(const std::string &msg);

	void error(const char *fmt, ...);

	void todo(const char *fmt, ...);

	void debug(const char *fmt, ...);

	void fatal(antlr4::Token *loc, const std::string &msg);

private:
	bool						m_debug;
	IModel						*m_model;
	IItemFactory				*m_factory;
	std::string					 m_file;
	std::vector<IScopeItem *>	m_scopes;
	IExpr						*m_expr;
};

