/*
 * LFSR.h
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
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

	void seed(uint64_t seed) { m_value = seed; }

private:
	uint64_t				m_value;

	static uint64_t			m_byte_feedback[256];

};

#endif /* PROCESSOR_SRC_LFSR_H_ */
