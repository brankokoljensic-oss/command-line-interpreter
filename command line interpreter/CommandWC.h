#pragma once
#include "Command.h"

class CommandWC : public Command {
public:
	CommandWC(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandWC() override {};

	void checkFormat();
	void checkParams() override;

	std::string executeCommand() override;

	int wc(std::string opt);
	int wc(std::istream& stream, std::string opt);
};

