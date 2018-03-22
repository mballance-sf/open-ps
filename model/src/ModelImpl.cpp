/*
 * ModelImpl.cpp
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
 *
 *  Created on: Apr 26, 2016
 *      Author: ballance
 */

#include "ModelImpl.h"

#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <string>
#include <vector>
#include <assert.h>

#include "ActionImpl.h"
#include "BinaryExprImpl.h"
#include "BindImpl.h"
#include "BindPathImpl.h"
#include "ComponentImpl.h"
#include "ConstraintBlockImpl.h"
#include "ConstraintExprImpl.h"
#include "ConstraintIfImpl.h"
#include "ConstraintImpliesImpl.h"
#include "ExecImpl.h"
#include "ExecExprStmtImpl.h"
#include "ExtendImpl.h"
#include "FieldImpl.h"
#include "FieldRefImpl.h"
#include "ActivityBlockStmtImpl.h"
#include "ActivityRepeatStmtImpl.h"
#include "ActivityTraverseStmtImpl.h"
#include "ImportFuncImpl.h"
#include "LiteralImpl.h"
#include "ScalarTypeImpl.h"
#include "StructImpl.h"
#include "IBaseItem.h"
#include "MethodCallExprImpl.h"
#if !defined(_WIN32) && !defined(__CYGWIN__)
#include <execinfo.h>
#endif

namespace psi {

ModelImpl::ModelImpl() :
		BaseItemImpl(IBaseItem::TypeModel),
		m_global_pkg(""), m_callback_ctxt(0) {
}

ModelImpl::~ModelImpl() {
	// TODO Auto-generated destructor stub
}

//const std::vector<IPackage*> &ModelImpl::getPackages() {
//	return m_packages;
//}

const std::vector<IBaseItem *> &ModelImpl::getItems() const {
	return m_children;
}

void ModelImpl::remove(IBaseItem *it) {
	for (std::vector<IBaseItem *>::iterator i=m_children.begin();
			i!=m_children.end(); i++) {
		if ((*i) == it) {
			m_children.erase(i);
			break;
		}
	}
}


void ModelImpl::add(IBaseItem *it) {
	if (it) {
		it->setParent(this);
		m_children.push_back(it);
	} else {
		fprintf(stdout, "Error: Attempting to add null item\n");
#if !defined(_WIN32) && !defined(__CYGWIN__)
		{
			int bt_sz;
			void *bt[256];
			bt_sz = backtrace(bt, sizeof(bt)/sizeof(void *));

			char **bt_s = backtrace_symbols(bt, bt_sz);
			fprintf(stdout, "Backtrace:\n");
			for (int i=0; i<bt_sz; i++) {
				fprintf(stdout, "  %s\n", bt_s[i]);
			}
			free(bt_s);
		}
#endif
	}
}

IPackage *ModelImpl::findPackage(const std::string &name, bool create) {
	std::vector<IBaseItem *>::iterator it;

	for (it=m_children.begin(); it!=m_children.end(); it++) {
		if ((*it)->getType() == IBaseItem::TypePackage) {
			IBaseItem *bi = *it;
			IPackage *pkg = dynamic_cast<IPackage *>(bi);
			if (pkg->getName() == name) {
				return pkg;
			}
		}
	}

	if (create) {
		IPackage *pkg = new PackageImpl(name);
		m_children.push_back(pkg);
		return pkg;
	} else {
		return 0;
	}
}

/**
 * Data pss_types
 */



ICallbackContext *ModelImpl::getCallbackContext() {
	if (!m_callback_ctxt) {
		fprintf(stdout, "Error: getCallbackContext returning null handle\n");
		fflush(stdout);
	}
	return m_callback_ctxt;
}

void ModelImpl::setCallbackContext(ICallbackContext *ctxt) {
	m_callback_ctxt = ctxt;
}

IModel *ModelImpl::clone() const {
	ModelImpl *ret = new ModelImpl();

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it != getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	return ret;
}

IBaseItem *ModelImpl::clone(IBaseItem *item) {
	return 0;
}

const std::string &ModelImpl::getAttribute(const std::string &key) {
	return m_null_attr;
}

void ModelImpl::getAttributes(std::vector<std::string> &keys) {
	keys.clear();
}

bool ModelImpl::hasAttribute(const std::string &key) {
	return false;
}

void ModelImpl::setAttribute(const std::string &key, const std::string &val) {

}

void ModelImpl::clearAttribute(const std::string &key) {

}


} /* namespace psi */
