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
#include "ExecExprStmtImpl.h"
#include "MethodCallExprImpl.h"
#include "StructImpl.h"
#include "ExtendImpl.h"
#include "ExtendCompositeImpl.h"
#include "ExtendEnumImpl.h"
#include "FieldImpl.h"
#include "ActivityBlockStmtImpl.h"
#include "ActivityRepeatStmtImpl.h"
#include "ActivityDoActionStmtImpl.h"
#include "ActivityTraverseStmtImpl.h"
#include "BinaryExprImpl.h"
#include "FieldRefImpl.h"
#include "LiteralImpl.h"
#include "ConstraintBlockImpl.h"
#include "ConstraintExprImpl.h"
#include "ConstraintIfImpl.h"
#include "ConstraintImpliesImpl.h"
#include "ImportFuncImpl.h"
#include "ImportImpl.h"
#include "RefTypeImpl.h"
#include "VariableRefImpl.h"
#include "SymbolImpl.h"


using namespace psi;

ItemFactoryImpl::ItemFactoryImpl() {
	// TODO Auto-generated constructor stub

}

ItemFactoryImpl::~ItemFactoryImpl() {
	// TODO Auto-generated destructor stub
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
		IExpr					*lsb) {
	if (t != IScalarType::ScalarType_Bit && t != IScalarType::ScalarType_Int) {
		msb = 0;
		lsb = 0;
	}
	return new ScalarTypeImpl(t, msb, lsb);
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
		const std::string		&text) {
	return new ExecImpl(kind, language, text);
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
		IImportFunc					*func,
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
		IGraphBlockStmt					*body) {
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

IGraphBlockStmt *ItemFactoryImpl::mkGraphBlockStmt(IGraphStmt::GraphStmtType type) {
	return new ActivityBlockStmtImpl(type);
}

IGraphRepeatStmt *ItemFactoryImpl::mkGraphRepeatStmt(
		IGraphRepeatStmt::RepeatType type,
		IExpr *expr, IGraphStmt *body) {
	return new ActivityRepeatStmtImpl(type, expr, body);
}

IActivityDoActionStmt *ItemFactoryImpl::mkActivityDoActionStmt(
		IBaseItem *target,
		IConstraintBlock *with_c) {
	return new ActivityDoActionStmtImpl(target, with_c);
}

IActivityTraverseStmt *ItemFactoryImpl::mkActivityTraverseStmt(
		IVariableRef *action, IConstraint *with_c) {
	return new ActivityTraverseStmtImpl(action, with_c);
}

IBinaryExpr *ItemFactoryImpl::mkBinExpr(
		IExpr 					*lhs,
		IBinaryExpr::BinOpType	op,
		IExpr 					*rhs) {
	return new BinaryExprImpl(lhs, op, rhs);
}

IFieldRef *ItemFactoryImpl::mkFieldRef(
		const std::vector<IField *>		&field_path) {
	return new FieldRefImpl(field_path);
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
		const std::vector<std::string>		&type) {
	return new RefTypeImpl(scope, type);
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
