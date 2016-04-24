/*
 * HarmonicGenerator.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: bog
 */

#include "HarmonicGenerator.h"
#include <cassert>
#include <cmath>

void HarmonicGenerator::configureHarmonics(std::vector<float> && amplitudes) {
	harmonics_ = std::move(amplitudes);
}

void HarmonicGenerator::skipInterval(unsigned long length_us) {
	crtTime_ += length_us;
}

void HarmonicGenerator::generate16bitSamples(unsigned long length_us, unsigned sampleRate, int16_t out[]) {
	unsigned long samplesRequired = (unsigned long)(length_us * sampleRate) * 1.e-6;
	unsigned long sampleInterval = 1e+6 / sampleRate;	// in microseconds
	for (unsigned i=0; i<samplesRequired; i++) {
		double value = 0;
		float baseFreq = harmonics_[0];
		for (unsigned k=1; k < harmonics_.size(); k++) {
			if (harmonics_[k] == 0)
				continue;
			double omega = baseFreq * k;
			double t = (crtTime_ + i*sampleInterval) * 1.e-6;	// time in seconds
			double phase = omega * t;
			double amplitude = harmonics_[k];

			value += sin(phase) * amplitude * volume_;
		}
		out[i] = 0x7FFFF * std::min(1.0, std::max(-1.0, value));
	}
	crtTime_ += length_us;
}

void HarmonicGenerator::setVolume(float volume) {
	assert(volume >= 0 && volume <= 1);
	volume_ = volume;
}
