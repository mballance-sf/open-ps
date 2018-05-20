/*
 * OPS.h
 *
 *  Created on: Mar 5, 2018
 *      Author: ballance
 */

#ifndef OPS_SRC_OPS_H_
#define OPS_SRC_OPS_H_
#include <vector>
#include <string>
#include "IModel.h"

class OPS {
public:
	OPS();

	virtual ~OPS();

	void add_source_file(const std::string &file);

	bool parse();

private:
	std::vector<std::string>		m_source_files;
	IModel					*m_model;
};

#endif /* OPS_SRC_OPS_H_ */
