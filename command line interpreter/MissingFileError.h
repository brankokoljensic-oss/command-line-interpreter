#pragma once
#include "CustomError.h"
class MissingFileError : public CustomError
{
public:
	MissingFileError(std::string command_first_argument);
};

