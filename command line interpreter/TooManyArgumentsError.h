#pragma once
#include "CustomError.h"
class TooManyArgumentsError : public CustomError {
public: 
	TooManyArgumentsError(std::string command_name);

};

