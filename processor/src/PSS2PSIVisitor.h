/*
 * PSS2PSIVisitor.h
 *
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#ifndef PSS2PSI_SRC_PSS2PSIVISITOR_H_
#define PSS2PSI_SRC_PSS2PSIVISITOR_H_
#include "PSSBaseVisitor.h"
#include "IModel.h"
#include <vector>

using namespace psi_api;

class PSS2PSIVisitor : public PSSBaseVisitor {
public:

	PSS2PSIVisitor(IModel *model, const std::string &path);

	virtual ~PSS2PSIVisitor();

	antlrcpp::Any visitModel(PSSParser::ModelContext *ctx) override;
	antlrcpp::Any visitAction_declaration(PSSParser::Action_declarationContext *ctx) override;
	antlrcpp::Any visitActivity_declaration(PSSParser::Activity_declarationContext *ctx) override;
	antlrcpp::Any visitActivity_action_traversal_stmt(PSSParser::Activity_action_traversal_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_parallel_stmt(PSSParser::Activity_parallel_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_repeat_stmt(PSSParser::Activity_repeat_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_select_stmt(PSSParser::Activity_select_stmtContext *ctx) override;
	antlrcpp::Any visitActivity_sequence_block_stmt(PSSParser::Activity_sequence_block_stmtContext *ctx) override;
	antlrcpp::Any visitStruct_declaration(PSSParser::Struct_declarationContext *ctx) override;
	antlrcpp::Any visitStruct_field_declaration(PSSParser::Struct_field_declarationContext *ctx) override;
	antlrcpp::Any visitAction_field_declaration(PSSParser::Action_field_declarationContext *ctx) override;
	antlrcpp::Any visitAction_data_type(PSSParser::Action_data_typeContext *ctx) override;
	antlrcpp::Any visitData_type(PSSParser::Data_typeContext *ctx) override;
	antlrcpp::Any visitScalar_data_type(PSSParser::Scalar_data_typeContext *ctx) override;
	antlrcpp::Any visitComponent_declaration(PSSParser::Component_declarationContext *ctx) override;
	antlrcpp::Any visitComponent_field_declaration(PSSParser::Component_field_declarationContext *ctx) override;
	antlrcpp::Any visitComponent_data_declaration(PSSParser::Component_data_declarationContext *ctx) override;
	antlrcpp::Any visitComponent_pool_declaration(PSSParser::Component_pool_declarationContext *ctx) override;
	antlrcpp::Any visitConstraint_declaration(PSSParser::Constraint_declarationContext *ctx) override;
	antlrcpp::Any visitExpression_constraint_item(PSSParser::Expression_constraint_itemContext *ctx) override;
	antlrcpp::Any visitIf_constraint_item(PSSParser::If_constraint_itemContext *ctx) override;
	antlrcpp::Any visitConstraint_set(PSSParser::Constraint_setContext *ctx) override;
	antlrcpp::Any visitConstraint_block(PSSParser::Constraint_blockContext *ctx) override;
	antlrcpp::Any visitPackage_declaration(PSSParser::Package_declarationContext *ctx);
	antlrcpp::Any visitExpression(PSSParser::ExpressionContext *ctx);
	antlrcpp::Any visitPrimary(PSSParser::PrimaryContext *ctx);
	antlrcpp::Any visitExec_block_stmt(PSSParser::Exec_block_stmtContext *ctx);
	antlrcpp::Any visitExec_body_stmt(PSSParser::Exec_body_stmtContext *ctx);
	antlrcpp::Any visitExtend_stmt(PSSParser::Extend_stmtContext *ctx);
	antlrcpp::Any visitMethod_prototype(PSSParser::Method_prototypeContext *ctx);
	antlrcpp::Any visitFunction_decl(PSSParser::Function_declContext *ctx) override;
	antlrcpp::Any visitUser_defined_datatype(PSSParser::User_defined_datatypeContext *ctx);

	IFieldRef *elaborate_field_ref(
			const std::vector<PSSParser::Variable_refContext *> &path);

	IImportFunc *find_import_func(
			PSSParser::Function_callContext *func
			);

	IBaseItem *find_type(PSSParser::Type_identifierContext *type);


	virtual IExpr *expression(PSSParser::ExpressionContext *ctxt);

	void push_scope(IScopeItem *scope);

	IScopeItem *scope() const;

	IScopeItem *pop_scope();

	static IScopeItem *getSuperType(IScopeItem *it);

private:

	std::pair<IBaseItem *, IScopeItem *> find_type(const std::string &name);

	static IBaseItem *find_type(IScopeItem *scope, const std::string &name);

	std::vector<std::string> type2vector(PSSParser::Type_identifierContext *type);


	void fatal(const std::string &msg);

	void fatal(antlr4::Token *loc, const std::string &msg);

private:

	IModel						*m_model;
	IItemFactory				*m_factory;
	std::string					 m_file;
	std::vector<IScopeItem *>	m_scopes;
	IExpr						*m_expr;
};

#endif /* PSS2PSI_SRC_PSS2PSIVISITOR_H_ */
