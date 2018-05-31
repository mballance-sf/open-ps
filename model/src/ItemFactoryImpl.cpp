/*
 * ItemFactoryImpl.cpp
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

#include "ItemFactoryImpl.h"
#include "ArrayTypeImpl.h"
#include "ScalarTypeImpl.h"
#include "BindImpl.h"
#include "BindPathImpl.h"
#include "ActionImpl.h"
#include "ComponentImpl.h"
#include "CoverspecImpl.h"
#include "EnumeratorImpl.h"
#include "EnumTypeImpl.h"
#include "ExecImpl.h"
#include "ExecReplacementExprImpl.h"
#include "ExecExprStmtImpl.h"
#include "MethodCallExprImpl.h"
#include "StructImpl.h"
#include "ExtendImpl.h"
#include "ExtendCompositeImpl.h"
#include "ExtendEnumImpl.h"
#include "FieldImpl.h"
#include "PoolImpl.h"
#include "ActivityBlockStmtImpl.h"
#include "ActivityRepeatStmtImpl.h"
#include "ActivityDoActionStmtImpl.h"
#include "ActivityIfElseStmtImpl.h"
#include "ActivitySelectStmtImpl.h"
#include "ActivitySelectBranchStmtImpl.h"
#include "ActivityTraverseStmtImpl.h"
#include "BinaryExprImpl.h"
#include "InExprImpl.h"
#include "LiteralImpl.h"
#include "ConstraintBlockImpl.h"
#include "ConstraintExprImpl.h"
#include "ConstraintForeachImpl.h"
#include "ConstraintIfImpl.h"
#include "ConstraintImpliesImpl.h"
#include "ConstraintUniqueImpl.h"
#include "ImportFuncImpl.h"
#include "ImportImpl.h"
#include "OpenRangeValueImpl.h"
#include "OpenRangeListImpl.h"
#include "RefTypeImpl.h"
#include "VariableRefImpl.h"
#include "SymbolImpl.h"
#include "ModelImpl.h"



ItemFactoryImpl::ItemFactoryImpl() {
	// TODO Auto-generated constructor stub

}

ItemFactoryImpl::~ItemFactoryImpl() {
	// TODO Auto-generated destructor stub
}


IModel *ItemFactoryImpl::mkModel() {
	return new ModelImpl();
}

IArrayType *ItemFactoryImpl::mkArrayType(
		IBaseItem				*target,
		bool					has_sum,
		IExpr					*lhs,
		IExpr					*rhs) {
	return new ArrayTypeImpl(
			target, has_sum, lhs, rhs);
}
/**
 * Creates a scalar type. The msb and lsb parameters are ignored for types
 * other than pss_int and pss_bit
 */
IScalarType *ItemFactoryImpl::mkScalarType(
		IScalarType::ScalarType t,
		IExpr					*msb,
		IExpr					*lsb,
		IOpenRangeList			*domain) {
	if (t != IScalarType::ScalarType_Bit && t != IScalarType::ScalarType_Int) {
		msb = 0;
		lsb = 0;
	}
	return new ScalarTypeImpl(t, msb, lsb, domain);
}

IBind *ItemFactoryImpl::mkBind(const std::vector<IBindPath *> &targets) {
	return new BindImpl(targets);
}

IBindPath *ItemFactoryImpl::mkBindPath(const std::vector<IBaseItem *> &path) {
	return new BindPathImpl(path);
}

/**
 * action
 */
IAction *ItemFactoryImpl::mkAction(const std::string &name, IBaseItem *super_type) {
	return new ActionImpl(name, super_type);
}

IComponent *ItemFactoryImpl::mkComponent(const std::string &name, IBaseItem *super_type) {
	return new ComponentImpl(name, super_type);
}

ICoverspec *ItemFactoryImpl::mkCoverspec(const std::string &name) {
	return new CoverspecImpl(name);
}

IExec *ItemFactoryImpl::mkTargetTemplateExec(
		IExec::ExecKind			kind,
		const std::string		&language,
		const std::string		&text,
		const std::vector<IExecReplacementExpr *> &replacements) {
	return new ExecImpl(kind, language, text, replacements);
}

