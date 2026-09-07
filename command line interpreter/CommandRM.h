#pragma once
#include "Command.h"
class CommandRM : public Command {
public:
	CommandRM(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandRM() {};

	void checkFormat();//nema sta da se gleda, regulise sve ckeck params
	void checkParams();

	std::string executeCommand();
	void printResult(std::string result) override;
	void rm();
};

