#include "CommandTruncate.h"
#include <fstream>
#include "MissingArgumentError.h"
#include "InvalidOptionError.h"
#include "AttemptedInputRedirectionError.h"
#include "AttemptedOutputRedirectionError.h"
#include "MissingFileError.h"
#include "BadFormatError.h"

CommandTruncate::CommandTruncate(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
	std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
	: Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{
}

void CommandTruncate::checkFormat() {
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

void CommandTruncate::checkParams() {
	if (!getOpt().empty()) {
		throw new InvalidOptionError(getName(), getOpt()[0]);

	}
	else if (checkFirstArgument()) {
		throw new MissingArgumentError(getName());
	}
	else {
		Nature nature = checkNature(getFirstArgument());
		if (nature != is_file) {
			throw new CustomError("Error: argument must be a file");
		}
	}

	if (!getInputRedirection().empty()) {
		throw new AttemptedInputRedirectionError(getName());
	}
	else if (!getOutputRedirectionNew().empty() || !getOutputRedirectionAppend().empty()) {
		throw new AttemptedOutputRedirectionError(getName());
	}
}

std::string CommandTruncate::executeCommand() {
	truncate();
	return "File contents deleted";
}

void CommandTruncate::printResult(std::string result) {
	return;
}

void CommandTruncate::truncate() {
	std::string filename;
	filename = getFirstArgument();
	std::ifstream check_file(filename);
	if (!check_file.good()) {
		throw new MissingFileError(filename);
	}
	check_file.close();

	std::ofstream file_to_trunc(filename, std::ios::trunc);

	/*if (!file_to_trunc) {
		throw new CustomError("Error: failed to open file for truncating: " + filename);
	}*/

}

