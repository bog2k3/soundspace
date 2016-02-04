/*
 * WavegenClient.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: bog
 */


#include "WavegenClient.h"
#include <pulse/mainloop-api.h>
#include <pulse/thread-mainloop.h>
#include <pulse/context.h>
#include <pulse/def.h>

WavegenClient::WavegenClient(std::string const& name)
	: name_(name)
{
	paMainLoop_ = pa_threaded_mainloop_new();
	pa_threaded_mainloop_set_name(paMainLoop_, name_.c_str());
	pa_threaded_mainloop_start(paMainLoop_);

	paMainApi_ = pa_threaded_mainloop_get_api(paMainLoop_);
	paContext_ = pa_context_new(paMainApi_, name_.c_str());
	pa_context_set_state_callback(paContext_, &WavegenClient::contextStateCallback, this);
}

WavegenClient::~WavegenClient() {
	pa_context_unref(paContext_);
	pa_threaded_mainloop_free(paMainLoop_);
}

bool WavegenClient::connect(std::string const& serverAddr) {
	if (pa_context_connect(paContext_, serverAddr.c_str(), PA_CONTEXT_NOFLAGS, nullptr) < 0)
		// connection failed
		return false;
	// wait for connection to finish establishing...
	// TODO

}

void WavegenClient::connectAsync(std::string const& serverAddr, AsyncOperationCb cb) {

}

void WavegenClient::onContextStateChanged() {
	switch (pa_context_get_state(paContext_)) {
	case PA_CONTEXT_FAILED:
	case PA_CONTEXT_TERMINATED:
		// connection failed/closed
		break;
	case PA_CONTEXT_READY:
		// connection OK
		break;
	default:
		// connecting in progress...
		break;
	}
}

void WavegenClient::contextStateCallback(pa_context*, void* userdata) {
	WavegenClient* wg = (WavegenClient*)userdata;
	wg->onContextStateChanged();
}
