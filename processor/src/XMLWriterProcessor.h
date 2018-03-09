/*
 * XMLWriterProcessor.h
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#ifndef SRC_XMLWRITERPROCESSOR_H_
#define SRC_XMLWRITERPROCESSOR_H_

#include "IModelProcessor.h"
#include <string>

namespace qpssc {

class XMLWriterProcessor: public IModelProcessor {
public:
	XMLWriterProcessor(const std::string &filename);

	virtual ~XMLWriterProcessor();

	virtual bool process(IModel *model);

private:
	std::string				m_filename;
};

} /* namespace qpssc */

#endif /* SRC_XMLWRITERPROCESSOR_H_ */
