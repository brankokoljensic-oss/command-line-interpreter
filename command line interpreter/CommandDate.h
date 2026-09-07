#pragma once
#include "Command.h"

class CommandDate : public Command {
public:
	CommandDate(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandDate() override {};

	void checkFormat(); 
	void checkParams() override;

	std::string executeCommand() override;

	std::string date(int d, int m, int y);
};

