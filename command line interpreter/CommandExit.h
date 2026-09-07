#pragma once
#include "Command.h"
class CommandExit : public Command {
public:
	CommandExit(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandExit() {};


	void checkFormat() {}; //nema sta da se gleda, regulise sve ckeck params
	void checkParams() override;

	std::string executeCommand() override;

	std::string exit();
};

