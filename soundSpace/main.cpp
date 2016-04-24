/*
 * main.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: bog
 */

#include "../wavegen/WavegenClient.h"
#include "../wavegen/HarmonicGenerator.h"

#include <unistd.h>

#include <iostream>

using namespace wavegen;

int main() {
	WavegenClient wc("test");
	wc.connectAsync("", [](OperationResult res) {
		switch (res) {
		case OperationResult::InProgress:
			std::cout << "Connection to PulseAudio server in progress...\n";
			break;
		case OperationResult::Failed:
			std::cout << "Connection to PulseAudio server FAILED!!!\n";
			return -1;
			break;
		case OperationResult::Success:
			std::cout << "Connection to PulseAudio server SUCCESS!\n";
			break;
		default:
			std::cout << "Connection to PulseAudio server UNKNOWN state: " << (int)res << "\n";
		}
	});

	HarmonicGenerator gen;
	gen.configureHarmonics({800, 1.f, 0.3f, 0.6f, 0.1f, 0.4f});
	gen.setVolume(0.3f);

	wc.addGenerator(gen);

	sleep(5);
	return 0;
}

