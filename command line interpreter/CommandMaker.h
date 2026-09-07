#pragma once
#include <vector>
#include <string>
#include <memory>
#include "DataType.h"

class Command;


class CommandMaker {
public:
	static CommandMaker* Instance();
	std::unique_ptr<Command>  makeCommand(std::string name, std::vector<std::string> options, std::vector<std::string> arguments,
		std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params);
private:
	static CommandMaker* s_instance;

};

