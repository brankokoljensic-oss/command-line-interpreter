#include "MissingFileError.h"

MissingFileError::MissingFileError(std::string command_first_argument) 
	: CustomError ("Error: file \"" + command_first_argument +  "\" doesn't exist")
{}
