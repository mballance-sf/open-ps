/*
 * LFSR.h
 *
 *  Created on: Mar 27, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_LFSR_H_
#define PROCESSOR_SRC_LFSR_H_
#include <stdint.h>

class LFSR {
public:
	LFSR();

	virtual ~LFSR();

	uint64_t value() const { return m_value; }

	uint64_t next();

private:
	uint64_t				m_value;

	static uint64_t			m_byte_feedback[256];

};

#endif /* PROCESSOR_SRC_LFSR_H_ */
