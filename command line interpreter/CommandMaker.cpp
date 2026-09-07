#include "CommandMaker.h"
#include "CommandEcho.h"
#include "CommandTime.h"
#include "CommandDate.h"
#include "CommandExit.h"
#include "CommandWC.h"
#include "CommandTouch.h"
#include "CommandPrompt.h"
#include "CommandRM.h"
#include "CommandTruncate.h"
#include "CommandHead.h"
#include "CommandBatch.h"
#include "CommandTR.h"
#include "UnknownCommandError.h"

CommandMaker* CommandMaker::s_instance = nullptr;

CommandMaker* CommandMaker::Instance() {
	if (s_instance == nullptr) {
		s_instance = new CommandMaker();
	}
	return s_instance;
}

std::unique_ptr<Command> CommandMaker::makeCommand(std::string name, std::vector<std::string> options, std::vector<std::string> arguments,
	std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params) {
	if (name == "echo") {
		return std::make_unique<CommandEcho>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "time") {
		return std::make_unique<CommandTime>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "date") {
		return std::make_unique<CommandDate>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "exit") {
		return std::make_unique<CommandExit>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "wc") {
		return std::make_unique<CommandWC>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "touch") {
		return std::make_unique<CommandTouch>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "prompt") {
		return std::make_unique<CommandPrompt>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "rm") {
		return std::make_unique<CommandRM>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "truncate") {
		return std::make_unique<CommandTruncate>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "head") {
		return std::make_unique<CommandHead>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "batch") {
		return std::make_unique<CommandBatch>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else if (name == "tr") {
		return std::make_unique<CommandTR>(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params);
	}
	else {
		throw new UnknownCommandError(name);
	}
}
