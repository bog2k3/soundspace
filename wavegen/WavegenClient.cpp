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

#include <cassert>
#include <condition_variable>
#include <mutex>

namespace wavegen {

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
	pa_context_disconnect(paContext_);
	pa_context_unref(paContext_);
	pa_threaded_mainloop_free(paMainLoop_);
}

bool WavegenClient::connect(std::string const& serverAddr) {
	std::condition_variable condConnectionSettled;
	std::mutex mtx;
	std::unique_lock<std::mutex> lk(mtx);
	volatile bool connectionSettled = false;

	connectAsync(serverAddr, [&connectionSettled, &condConnectionSettled, &mtx](OperationResult res) {
		if (res == OperationResult::Failed ||
			res == OperationResult::Success) {
			connectionSettled = true;
			condConnectionSettled.notify_one();
		}
	});

	// wait for the operation to signal completion:
	condConnectionSettled.wait(lk, [&connectionSettled] {return connectionSettled; });
}

void WavegenClient::connectAsync(std::string const& serverAddr, AsyncOperationCb &&cb) {
	{
		std::lock_guard<std::mutex> lock(mapOperationCallbacksMutex_);
		if (cb)
			mapOperationCallbacks_[OperationType::Connect].emplace_back(std::move(cb));
	}

	if (pa_context_connect(paContext_, serverAddr.c_str(), PA_CONTEXT_NOFLAGS, nullptr) < 0) {
		// connection failed
		notifyResult(OperationType::Connect, OperationResult::Failed, true);
	}
}

void WavegenClient::notifyResult(OperationType type, OperationResult result, bool clearCallbacks) {
	std::vector<AsyncOperationCb> vCallbacks;
	{
		std::lock_guard<std::mutex> lock(mapOperationCallbacksMutex_);
		vCallbacks.swap(mapOperationCallbacks_[type]);
	}

	for (auto& cb : vCallbacks)
		cb(result);

	if (clearCallbacks)
		vCallbacks.clear();

	{
		std::lock_guard<std::mutex> lock(mapOperationCallbacksMutex_);
		std::move(vCallbacks.begin(), vCallbacks.end(), std::back_inserter(mapOperationCallbacks_[type]));
	}
}

void WavegenClient::onContextStateChanged() {
	switch (pa_context_get_state(paContext_)) {
	case PA_CONTEXT_FAILED:
	case PA_CONTEXT_TERMINATED:
		notifyResult(OperationType::Connect, OperationResult::Failed, true);
		break;
	case PA_CONTEXT_READY:
		// connection OK
		notifyResult(OperationType::Connect, OperationResult::Success, true);
		break;
	default:
		// connecting in progress...
		notifyResult(OperationType::Connect, OperationResult::InProgress, false);
		break;
	}
}

void WavegenClient::contextStateCallback(pa_context*, void* userdata) {
	assert(userdata);
	WavegenClient* wg = (WavegenClient*)userdata;
	wg->onContextStateChanged();
}

} // namespace wavegen