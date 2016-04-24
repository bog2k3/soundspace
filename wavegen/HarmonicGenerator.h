/*
 * HarmonicGenerator.h
 *
 *  Created on: Apr 24, 2016
 *      Author: bog
 */

#ifndef HARMONICGENERATOR_H_
#define HARMONICGENERATOR_H_

#include "IGenerator.h"

#include <vector>

class HarmonicGenerator: public wavegen::IGenerator {
public:
	HarmonicGenerator() = default;
	virtual ~HarmonicGenerator() = default;

	// pass a vector containing the base Frequency at [0]
	// 		and i-th harmonic amplitude at [i] where i=1..n
	// 		1st harmonic amplitude is actually the amplitude of the base frequency
	// amplitude values must be between [0.0 ~ 1.0]
	void configureHarmonics(std::vector<float> &&amplitudes);

	// sets the overal volume; value must be between [0.0 ~ 1.0]
	// this value is multiplied by each harmonic's amplitude
	void setVolume(float volume);

	void skipInterval(unsigned long length_us) override;
	void generate16bitSamples(unsigned long length_us, unsigned sampleRate, int16_t out[]) override;

private:
	unsigned long crtTime_ = 0;	// in microseconds
	float volume_ = 1.f;
	std::vector<float> harmonics_;
};

#endif /* HARMONICGENERATOR_H_ */
