#pragma once
#include "CustomError.h"
class InvalidOptionError : public CustomError {
public:
	InvalidOptionError(std::string command_name, std::string command_opt);
};

