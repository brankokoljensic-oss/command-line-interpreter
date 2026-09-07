#pragma once
#include "CustomError.h"
class AttemptedInputRedirectionError : public CustomError{
public: 
	AttemptedInputRedirectionError(std::string command_name);
};