IExecReplacementExpr *ItemFactoryImpl::mkExecReplacementExpr(
			IExpr					*expr,
			uint32_t				offset,
			uint32_t				length) {
	return new ExecReplacementExprImpl(expr, offset, length);
}

IExec *ItemFactoryImpl::mkInlineExec(
		IExec::ExecKind			kind,
		IExecCallback				*exec) {
	return new ExecImpl(kind, exec);
}

IExec *ItemFactoryImpl::mkNativeExec(
		IExec::ExecKind					kind,
		const std::vector<IExecStmt *>	&stmts) {
	return new ExecImpl(kind, stmts);
}

IExecExprStmt *ItemFactoryImpl::mkExecExprStmt(
		IExpr					*lhs,
		IExecExprStmt::AssignOp	op,
		IExpr					*rhs) {
	return new ExecExprStmtImpl(lhs, op, rhs);
}

IEnumerator *ItemFactoryImpl::mkEnumerator(
		const std::string		&name,
		IExpr					*value) {
	return new EnumeratorImpl(name, value);
}

IEnumType *ItemFactoryImpl::mkEnumType(
		const std::string					&name,
		const std::vector<IEnumerator *>	&enumerators) {
	return new EnumTypeImpl(name, enumerators);
}


IMethodCallExpr *ItemFactoryImpl::mkMethodCallExpr(
		IRefType					*func,
		const std::vector<IExpr *>	&parameters) {
	return new MethodCallExprImpl(func, parameters);
}

IVariableRef *ItemFactoryImpl::mkVariableRef(
		IBaseItem					*scope,
		const std::string			&id,
		IExpr						*index_lhs,
		IExpr						*index_rhs) {
	return new VariableRefImpl(scope, id, index_lhs, index_rhs);
}

IStruct *ItemFactoryImpl::mkStruct(
		const std::string 		&name,
		IStruct::StructType		t,
		IBaseItem 				*super_type) {
	return new StructImpl(name, t, super_type);
}

ISymbol *ItemFactoryImpl::mkSymbol(
		const std::string				&name,
		const std::vector<IField *>		&params,
		IActivityBlockStmt					*body) {
	return new SymbolImpl(name, params, body);
}

IExtendComposite *ItemFactoryImpl::mkExtendComposite(
		IExtend::ExtendType			type,
		IBaseItem 					*target) {
	return new ExtendCompositeImpl(type, target);
}

IExtendEnum *ItemFactoryImpl::mkExtendEnum(
		IBaseItem							*target,
		const std::vector<IEnumerator *>	&enumerators) {
	return new ExtendEnumImpl(target, enumerators);
}

/**
 * Create a field for use in declaring the contents of an
 * action or struct data type
 */
IField *ItemFactoryImpl::mkField(
		const std::string		&name,
		IBaseItem				*field_type,
		IField::FieldAttr		attr,
		IExpr					*array_dim) {
	return new FieldImpl(name, field_type, attr, array_dim);
}

IPool *ItemFactoryImpl::mkPool(
			const std::string		&name,
			IBaseItem				*pool_type,
			IExpr					*pool_size) {
	return new PoolImpl(name, pool_type, pool_size);
}

IActivityBlockStmt *ItemFactoryImpl::mkActivityBlockStmt(IActivityStmt::ActivityStmtType type) {
	return new ActivityBlockStmtImpl(type);
}

IActivityRepeatStmt *ItemFactoryImpl::mkActivityRepeatStmt(
		IActivityRepeatStmt::RepeatType type,
		IExpr *expr, IActivityStmt *body) {
	return new ActivityRepeatStmtImpl(type, expr, body);
}

IActivityDoActionStmt *ItemFactoryImpl::mkActivityDoActionStmt(
		IBaseItem *target,
		IConstraintBlock *with_c) {
	return new ActivityDoActionStmtImpl(target, with_c);
}

IActivityIfElseStmt *ItemFactoryImpl::mkActivityIfElseStmt(
			IExpr			*cond,
			IActivityStmt	*true_stmt,
			IActivityStmt	*false_stmt) {
	return new ActivityIfElseStmtImpl(cond, true_stmt, false_stmt);
}

IActivitySelectStmt *ItemFactoryImpl::mkActivitySelectStmt(
			const std::vector<IActivitySelectBranchStmt *> &branches) {
	return new ActivitySelectStmtImpl(branches);
}

