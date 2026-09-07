#pragma once
#include "Command.h"
class CommandHead : public Command {
public:
	CommandHead(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandHead() override {};

	void checkFormat();
	void checkParams() override;

	std::string executeCommand() override;

	std::string head(int count, std::istream& stream);
	std::string head(int count);
protected:
	bool validateOption();
};

