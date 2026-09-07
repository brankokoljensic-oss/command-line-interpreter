#include "TooManyArgumentsError.h"

TooManyArgumentsError::TooManyArgumentsError(std::string command_name) 
	: CustomError ("Error: command \"" + command_name + "\" doesn't take this many arguments")
{}