IActivitySelectBranchStmt *ItemFactoryImpl::mkActivitySelectBranchStmt(
			IActivityStmt			*stmt,
			IExpr					*guard,
			IExpr					*weight) {
	return new ActivitySelectBranchStmtImpl(stmt, guard, weight);
}

IActivityTraverseStmt *ItemFactoryImpl::mkActivityTraverseStmt(
		IVariableRef *action, IConstraint *with_c) {
	return new ActivityTraverseStmtImpl(action, with_c);
}

IOpenRangeValue *ItemFactoryImpl::mkOpenRangeValue(
			IExpr 					*lhs,
			IExpr 					*rhs,
			bool					domain_bound) {
	return new OpenRangeValueImpl(lhs, rhs, domain_bound);
}

IOpenRangeList *ItemFactoryImpl::mkOpenRangeList(
			const std::vector<IOpenRangeValue *>	&ranges) {
	return new OpenRangeListImpl(ranges);
}

IBinaryExpr *ItemFactoryImpl::mkBinExpr(
		IExpr 					*lhs,
		IBinaryExpr::BinOpType	op,
		IExpr 					*rhs) {
	return new BinaryExprImpl(lhs, op, rhs);
}

IInExpr *ItemFactoryImpl::mkInExpr(
			IExpr					*lhs,
			IOpenRangeList			*rhs) {
	return new InExprImpl(lhs, rhs);
}

ILiteral *ItemFactoryImpl::mkIntLiteral(int64_t v) {
	return new LiteralImpl(v);
}

ILiteral *ItemFactoryImpl::mkBitLiteral(uint64_t v) {
	return new LiteralImpl(v);
}

ILiteral *ItemFactoryImpl::mkBoolLiteral(bool v) {
	return new LiteralImpl(v);
}

ILiteral *ItemFactoryImpl::mkStringLiteral(const std::string &v) {
	return new LiteralImpl(v);
}

IRefType *ItemFactoryImpl::mkRefType(
		IScopeItem							*scope,
		const std::vector<std::string>		&type,
		bool								fully_qualified) {
	return new RefTypeImpl(scope, type, fully_qualified);
}

IConstraintBlock *ItemFactoryImpl::mkConstraintBlock(
		const std::string 		&name,
		IConstraint				*c) {
	IConstraintBlock *ret = new ConstraintBlockImpl(name);

	if (c) {
		ret->add(c);
	}

	return ret;
}

IConstraintBlock *ItemFactoryImpl::mkConstraintBlock(
		const std::string 					&name,
		const std::vector<IConstraint *>	&cl) {
	IConstraintBlock *ret = new ConstraintBlockImpl(name);

	ret->add(cl);

	return ret;
}

IConstraintExpr *ItemFactoryImpl::mkConstraintExpr(IExpr *expr) {
	return new ConstraintExprImpl(expr);
}

IConstraintForeach *ItemFactoryImpl::mkConstraintForeach(
			IVariableRef			*target,
			const std::string		&iterator,
			IConstraintBlock		*body) {
	return new ConstraintForeachImpl(
			target,
			iterator,
			body);
}


IConstraintIf *ItemFactoryImpl::mkConstraintIf(
		IExpr 			*cond,
		IConstraint 	*true_c,
		IConstraint 	*false_c) {
	return new ConstraintIfImpl(cond, true_c, false_c);
}

IConstraintImplies *ItemFactoryImpl::mkConstraintImplies(
				IExpr			*cond,
				IConstraint		*imp) {
	return new ConstraintImpliesImpl(cond, imp);
}

IConstraintUnique *ItemFactoryImpl::mkConstraintUnique(
		IOpenRangeList				*target) {
	return new ConstraintUniqueImpl(target);
}

IImport *ItemFactoryImpl::mkImport(
		IBaseItem						*target,
		bool							is_wildcard) {
	return new ImportImpl(target, is_wildcard);
}

IImportFunc *ItemFactoryImpl::mkImportFunc(
		const std::string				&name,
		IBaseItem						*ret_type,
		const std::vector<IField *>		&parameters) {
	return new ImportFuncImpl(
			name,
			(ret_type)?(new FieldImpl("", ret_type, IField::FieldAttr_None, 0)):0,
					parameters);
}
