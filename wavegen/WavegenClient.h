/*
 * WavegenClient.h
 *
 *  Created on: Feb 4, 2016
 *      Author: bog
 */

#ifndef WAVEGENCLIENT_H_
#define WAVEGENCLIENT_H_

#include "AsyncOps.h"

#include <string>
#include <map>
#include <vector>
#include <mutex>

struct pa_context;
struct pa_threaded_mainloop;
struct pa_mainloop_api;

namespace wavegen {

class WavegenClient {
public:

	WavegenClient(std::string const& name);
	virtual ~WavegenClient();

	bool connect(std::string const& serverAddr);

	void connectAsync(std::string const& serverAddr, AsyncOperationCb &&cb);

private:
	std::string name_;
	pa_threaded_mainloop *paMainLoop_ = nullptr;
	pa_mainloop_api *paMainApi_ = nullptr;
	pa_context *paContext_ = nullptr;

	using MapOperationCallbacks = std::map<OperationType, std::vector<AsyncOperationCb>>;
	MapOperationCallbacks mapOperationCallbacks_;
	std::mutex mapOperationCallbacksMutex_;

	void notifyResult(OperationType type, OperationResult result, bool clearCallbacks);

	void onContextStateChanged();

	static void contextStateCallback(pa_context* ctx, void* userdata);
};

} // namespace wavegen

#endif /* WAVEGENCLIENT_H_ */
