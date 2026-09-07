#include "UnexpectedCharactersError.h"

UnexpectedCharactersError::UnexpectedCharactersError(std::string input, std::vector <size_t> positions)
	: ReadError("Error: unexpected characters found", input, positions ) 
{}