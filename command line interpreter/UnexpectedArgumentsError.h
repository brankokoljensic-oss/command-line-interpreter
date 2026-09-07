#pragma once
#include "ReadError.h"
class UnexpectedArgumentsError : public ReadError {
public:
	UnexpectedArgumentsError(std::string input, std::vector <size_t> positions);
};

