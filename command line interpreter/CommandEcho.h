#pragma once
#include "Command.h"
class CommandEcho :public Command {
public:
	
	CommandEcho(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandEcho() override {};

	void checkFormat();
	void checkParams() override;

	std::string executeCommand() override;

	std::string echo(std::istream& stream);
	std::string echo();
};

