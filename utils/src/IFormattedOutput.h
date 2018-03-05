/*
 * IFormattedOutput.h
 *
 *  Created on: Aug 26, 2016
 *      Author: ballance
 */

#ifndef SRC_IFORMATTEDOUTPUT_H_
#define SRC_IFORMATTEDOUTPUT_H_
#include <string>

namespace qpssc {

class IFormattedOutput {
public:

	virtual ~IFormattedOutput() { }

	virtual void println(const std::string &l) = 0;

	virtual void println(const char *fmt, ...) = 0;

	virtual void print(const std::string &l) = 0;

	virtual void print(const char *fmt, ...) = 0;

	virtual void flush() = 0;

	virtual const std::string &indent() const = 0;

	virtual void inc_indent() = 0;

	virtual void dec_indent() = 0;

};
}



#endif /* SRC_IFORMATTEDOUTPUT_H_ */
