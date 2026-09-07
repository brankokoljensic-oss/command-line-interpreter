#pragma once
#include "CustomError.h"
class UnknownCommandError : public CustomError {
public:
	UnknownCommandError(std::string command_name);
};

