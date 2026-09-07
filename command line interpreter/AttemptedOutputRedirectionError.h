#pragma once
#include "CustomError.h"
class AttemptedOutputRedirectionError : public CustomError {
public:
	AttemptedOutputRedirectionError(std::string command_name);
};

