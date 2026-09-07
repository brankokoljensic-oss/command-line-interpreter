#include "UnexpectedArgumentsError.h"

UnexpectedArgumentsError::UnexpectedArgumentsError(std::string input, std::vector <size_t> positions)
	: ReadError("Error: unexpected arguments found", input, positions)
{}