#pragma once
#include "Command.h"
class CommandBatch : public Command {
public:

	CommandBatch(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection, 
				std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandBatch() override {};

	void checkFormat();
	void checkParams() override;

	std::string executeCommand() override;
	void printResult(std::string result) override;

	void determineRedirection(); //prosledjuje redirekciju sa batch komande

	void batch();
};

