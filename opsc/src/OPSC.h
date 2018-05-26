/*
 * OPSC.h
 *
 *  Created on: May 26, 2018
 *      Author: ballance
 */

#pragma once
#include "IModel.h"
#include <istream>
#include <ostream>

class OPSC {
public:
	OPSC();

	virtual ~OPSC();

	/**
	 * Parse a PSS file
	 */
	bool parse(
			std::istream 		&in,
			const std::string	&path);

	/**
	 * Load a .psi (incremental) file
	 */
	bool load(
			std::istream 		&in,
			const std::string	&path);

	bool link();

	bool elab(
			const std::string	&comp,
			const std::string	&action);

	bool write(std::ostream &out);

	const IModelPtr &model() const { return m_model; }

	IComponent *root_comp() const { return m_root_comp; }

	IAction *root_action() const { return m_root_action; }

private:
	IModelPtr			m_model;
	IComponent			*m_root_comp;
	IAction				*m_root_action;

};

