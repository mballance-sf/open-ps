/*
 * SolverErrorException.h
 *
 *  Created on: Apr 9, 2018
 *      Author: ballance
 */

#pragma once
#include <string>
#include <exception>

class SolverErrorException: public std::exception {
public:
	SolverErrorException(const std::string &msg);

	virtual ~SolverErrorException();

	const std::string &msg() const { return m_msg; }

private:
	std::string							m_msg;
};

