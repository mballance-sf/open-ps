/*
 * IItemFactory.h
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
 *
 * Licensed under the Apache License, Version 2.0 (the
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
#include <vector>
#include <string>
#include <stdint.h>
#include "IAction.h"
#include "IBinaryExpr.h"
#include "IInExpr.h"
#include "IBind.h"
#include "IBindPath.h"
#include "IComponent.h"
#include "IConstraintBlock.h"
#include "IConstraintExpr.h"
#include "IConstraintForeach.h"
#include "IConstraintIf.h"
#include "IConstraintUnique.h"
#include "ICoverspec.h"
#include "IEnumerator.h"
#include "IEnumType.h"
#include "IExec.h"
#include "IExecReplacementExpr.h"
#include "IExecStmt.h"
#include "IExecExprStmt.h"
#include "IExtend.h"
#include "IExtendComposite.h"
#include "IExtendEnum.h"
#include "IActivityBlockStmt.h"
#include "IActivityDoActionStmt.h"
#include "IActivityIfElseStmt.h"
#include "IActivityTraverseStmt.h"
#include "IActivityRepeatStmt.h"
#include "IActivitySelectStmt.h"
#include "IActivitySelectBranchStmt.h"
#include "ILiteral.h"
#include "IArrayType.h"
#include "IScalarType.h"
#include "IStruct.h"
#include "IConstraintImplies.h"
#include "IImport.h"
#include "IImportFunc.h"
#include "IMethodCallExpr.h"
#include "IOpenRangeValue.h"
#include "IOpenRangeList.h"
#include "IPool.h"
#include "IRefType.h"
#include "IVariableRef.h"
#include "ISymbol.h"

class IItemFactory {
public:

	virtual ~IItemFactory() { }

	/**
	 * Data pss_types
	 */

	virtual IModel *mkModel() = 0;

	virtual IArrayType *mkArrayType(
			IBaseItem			*target,
			bool				has_sum,
			IExpr				*lhs,
			IExpr				*rhs
			) = 0;
	/**
	 * Creates a scalar type. The msb and lsb parameters are ignored for types
	 * other than pss_int and pss_bit
	 */
	virtual IScalarType *mkScalarType(
			IScalarType::ScalarType t,
			IExpr					*msb,
			IExpr					*lsb,
			IOpenRangeList			*domain=0) = 0;

	/**
	 * action
	 */
	virtual IAction *mkAction(const std::string &name, IBaseItem *super_type) = 0;

	/**
	 * Creates a new bind statement
	 */
	virtual IBind *mkBind(const std::vector<IBindPath *> &targets) = 0;

	virtual IBindPath *mkBindPath(const std::vector<IBaseItem *> &elems) = 0;


	/**
	 * Create a new component type
	 */
	virtual IComponent *mkComponent(const std::string &name, IBaseItem	*super_type) = 0;

	virtual ICoverspec *mkCoverspec(const std::string &name) = 0;

	virtual IExec *mkTargetTemplateExec(
			IExec::ExecKind 		kind,
			const std::string		&language,
			const std::string 		&text,
			const std::vector<IExecReplacementExpr *> &replacements) = 0;

	virtual IExecReplacementExpr *mkExecReplacementExpr(
			IExpr					*expr,
			uint32_t				offset,
			uint32_t				length) = 0;

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

	virtual IEnumerator *mkEnumerator(
			const std::string		&name,
			IExpr					*value) = 0;

	virtual IEnumType *mkEnumType(
			const std::string					&name,
			const std::vector<IEnumerator *>	&enumerators) = 0;

	virtual IMethodCallExpr *mkMethodCallExpr(
			IRefType					*func,
			const std::vector<IExpr *>	&parameters) = 0;

	/**
	 * Create a new struct type
	 */
	virtual IStruct *mkStruct(
			const std::string 		&name,
			IStruct::StructType		t,
			IBaseItem 				*super_type) = 0;

	virtual ISymbol *mkSymbol(
			const std::string			&name,
			const std::vector<IField *>	&params,
			IActivityBlockStmt				*body) = 0;

	virtual IExtendComposite *mkExtendComposite(
			IExtend::ExtendType		type,
			IBaseItem				*target) = 0;

	virtual IExtendEnum *mkExtendEnum(
			IBaseItem							*target,
			const std::vector<IEnumerator *>	&enumerators) = 0;

	/**
	 * Create a field for use in declaring the contents of an
	 * action or struct data type
	 */
	virtual IField *mkField(
			const std::string		&name,
			IBaseItem				*field_type,
			IField::FieldAttr		attr,
			IExpr					*array_dim) = 0;

	virtual IPool *mkPool(
			const std::string		&name,
			IBaseItem				*pool_type,
			IExpr					*pool_size) = 0;

	virtual IBinaryExpr *mkBinExpr(
			IExpr 					*lhs,
			IBinaryExpr::BinOpType	op,
			IExpr 					*rhs) = 0;

	virtual IInExpr *mkInExpr(
			IExpr					*lhs,
			IOpenRangeList			*rhs) = 0;

	virtual IOpenRangeValue *mkOpenRangeValue(
			IExpr					*lhs,
			IExpr					*rhs,
			bool					domain_bound=false) = 0;

	virtual IOpenRangeList *mkOpenRangeList(
			const std::vector<IOpenRangeValue *>	&ranges) = 0;

	virtual IVariableRef *mkVariableRef(
			IBaseItem				*scope,
			const std::string		&id,
			IExpr					*index_lhs,
			IExpr					*index_rhs) = 0;

	/**
	 * Creates a new block statement. parallel, schedule, pss_select, and
	 * Block statements are all, fundamentally, block statements.
	 * The value of the 'type' parameter can be
	 * ActivityStmt_Block, ActivityStmt_Parallel, ActivityStmt_Schedule, or ActivityStmt_Select
	 */
	virtual IActivityBlockStmt *mkActivityBlockStmt(
			IActivityStmt::ActivityStmtType type=IActivityStmt::ActivityStmt_Block) = 0;

	virtual IActivityTraverseStmt *mkActivityTraverseStmt(
			IVariableRef 	*action,
			IConstraint 	*with_c=0) = 0;

	virtual IActivityDoActionStmt *mkActivityDoActionStmt(
			IBaseItem *type, IConstraintBlock *with_c) = 0;

	virtual IActivityIfElseStmt *mkActivityIfElseStmt(
			IExpr			*cond,
			IActivityStmt	*true_stmt,
			IActivityStmt	*false_stmt) = 0;

	virtual IActivityRepeatStmt *mkActivityRepeatStmt(
			IActivityRepeatStmt::RepeatType type,
			IExpr *expr, IActivityStmt *body) = 0;

	virtual IActivitySelectStmt *mkActivitySelectStmt(
			const std::vector<IActivitySelectBranchStmt *> &branches) = 0;

	virtual IActivitySelectBranchStmt *mkActivitySelectBranchStmt(
			IActivityStmt			*stmt,
			IExpr					*guard,
			IExpr					*weight) = 0;

	virtual ILiteral *mkIntLiteral(int64_t v) = 0;

	virtual ILiteral *mkBitLiteral(uint64_t v) = 0;

	virtual ILiteral *mkBoolLiteral(bool v) = 0;

	virtual ILiteral *mkStringLiteral(const std::string &v) = 0;

	virtual IRefType *mkRefType(
			IScopeItem 						*scope,
			const std::vector<std::string>	&type,
			bool							fully_qualified) = 0;

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

	virtual IConstraintForeach *mkConstraintForeach(
			IVariableRef			*target,
			const std::string		&iterator,
			IConstraintBlock		*body) = 0;

	virtual IConstraintIf *mkConstraintIf(
			IExpr 			*cond,
			IConstraint 	*true_c,
			IConstraint 	*false_c) = 0;

	virtual IConstraintImplies *mkConstraintImplies(
			IExpr			*cond,
			IConstraint		*imp) = 0;

	virtual IConstraintUnique *mkConstraintUnique(
			IOpenRangeList			*target) = 0;

	virtual IImport *mkImport(
			IBaseItem						*target,
			bool							is_wildcard) = 0;

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

