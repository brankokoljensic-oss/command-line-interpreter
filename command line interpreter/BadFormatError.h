#pragma once
#include "CustomError.h"
class BadFormatError : public CustomError {
public:
	BadFormatError(std::string command_name);
};

