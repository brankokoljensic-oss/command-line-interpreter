#pragma once
#include "ReadError.h"
class UnexpectedCharactersError : public ReadError {
public:
	UnexpectedCharactersError(std::string input,std::vector <size_t> positions);
};

