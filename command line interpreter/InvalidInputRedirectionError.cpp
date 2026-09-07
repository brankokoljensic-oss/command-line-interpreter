#include "InvalidInputRedirectionError.h"

InvalidInputRedirectionError::InvalidInputRedirectionError(std::string command_name)
	: CustomError("Error: command \"" + command_name + "\" already has input source")
{}
