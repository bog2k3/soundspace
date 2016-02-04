/*
 * WavegenClient.h
 *
 *  Created on: Feb 4, 2016
 *      Author: bog
 */

#ifndef WAVEGENCLIENT_H_
#define WAVEGENCLIENT_H_

#include <string>
#include <functional>

struct pa_context;
struct pa_threaded_mainloop;
struct pa_mainloop_api;

class WavegenClient {
public:

	struct OperationContext {

	};

	typedef std::function<bool(OperationContext* pCtx)> AsyncOperationCb;

	WavegenClient(std::string const& name);
	virtual ~WavegenClient();

	bool connect(std::string const& serverAddr);
	void connectAsync(std::string const& serverAddr, AsyncOperationCb cb);

private:
	std::string name_;
	pa_threaded_mainloop *paMainLoop_ = nullptr;
	pa_mainloop_api *paMainApi_ = nullptr;
	pa_context *paContext_ = nullptr;

	void onContextStateChanged();

	static void contextStateCallback(pa_context* ctx, void* userdata);
};

#endif /* WAVEGENCLIENT_H_ */
