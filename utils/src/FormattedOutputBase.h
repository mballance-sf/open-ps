/*
 * FormattedOutputBase.h
 *
 *  Created on: Aug 26, 2016
 *      Author: ballance
 */

#ifndef SRC_FORMATTEDOUTPUTBASE_H_
#define SRC_FORMATTEDOUTPUTBASE_H_
#include <stdint.h>
#include "IFormattedOutput.h"

namespace qpssc {

class FormattedOutputBase : public virtual IFormattedOutput {
public:
	FormattedOutputBase();

	virtual ~FormattedOutputBase();

	virtual const std::string &indent() const;

	virtual void inc_indent();

	virtual void dec_indent();

private:
	uint32_t			m_indent_inc;
	std::string			m_ind;

};

} /* namespace qpssc */

#endif /* SRC_FORMATTEDOUTPUTBASE_H_ */
