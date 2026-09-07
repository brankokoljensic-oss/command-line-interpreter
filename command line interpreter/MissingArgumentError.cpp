#include "MissingArgumentError.h"

MissingArgumentError::MissingArgumentError(std::string command_name) 
	: CustomError("Error: command \"" + command_name + "\" is missing an argument")
{}
