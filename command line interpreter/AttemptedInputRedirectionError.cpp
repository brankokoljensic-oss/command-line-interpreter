#include "AttemptedInputRedirectionError.h"

AttemptedInputRedirectionError::AttemptedInputRedirectionError(std::string command_name) 
	: CustomError("Error: command \"" + command_name + "\" can't have it's input source redirected")
{}
