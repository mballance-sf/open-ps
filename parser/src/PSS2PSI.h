/*
 * PSS2PSI.h
 *
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#ifndef PSS2PSI_SRC_PSS2PSI_H_
#define PSS2PSI_SRC_PSS2PSI_H_
#include "IModel.h"
#include <string>

using namespace psi_api;

class PSS2PSI {
public:
	PSS2PSI();

	virtual ~PSS2PSI();

	bool process(IModel *model, const std::string &path);
};

#endif /* PSS2PSI_SRC_PSS2PSI_H_ */

