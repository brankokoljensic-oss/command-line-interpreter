#pragma once
#include "CustomError.h"
#include <vector>
class ReadError : public CustomError{
public:
	ReadError(std::string massage, std::string input, std::vector <size_t> positions);
	void BonusInfo();
protected:
	std::vector <size_t> positions;
	std::string input;
};

