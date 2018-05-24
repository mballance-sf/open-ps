/*
 * ModelNameProvider.cpp
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#include "ModelNameProvider.h"

ModelNameProvider::ModelNameProvider() {
	// TODO Auto-generated constructor stub

}

ModelNameProvider::~ModelNameProvider() {
	// TODO Auto-generated destructor stub
}

void ModelNameProvider::enter(const std::string &name) {
	m_names.push_back(name);
}

void ModelNameProvider::leave(const std::string &name) {
	if (m_names.size() > 0) {
		m_names.pop_back();
	} else {
		fprintf(stdout, "Error: leave(%s) with 0-size stack\n",
				name.c_str());
	}
}

void ModelNameProvider::enter(IField *field) {
	m_names.push_back(field->getName());
}

void ModelNameProvider::leave(IField *field) {
	if (m_names.size() > 0) {
		m_names.pop_back();
	} else {
		fprintf(stdout, "Error: leave(field=%s) with 0-size stack\n",
				field->getName().c_str());
	}
}

std::string ModelNameProvider::name() {
	std::string name;

	for (uint32_t i=0; i<m_names.size(); i++) {
		name.append(m_names.at(i));
		if (i+1 < m_names.size()) {
			name.append(".");
		}
	}

	return name;
}
