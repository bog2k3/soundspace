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
struct pa_stream;

namespace wavegen {

class IGenerator;

class WavegenClient {
public:

	static constexpr unsigned SAMPLE_RATE = 44100;

	WavegenClient(std::string const& name);
	virtual ~WavegenClient();

	bool connect(std::string const& serverAddr);

	void connectAsync(std::string const& serverAddr, AsyncOperationCb &&cb);

	void addGenerator(IGenerator &gen);

private:
	std::string name_;
	pa_threaded_mainloop *paMainLoop_ = nullptr;
	pa_mainloop_api *paMainApi_ = nullptr;
	pa_context *paContext_ = nullptr;
	pa_stream *paStream_ = nullptr;

	using MapOperationCallbacks = std::map<OperationType, std::vector<AsyncOperationCb>>;
	MapOperationCallbacks mapOperationCallbacks_;
	std::mutex mapOperationCallbacksMutex_;

	bool firstConnection_ = true;

	void notifyResult(OperationType type, OperationResult result, bool clearCallbacks);
	void onConnectionEstablishedFirstTime();

	void onContextStateChanged();

	static void contextStateCallback(pa_context* ctx, void* userdata);

	std::vector<IGenerator*> generators_;
};

} // namespace wavegen

#endif /* WAVEGENCLIENT_H_ */
