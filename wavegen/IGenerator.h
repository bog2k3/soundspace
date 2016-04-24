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
	IGenerator() = default;
	virtual ~IGenerator() = default;

	// skip an interval of time (fast-forward)
	virtual void skipInterval(unsigned long length_us) = 0;

	// generate samples for the next length_us interval of time (in microseconds)
	// sampleRate is given in samples per second
	virtual void generate16bitSamples(unsigned long length_us, unsigned sampleRate, int16_t out[]) = 0; // find a better buffer solution, maybe std::array
private:
};

} // namespace wavegen

#endif /* GENERATOR_H_ */
