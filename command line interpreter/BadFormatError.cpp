#include "BadFormatError.h"

BadFormatError::BadFormatError(std::string command_name) 
	:CustomError("Error: bad format for command \"" + command_name + "\"")
{}
