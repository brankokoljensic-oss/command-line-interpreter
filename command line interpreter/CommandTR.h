#pragma once
#include "Command.h"
class CommandTR : public Command {
public:
	CommandTR(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
	~CommandTR() override {};
	
	void checkFormat();
	void checkParams() override;

	std::string executeCommand() override;

	std::string tr(std::istream& stream);
	std::string tr();
protected:
	bool validateOption();
};

