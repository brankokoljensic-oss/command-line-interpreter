#include "UnknownCommandError.h"

UnknownCommandError::UnknownCommandError(std::string command_name) 
	: CustomError("Error: unknown command: " + command_name) {}
