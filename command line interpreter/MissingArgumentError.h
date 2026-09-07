#pragma once
#include "CustomError.h"
class MissingArgumentError : public CustomError {
public:
	MissingArgumentError(std::string command_name);
};

