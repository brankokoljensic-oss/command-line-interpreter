#pragma once
#include "CustomError.h"
class InvalidInputRedirectionError : public CustomError {
public:
	InvalidInputRedirectionError(std::string command_name);
};

