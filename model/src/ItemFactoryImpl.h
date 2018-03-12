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

using namespace psi_api;

class ItemFactoryImpl : public virtual IItemFactory {
public:
	ItemFactoryImpl();

	virtual ~ItemFactoryImpl();

	/**
	 * Data pss_types
	 */

	/**
	 * Creates a scalar type. The msb and lsb parameters are ignored for types
	 * other than pss_int and pss_bit
	 */
	virtual IScalarType *mkScalarType(
			IScalarType::ScalarType t,
			IExpr					*msb,
			IExpr					*lsb);

	/**
	 * action
	 */
	virtual IAction *mkAction(const std::string &name, IBaseItem *super_type);

	virtual IBind *mkBind(const std::vector<IBindPath *> &targets);

	virtual IBindPath *mkBindPath(const std::vector<IBaseItem *> &path);

	virtual IComponent *mkComponent(const std::string &name, IBaseItem *super_type);

	virtual IExec *mkTargetTemplateExec(
			IExec::ExecKind			kind,
			const std::string		&language,
			const std::string		&text);

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

	virtual IMethodCallExpr *mkMethodCallExpr(
			IImportFunc					*func,
			const std::vector<IExpr *>	&parameters);

	virtual IStruct *mkStruct(
			const std::string 		&name,
			IStruct::StructType		t,
			IBaseItem 				*super_type);

	virtual IExtend *mkExtend(IBaseItem *target);

	/**
	 * Create a field for use in declaring the contents of an
	 * action or struct data type
	 */
	virtual IField *mkField(
			const std::string		&name,
			IBaseItem				*field_type,
			IField::FieldAttr		attr,
			IExpr					*array_dim);

	virtual IGraphBlockStmt *mkGraphBlockStmt(
			IGraphStmt::GraphStmtType type=IGraphStmt::GraphStmt_Block);

	virtual IGraphTraverseStmt *mkGraphTraverseStmt(
			IFieldRef *action, IConstraint *with_c=0);

	virtual IGraphRepeatStmt *mkGraphRepeatStmt(
			IGraphRepeatStmt::RepeatType type,
			IExpr *expr, IGraphStmt *body);


	virtual IBinaryExpr *mkBinExpr(
			IExpr 					*lhs,
			IBinaryExpr::BinOpType	op,
			IExpr 					*rhs);

	virtual IRefExpr *mkRefExpr(
			IScopeItem						*scope,
			const std::vector<std::string>	&path);

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

	virtual IConstraintIf *mkConstraintIf(
			IExpr 			*cond,
			IConstraint 	*true_c,
			IConstraint 	*false_c);

	virtual IConstraintImplies *mkConstraintImplies(
			IExpr			*cond,
			IConstraint		*imp);

	virtual IImportFunc *mkImportFunc(
			const std::string				&name,
			IBaseItem						*ret_type,
			const std::vector<IField *>		&parameters);
};

#endif /* CONTRIB_API_IMPL_ITEMFACTORYIMPL_H_ */
