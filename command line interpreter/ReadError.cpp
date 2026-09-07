#include "ReadError.h"
#include <iostream>

ReadError::ReadError(std::string massage,std::string input, std::vector <size_t> positions)
	: CustomError(massage)
	, positions(positions)
	, input(input)
{}

void ReadError::BonusInfo() {
	std::string error_line (input.length(), ' ');
	for (size_t k = 0; k < input.length(); k++) { //uradjeno da bi se karakter na dobrom mestu prikazao
		char c = input[k];
		if (c == ' ' || c == '\t') {
			error_line[k] = c;
		}
		else {
			error_line[k] = ' ';
		}
	}

	for (size_t i = 0; i < positions.size(); i++) {
		error_line[positions[i]] = '^';
	}
	std::cout << input << std::endl;
	std::cout << error_line << std::endl;
	return;
}
