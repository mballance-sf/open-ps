/*
 * Rules2AutProcessor.h
 *
 *  Created on: Aug 25, 2016
 *      Author: ballance
 */

#ifndef SRC_RULES2AUTPROCESSOR_H_
#define SRC_RULES2AUTPROCESSOR_H_

#include <string>
#include "IModelProcessor.h"

namespace qpssc {

class Rules2AutProcessor: public IModelProcessor {
public:
	Rules2AutProcessor(const std::string &outdir);

	virtual ~Rules2AutProcessor();

	virtual bool process(IModel *model);

private:
	std::string					m_outdir;
};

} /* namespace qpssc */

#endif /* SRC_RULES2AUTPROCESSOR_H_ */
