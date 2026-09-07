#include "CommandTR.h"
#include <fstream>
#include "InvalidOptionError.h"
#include "InvalidInputRedirectionError.h"
#include "MissingOptionError.h"
#include "MissingFileError.h"
#include "BadFormatError.h"

CommandTR::CommandTR(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
	std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
	: Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}


void CommandTR::checkFormat() {
	std::vector <DataType> types = getTypesOfParams();
	bool option_seen = false;
	bool argument_seen = false;
	bool argument_seen_twice = false;
	bool input_red_seen = false;
	bool output_red_seen = false;
	for (size_t i = 1; i < types.size(); i++) {
		if (types[i] == DataType::argument) {
			if (input_red_seen || output_red_seen || argument_seen_twice) {
				throw new BadFormatError(getName());
			}
			else if (argument_seen && !option_seen) {
				throw new BadFormatError(getName());
			}
			else if (!argument_seen && option_seen) {
				std::string first_argument = getFirstArgument();
				setFirstArgument("");
				setSecondArgument(first_argument);
				argument_seen_twice = true;
			}
			else if (argument_seen && option_seen) {
				argument_seen_twice = true;
			}
			argument_seen = true;
		}
		else if (types[i] == DataType::option) {
			if (input_red_seen || output_red_seen || option_seen) {
				throw new BadFormatError(getName());
			}
			option_seen = true;
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

bool CommandTR::validateOption() {
	std::string option = getOpt()[0];
	if (option.size() < 4) {
		return false;
	}
	else if (option[1] != '"' || option.back() != '"') {
		return false;
	}
	return true;
}

void CommandTR::checkParams() {
	if (getOpt().empty()) {
		throw new MissingOptionError(getName());
	}
	else if (!validateOption()) {
		throw new InvalidOptionError(getName(), getOpt()[0]);
	}
	if (!getSecondArgument().empty()) {
		Nature nature = checkNature(getSecondArgument());
		if (nature != is_argument) {
			throw new CustomError("Error: second argument must have quotation marks");
		}
	}
	if (!checkFirstArgument() && !checkInputRedirection()) {
		throw new InvalidInputRedirectionError(getName());
	}
	if (checkFirstArgument() && checkInputRedirection()) {
		setLoopFlag(true);  //Moram sam da pisem argument
	}
	else {
		setLoopFlag(false);
	}
}

std::string CommandTR::executeCommand() {
	std::string text = "";
	bool first_appended = false;
	if (!getSecondArgument().empty()) {
		setSecondArgument(stripArg(getSecondArgument()));
	}

	if (getLoopFlag()) {
		while (getLoopFlag()) {
			std::string tr_text = tr(std::cin);
			if (tr_text == "" && !getLoopFlag()) {
				continue;
			}
			else if (!first_appended) {
				text = text + tr_text;
				first_appended = true;
			}
			else {
				text = text + '\n' + tr_text;
			}
		}
		return text;
	}
	else {
		if (checkNature(getFirstArgument()) == is_argument) {
			std::string line = getFirstArgument();
			line = stripArg(line);
			setFirstArgument(line);
			text = tr();
			return text;
		}
		else {
			std::string filename;
			if (checkInputRedirection()) {
				filename = getFirstArgument();
			}
			else {
				filename = stripInRed();
			}

			std::ifstream file(filename);
			if (!file) {
				throw new MissingFileError(filename);
			}
			else {
				setLoopFlag(true);
				while (getLoopFlag()) {
					std::string tr_text = tr(file);
					if (tr_text == "" && !getLoopFlag()) {
						continue;
					}
					else if (!first_appended) {
						text = text + tr_text;
						first_appended = true;
					}
					else {
						text = text + '\n' + tr_text;
					}
				}
				file.close();
				return text;
			}
		}
	}
}

std::string CommandTR::tr(std::istream& stream) {
	std::string line;
	std::string text;
	std::string what = getOpt()[0].substr(2, getOpt()[0].size() - 3); //drugi argument je broj karaktera od prvog navedenog, linija uzima ono izmedju -" i "
	std::string with = getSecondArgument();
	size_t pos = 0;
	size_t found = 0;

	if (std::getline(stream, line)) {
		while ((found = line.find(what, pos)) != std::string::npos) {
			text.append(line, pos, found - pos); //drugi argument predstavlja broj karaktera od prvog navedenog
			text = text + with;
			pos = found + what.length();
		}
		text.append(line, pos, std::string::npos);
		return text;
	}
	else if (stream.eof()) { //na prethodnom citanju jesam li dobio EOF
		stream.clear(); 
		setLoopFlag(false);
		return "";
	}
	else {
		stream.clear();
		throw;  //za svaki slucaj, ako negde pukne
	}
}

std::string CommandTR::tr() {
	std::string line = getFirstArgument();
	std::string text;
	std::string what = getOpt()[0].substr(2, getOpt()[0].size() - 3);
	std::string with = getSecondArgument();
	size_t pos = 0;
	size_t found = 0;


	while ((found = line.find(what, pos)) != std::string::npos) {
		text.append(line, pos, found - pos); 
		text = text + with;
		pos = found + what.length();
	}
	text.append(line, pos, std::string::npos);
	return text;
}

