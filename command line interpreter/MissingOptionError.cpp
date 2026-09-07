#include "MissingOptionError.h"

MissingOptionError::MissingOptionError(std::string command_name) 
	: CustomError("Error: command  \"" + command_name + "\" is missing an option")
{}
