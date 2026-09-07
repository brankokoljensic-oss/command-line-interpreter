#pragma once
#include "CustomError.h"
class PipelineOrderError : public CustomError {
public:
	PipelineOrderError(std::string command_name);
};

