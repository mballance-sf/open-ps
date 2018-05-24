/*
 * ModelNameProvider.h
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#pragma once
#include <string>
#include <vector>
#include "IField.h"
#include "IAction.h"


class ModelNameProvider {
public:
	ModelNameProvider();

	virtual ~ModelNameProvider();

	void enter(const std::string &name);
	void leave(const std::string &name);

	void enter(IField *field);
	void leave(IField *field);

	std::string name();

private:
	std::vector<std::string>			m_names;

};

