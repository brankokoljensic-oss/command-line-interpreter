#pragma once
#include "Command.h"
class CommandPrompt : public Command {
public:
	CommandPrompt(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandPrompt() {};

	void checkFormat();
	void checkParams();

	std::string executeCommand() override;
	void printResult(std::string) override;

	void prompt();
};

