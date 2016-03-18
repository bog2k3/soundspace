/*
* AsyncOps.h
*
*  Created on: March 18, 2016
*      Author: bog
*/

#ifndef ASYNCOPS_H_
#define ASYNCOPS_H_

#include <functional>

namespace wavegen {

enum class OperationType {
	Connect,

};

enum class OperationResult {
	Failed,
	Success,
	InProgress,
};

/*struct OperationContext {
	OperationType type;
	OperationResult result;

	OperationContext(OperationType type, OperationResult result)
	: type(type), result(result)
	{}
	};*/

using AsyncOperationCb = std::function<void(OperationResult result)>;

} // namespace wavegen

#endif /* ASYNCOPS_H_ */