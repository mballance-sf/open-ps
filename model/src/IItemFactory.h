/*
 * IItemFactory.h
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

#ifndef INCLUDED_ITEM_FACTORY_H
#define INCLUDED_ITEM_FACTORY_H
#include <vector>
#include <string>
#include <stdint.h>
#include "IBinaryExpr.h"
#include "IBind.h"
#include "IBindPath.h"
#include "IComponent.h"
#include "IConstraintBlock.h"
#include "IConstraintExpr.h"
#include "IConstraintIf.h"
#include "IExec.h"
#include "IExecStmt.h"
#include "IExecExprStmt.h"
#include "IExtend.h"
#include "IGraphBlockStmt.h"
#include "IGraphRepeatStmt.h"
#include "IGraphTraverseStmt.h"
#include "ILiteral.h"
#include "IScalarType.h"
#include "IStruct.h"
#include "IConstraintImplies.h"
#include "IImportFunc.h"
#include "IMethodCallExpr.h"
#include "IRefExpr.h"

namespace psi_api {

class IItemFactory {
public:

	virtual ~IItemFactory() { }

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
			IExpr					*lsb) = 0;

	/**
	 * action
	 */
	virtual IAction *mkAction(const std::string &name, IAction *super_type) = 0;

	/**
	 * Creates a new bind statement
	 */
	virtual IBind *mkBind(const std::vector<IBindPath *> &targets) = 0;

	virtual IBindPath *mkBindPath(const std::vector<IBaseItem *> &elems) = 0;


	/**
	 * Create a new component type
	 */
	virtual IComponent *mkComponent(const std::string &name) = 0;

	virtual IExec *mkTargetTemplateExec(
			IExec::ExecKind 		kind,
			const std::string		&language,
			const std::string 		&text) = 0;

	virtual IExec *mkInlineExec(
			IExec::ExecKind 		kind,
			IExecCallback			*exec) = 0;

	virtual IExec *mkNativeExec(
			IExec::ExecKind					kind,
			const std::vector<IExecStmt *>	&stmts) = 0;

	virtual IExecExprStmt *mkExecExprStmt(
			IExpr					*lhs,
			IExecExprStmt::AssignOp	op,
			IExpr					*rhs) = 0;

	virtual IMethodCallExpr *mkMethodCallExpr(
			IImportFunc					*func,
			const std::vector<IExpr *>	&parameters) = 0;

	/**
	 * Create a new struct type
	 */
	virtual IStruct *mkStruct(
			const std::string 		&name,
			IStruct::StructType		t,
			IStruct 				*super_type) = 0;

	virtual IExtend *mkExtend(IBaseItem	*target) = 0;

	/**
	 * Create a field for use in declaring the contents of an
	 * action or struct data type
	 */
	virtual IField *mkField(
			const std::string		&name,
			IBaseItem				*field_type,
			IField::FieldAttr		attr,
			IExpr					*array_dim) = 0;

	virtual IBinaryExpr *mkBinExpr(
			IExpr 					*lhs,
			IBinaryExpr::BinOpType	op,
			IExpr 					*rhs) = 0;

	virtual IRefExpr *mkRefExpr(
			IBaseItem						*scope,
			const std::vector<std::string>	&path);

	virtual IFieldRef *mkFieldRef(
			const std::vector<IField *>		&field_path) = 0;

	/**
	 * Creates a new block statement. parallel, schedule, pss_select, and
	 * Block statements are all, fundamentally, block statements.
	 * The value of the 'type' parameter can be
	 * GraphStmt_Block, GraphStmt_Parallel, GraphStmt_Schedule, or GraphStmt_Select
	 */
	virtual IGraphBlockStmt *mkGraphBlockStmt(
			IGraphStmt::GraphStmtType type=IGraphStmt::GraphStmt_Block) = 0;

	virtual IGraphTraverseStmt *mkGraphTraverseStmt(
			IFieldRef *action, IConstraint *with_c=0) = 0;

	virtual IGraphRepeatStmt *mkGraphRepeatStmt(
			IGraphRepeatStmt::RepeatType type,
			IExpr *expr, IGraphStmt *body) = 0;

	virtual ILiteral *mkIntLiteral(int64_t v) = 0;

	virtual ILiteral *mkBitLiteral(uint64_t v) = 0;

	virtual ILiteral *mkBoolLiteral(bool v) = 0;

	virtual ILiteral *mkStringLiteral(const std::string &v) = 0;

	/**
	 * Construct a new constraint block. The constraint block is
	 * anonymous if name="". The specified constraint will be
	 * added to the block if it is non-null
	 */
	virtual IConstraintBlock *mkConstraintBlock(
			const std::string 	&name,
			IConstraint			*c=0) = 0;

	/**
	 * Construct a new constraint block, with a list of constraints
	 */
	virtual IConstraintBlock *mkConstraintBlock(
			const std::string 					&name,
			const std::vector<IConstraint *>	&cl) = 0;

	virtual IConstraintExpr *mkConstraintExpr(IExpr *expr) = 0;

	virtual IConstraintIf *mkConstraintIf(
			IExpr 			*cond,
			IConstraint 	*true_c,
			IConstraint 	*false_c) = 0;

	virtual IConstraintImplies *mkConstraintImplies(
			IExpr			*cond,
			IConstraint		*imp) = 0;

	virtual IImportFunc *mkImportFunc(
			const std::string				&name,
			IBaseItem						*ret_type,
			const std::vector<IField *>		&parameters) = 0;

	/**
	 * Clones the specified item using this factory.
	 * Clone is a recursive operation
	virtual IBaseItem *clone(IBaseItem *item) = 0;
	 */
};

}



#endif /* INCLUDE_API_IITEMFACTORY_H_ */
