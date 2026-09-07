#include "InvalidOptionError.h"

InvalidOptionError::InvalidOptionError(std::string command_name, std::string command_opt) 
	: CustomError ("Error: command \"" + command_name +"\" has no option \"" + command_opt + "\"")
{}
