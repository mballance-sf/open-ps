/*
 * IExecListener.h
 *
 *  Created on: Apr 29, 2018
 *      Author: ballance
 */
#pragma once
#include "IExec.h"



class IExecListener {
public:
	virtual ~IExecListener() { }

	virtual void fork_parallel() = 0;

	virtual void start_thread() = 0;

	virtual void end_thread() = 0;

	virtual void join_parallel() = 0;

	virtual void exec(
			const std::string	&context,
			IExec 				*e) = 0;

};


