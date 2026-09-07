#include "PipelineOrderError.h"

PipelineOrderError::PipelineOrderError(std::string command_name) 
	: CustomError("Error: command \"" + command_name + "\" can't be in this segment of the pipeline")
{}
