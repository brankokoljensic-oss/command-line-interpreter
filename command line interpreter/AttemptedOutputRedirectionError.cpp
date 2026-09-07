#include "AttemptedOutputRedirectionError.h"

AttemptedOutputRedirectionError::AttemptedOutputRedirectionError(std::string command_name) 
	: CustomError("Error: command \"" + command_name + "\" can't have it's output flow redirected")
{}
