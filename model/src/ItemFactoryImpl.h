/*
 * ItemFactoryImpl.h
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
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
 *  Created on: Aug 11, 2016
 *      Author: ballance
 */

#pragma once
#include "IItemFactory.h"


class ItemFactoryImpl : public virtual IItemFactory {
public:
	ItemFactoryImpl();

	virtual ~ItemFactoryImpl();

	virtual IModel *mkModel();

	/**
	 * Data pss_types
	 */

	virtual IArrayType *mkArrayType(
			IBaseItem				*target,
			bool					has_sum,
			IExpr					*lhs,
			IExpr					*rhs) override;
	/**
	 * Creates a scalar type. The msb and lsb parameters are ignored for types
	 * other than pss_int and pss_bit
	 */
	virtual IScalarType *mkScalarType(
			IScalarType::ScalarType t,
			IExpr					*msb,
			IExpr					*lsb,
			IOpenRangeList			*domain) override;

	/**
	 * action
	 */
	virtual IAction *mkAction(const std::string &name, IBaseItem *super_type) override;

	virtual IBind *mkBind(const std::vector<IBindPath *> &targets) override;

	virtual IBindPath *mkBindPath(const std::vector<IBaseItem *> &path) override;

	virtual IComponent *mkComponent(const std::string &name, IBaseItem *super_type) override;

	virtual ICoverspec *mkCoverspec(const std::string &name) override;

	virtual IExec *mkTargetTemplateExec(
			IExec::ExecKind			kind,
			const std::string		&language,
			const std::string		&text,
			const std::vector<IExecReplacementExpr *> &replacements) override;

	virtual IExecReplacementExpr *mkExecReplacementExpr(
			IExpr					*expr,
			uint32_t				offset,
			uint32_t				length) override;

	virtual IExec *mkInlineExec(
			IExec::ExecKind			kind,
			IExecCallback				*exec) override;

	virtual IExec *mkNativeExec(
			IExec::ExecKind					kind,
			const std::vector<IExecStmt *>	&stmts) override;

	virtual IExecExprStmt *mkExecExprStmt(
			IExpr					*lhs,
			IExecExprStmt::AssignOp	op,
			IExpr					*rhs) override;

	virtual IEnumerator *mkEnumerator(
			const std::string		&name,
			IExpr					*value) override;

	virtual IEnumType *mkEnumType(
			const std::string					&name,
			const std::vector<IEnumerator *>	&enumerators) override;


	virtual IMethodCallExpr *mkMethodCallExpr(
			IRefType					*func,
			const std::vector<IExpr *>	&parameters) override;

	virtual IVariableRef *mkVariableRef(
			IBaseItem					*scope,
			const std::string			&id,
			IExpr						*index_lhs,
			IExpr						*index_rhs) override;

	virtual IStruct *mkStruct(
			const std::string 		&name,
			IStruct::StructType		t,
			IBaseItem 				*super_type) override;

	virtual ISymbol *mkSymbol(
			const std::string			&name,
			const std::vector<IField *>	&params,
			IActivityBlockStmt				*body) override;

	virtual IExtendComposite *mkExtendComposite(
			IExtend::ExtendType		type,
			IBaseItem 				*target) override;

	virtual IExtendEnum *mkExtendEnum(
			IBaseItem							*target,
			const std::vector<IEnumerator *>	&enumerators) override;

	/**
	 * Create a field for use in declaring the contents of an
	 * action or struct data type
	 */
	virtual IField *mkField(
			const std::string		&name,
			IBaseItem				*field_type,
			IField::FieldAttr		attr,
			IExpr					*array_dim) override;

	virtual IPool *mkPool(
			const std::string		&name,
			IBaseItem				*pool_type,
			IExpr					*pool_size) override;

	virtual IActivityBlockStmt *mkActivityBlockStmt(
			IActivityStmt::ActivityStmtType type=IActivityStmt::ActivityStmt_Block) override;

	virtual IActivityTraverseStmt *mkActivityTraverseStmt(
			IVariableRef *action, IConstraint *with_c=0) override;

	virtual IActivityDoActionStmt *mkActivityDoActionStmt(
			IBaseItem *target, IConstraintBlock *with_c) override;

	virtual IActivityIfElseStmt *mkActivityIfElseStmt(
			IExpr			*cond,
			IActivityStmt	*true_stmt,
			IActivityStmt	*false_stmt) override;

	virtual IActivityRepeatStmt *mkActivityRepeatStmt(
			IActivityRepeatStmt::RepeatType type,
			IExpr *expr, IActivityStmt *body) override;

	virtual IActivitySelectStmt *mkActivitySelectStmt(
			const std::vector<IActivitySelectBranchStmt *> &branches) override;

	virtual IActivitySelectBranchStmt *mkActivitySelectBranchStmt(
			IActivityStmt			*stmt,
			IExpr					*guard,
			IExpr					*weight) override;

	virtual IOpenRangeValue *mkOpenRangeValue(
			IExpr 					*lhs,
			IExpr 					*rhs,
			bool					domain_bound) override;

	virtual IOpenRangeList *mkOpenRangeList(
			const std::vector<IOpenRangeValue *>	&ranges) override;

	virtual IBinaryExpr *mkBinExpr(
			IExpr 					*lhs,
			IBinaryExpr::BinOpType	op,
			IExpr 					*rhs) override;

	virtual IInExpr *mkInExpr(
			IExpr					*lhs,
			IOpenRangeList			*rhs) override;

	virtual ILiteral *mkIntLiteral(int64_t v) override;

	virtual ILiteral *mkBitLiteral(uint64_t v) override;

	virtual ILiteral *mkBoolLiteral(bool v) override;

	virtual ILiteral *mkStringLiteral(const std::string &v) override;

	virtual IRefType *mkRefType(
			IScopeItem						*scope,
			const std::vector<std::string>	&type,
			bool							fully_qualified) override;

	virtual IConstraintBlock *mkConstraintBlock(
			const std::string 		&name,
			IConstraint				*c) override;

	virtual IConstraintBlock *mkConstraintBlock(
			const std::string 					&name,
			const std::vector<IConstraint *>	&cl) override;

	virtual IConstraintExpr *mkConstraintExpr(IExpr *expr) override;

	virtual IConstraintForeach *mkConstraintForeach(
			IVariableRef			*target,
			const std::string		&iterator,
			IConstraintBlock		*body) override;

	virtual IConstraintIf *mkConstraintIf(
			IExpr 			*cond,
			IConstraint 	*true_c,
			IConstraint 	*false_c) override;

	virtual IConstraintImplies *mkConstraintImplies(
			IExpr			*cond,
			IConstraint		*imp) override;

	virtual IConstraintUnique *mkConstraintUnique(
			IOpenRangeList					*target) override;

	virtual IImport *mkImport(
			IBaseItem						*target,
			bool							is_wildcard) override;

	virtual IImportFunc *mkImportFunc(
			const std::string				&name,
			IBaseItem						*ret_type,
			const std::vector<IField *>		&parameters) override;
};

