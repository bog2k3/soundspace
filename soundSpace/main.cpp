/*
 * main.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: bog
 */

#include "../wavegen/WavegenClient.h"
#include <iostream>

int main() {
	WavegenClient wc("test");
	wc.connectAsync("localhost", [](OperationResult res) {
		switch (res) {
		case OperationResult::InProgress:
			std::cout << "Connection to PulseAudio server in progress...\n";
			break;
		case OperationResult::Failed:
			std::cout << "Connection to PulseAudio server FAILED!!!\n";
			break;
		case OperationResult::Success:
			std::cout << "Connection to PulseAudio server SUCCESS!\n";
			break;
		default:
			std::cout << "Connection to PulseAudio server UNKNOWN state: " << (int)res << "\n";
		}
	});
	return 0;
}

