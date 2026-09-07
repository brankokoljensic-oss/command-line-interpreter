#pragma once
#include "CustomError.h"
class MissingOptionError : public CustomError {
public:
	MissingOptionError(std::string command_name);
};

