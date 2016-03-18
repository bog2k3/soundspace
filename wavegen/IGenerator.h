/*
* Generator.h
*
*  Created on: March 18th, 2016
*      Author: bog
*/

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <array>
#include <cstdint>

namespace wavegen {

class IGenerator {
public:
	IGenerator(unsigned sampleFreq, unsigned sampleBitCount); // TODO change from unsigned to enum
	virtual ~IGenerator() {
	}

	virtual void generate16bitSamples(unsigned long startTime_us, unsigned long length_us, int16_t out[]); // find a better buffer solution, maybe std::array
private:
};

} // namespace wavegen

#endif /* GENERATOR_H_ */