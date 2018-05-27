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

#ifndef CONTRIB_API_IMPL_ITEMFACTORYIMPL_H_
#define CONTRIB_API_IMPL_ITEMFACTORYIMPL_H_
#include "IItemFactory.h"



class ItemFactoryImpl : public virtual IItemFactory {
public:
	ItemFactoryImpl();

	virtual ~ItemFactoryImpl();

	/**
	 * Data pss_types
	 */

	virtual IArrayType *mkArrayType(
			IBaseItem				*target,
			bool					has_sum,
			IExpr					*lhs,
			IExpr					*rhs);
	/**
	 * Creates a scalar type. The msb and lsb parameters are ignored for types
	 * other than pss_int and pss_bit
	 */
	virtual IScalarType *mkScalarType(
			IScalarType::ScalarType t,
			IExpr					*msb,
			IExpr					*lsb,
			IOpenRangeList			*domain);

	/**
	 * action
	 */
	virtual IAction *mkAction(const std::string &name, IBaseItem *super_type);

	virtual IBind *mkBind(const std::vector<IBindPath *> &targets);

	virtual IBindPath *mkBindPath(const std::vector<IBaseItem *> &path);

	virtual IComponent *mkComponent(const std::string &name, IBaseItem *super_type);

	virtual ICoverspec *mkCoverspec(const std::string &name);

	virtual IExec *mkTargetTemplateExec(
			IExec::ExecKind			kind,
			const std::string		&language,
			const std::string		&text,
			const std::vector<IExecReplacementExpr *> &replacements);

	virtual IExecReplacementExpr *mkExecReplacementExpr(
			IExpr					*expr,
			uint32_t				offset,
			uint32_t				length);

	virtual IExec *mkInlineExec(
			IExec::ExecKind			kind,
			IExecCallback				*exec);

	virtual IExec *mkNativeExec(
			IExec::ExecKind					kind,
			const std::vector<IExecStmt *>	&stmts);

	virtual IExecExprStmt *mkExecExprStmt(
			IExpr					*lhs,
			IExecExprStmt::AssignOp	op,
			IExpr					*rhs);

	virtual IEnumerator *mkEnumerator(
			const std::string		&name,
			IExpr					*value);

	virtual IEnumType *mkEnumType(
			const std::string					&name,
			const std::vector<IEnumerator *>	&enumerators);


	virtual IMethodCallExpr *mkMethodCallExpr(
			IRefType					*func,
			const std::vector<IExpr *>	&parameters);

	virtual IVariableRef *mkVariableRef(
			IBaseItem					*scope,
			const std::string			&id,
			IExpr						*index_lhs,
			IExpr						*index_rhs);

	virtual IStruct *mkStruct(
			const std::string 		&name,
			IStruct::StructType		t,
			IBaseItem 				*super_type);

	virtual ISymbol *mkSymbol(
			const std::string			&name,
			const std::vector<IField *>	&params,
			IActivityBlockStmt				*body);

	virtual IExtendComposite *mkExtendComposite(
			IExtend::ExtendType		type,
			IBaseItem 				*target);

	virtual IExtendEnum *mkExtendEnum(
			IBaseItem							*target,
			const std::vector<IEnumerator *>	&enumerators);

	/**
	 * Create a field for use in declaring the contents of an
	 * action or struct data type
	 */
	virtual IField *mkField(
			const std::string		&name,
			IBaseItem				*field_type,
			IField::FieldAttr		attr,
			IExpr					*array_dim);

	virtual IPool *mkPool(
			const std::string		&name,
			IBaseItem				*pool_type,
			IExpr					*pool_size);

	virtual IActivityBlockStmt *mkActivityBlockStmt(
			IActivityStmt::ActivityStmtType type=IActivityStmt::ActivityStmt_Block);

	virtual IActivityTraverseStmt *mkActivityTraverseStmt(
			IVariableRef *action, IConstraint *with_c=0);

	virtual IActivityDoActionStmt *mkActivityDoActionStmt(
			IBaseItem *target, IConstraintBlock *with_c);

	virtual IActivityIfElseStmt *mkActivityIfElseStmt(
			IExpr			*cond,
			IActivityStmt	*true_stmt,
			IActivityStmt	*false_stmt);

	virtual IActivityRepeatStmt *mkActivityRepeatStmt(
			IActivityRepeatStmt::RepeatType type,
			IExpr *expr, IActivityStmt *body);

	virtual IActivitySelectStmt *mkActivitySelectStmt(
			const std::vector<IActivitySelectBranchStmt *> &branches);

	virtual IActivitySelectBranchStmt *mkActivitySelectBranchStmt(
			IActivityStmt			*stmt,
			IExpr					*guard,
			IExpr					*weight);

	virtual IOpenRangeValue *mkOpenRangeValue(
			IExpr 					*lhs,
			IExpr 					*rhs,
			bool					domain_bound);

	virtual IOpenRangeList *mkOpenRangeList(
			const std::vector<IOpenRangeValue *>	&ranges);

	virtual IBinaryExpr *mkBinExpr(
			IExpr 					*lhs,
			IBinaryExpr::BinOpType	op,
			IExpr 					*rhs);

	virtual IInExpr *mkInExpr(
			IExpr					*lhs,
			IOpenRangeList			*rhs);

	virtual IFieldRef *mkFieldRef(
			const std::vector<IField *>		&field_path);

	virtual ILiteral *mkIntLiteral(int64_t v);

	virtual ILiteral *mkBitLiteral(uint64_t v);

	virtual ILiteral *mkBoolLiteral(bool v);

	virtual ILiteral *mkStringLiteral(const std::string &v);

	virtual IRefType *mkRefType(
			IScopeItem						*scope,
			const std::vector<std::string>	&type);

	virtual IConstraintBlock *mkConstraintBlock(
			const std::string 		&name,
			IConstraint				*c);

	virtual IConstraintBlock *mkConstraintBlock(
			const std::string 					&name,
			const std::vector<IConstraint *>	&cl);

	virtual IConstraintExpr *mkConstraintExpr(IExpr *expr);

	virtual IConstraintForeach *mkConstraintForeach(
			IVariableRef			*target,
			const std::string		&iterator,
			IConstraintBlock		*body);

	virtual IConstraintIf *mkConstraintIf(
			IExpr 			*cond,
			IConstraint 	*true_c,
			IConstraint 	*false_c);

	virtual IConstraintImplies *mkConstraintImplies(
			IExpr			*cond,
			IConstraint		*imp);

	virtual IConstraintUnique *mkConstraintUnique(
			IOpenRangeList					*target);

	virtual IImport *mkImport(
			IBaseItem						*target,
			bool							is_wildcard);

	virtual IImportFunc *mkImportFunc(
			const std::string				&name,
			IBaseItem						*ret_type,
			const std::vector<IField *>		&parameters);
};

#endif /* CONTRIB_API_IMPL_ITEMFACTORYIMPL_H_ */
