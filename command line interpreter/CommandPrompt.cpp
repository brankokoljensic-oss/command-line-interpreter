#include "CommandPrompt.h"
#include "Prompter.h"
#include "InvalidOptionError.h"
#include "MissingArgumentError.h"
#include "AttemptedInputRedirectionError.h"
#include "AttemptedOutputRedirectionError.h"
#include "BadFormatError.h"

CommandPrompt::CommandPrompt(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
	std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
	: Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}

void CommandPrompt::checkParams() {
	if (!getOpt().empty()) {
		throw new InvalidOptionError(getName(), getOpt()[0]);
	}
	if (checkFirstArgument()) {
		throw new MissingArgumentError(getName());
	}
	else {
		Nature nature = checkNature(getFirstArgument());
		if (nature != is_argument) {
			throw new CustomError("Error: argument must have quotation marks");
		}
	}
	if (!getInputRedirection().empty()) {
		throw new AttemptedInputRedirectionError(getName());
	}
	else if (!getOutputRedirectionNew().empty() || !getOutputRedirectionAppend().empty()) {
		throw new AttemptedOutputRedirectionError(getName());
	}
}

void CommandPrompt::checkFormat() {
	std::vector <DataType> types = getTypesOfParams();
	//bool option_seen = false;
	bool argument_seen = false;
	bool input_red_seen = false;
	bool output_red_seen = false;
	for (size_t i = 1; i < types.size(); i++) {
		if (types[i] == DataType::argument) {
			if (input_red_seen || output_red_seen || argument_seen) {
				throw new BadFormatError(getName());
			}
			argument_seen = true;
		}
		else if (types[i] == DataType::input_redirection) {
			if (input_red_seen) {
				throw new BadFormatError(getName());
			}
			input_red_seen = true;
		}
		else if (types[i] == DataType::output_redirection) {
			if (output_red_seen) {
				throw new BadFormatError(getName());
			}
			output_red_seen = true;
		}
	}
}


std::string CommandPrompt::executeCommand() {
	std::string symbol = stripArg(getFirstArgument());
	setFirstArgument(symbol);
	prompt();
	return "Prompt changed";
}

void CommandPrompt::printResult(std::string){
	return;
}

void CommandPrompt::prompt() {
	Prompter::Instance()->setPrompt(this->getFirstArgument());
}
