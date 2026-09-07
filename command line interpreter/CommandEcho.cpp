#include "CommandEcho.h"
#include <fstream>
#include "MissingFileError.h"
#include "InvalidOptionError.h"
#include "InvalidInputRedirectionError.h"
#include "BadFormatError.h"

CommandEcho::CommandEcho(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
	std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
	: Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}

void CommandEcho::checkFormat() {
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

void CommandEcho::checkParams() {
	if (!getOpt().empty()) {
		throw new InvalidOptionError(getName(), getOpt()[0]);
	}
	else if (!checkFirstArgument() && !checkInputRedirection()) {
		throw new InvalidInputRedirectionError(getName());
	}
	if (checkFirstArgument() && checkInputRedirection()) {
		 setLoopFlag(true);  //Moram sam da pisem argument
	}
	else {
		setLoopFlag(false);
	}
}

std::string CommandEcho::executeCommand() {
	std::string text = "";
	bool first_appended = false;
	if (getLoopFlag()) {
		while (getLoopFlag()) {
			std::string echo_text = echo(std::cin);
			if (echo_text == "" && !getLoopFlag()) {
				continue;
			}
			else if (!first_appended) {
				text = text + echo_text;
				first_appended = true;
			}
			else {
				text = text + '\n' + echo_text;
			}
		}
		return text;
	}
	else {
		if (checkNature(getFirstArgument()) == is_argument) {
			std::string line = getFirstArgument();
			line = stripArg(line);
			setFirstArgument(line);
			text = echo();
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
				while (getLoopFlag()){
					std::string echo_text = echo(file);
					if (echo_text == "" && !getLoopFlag()) {
						continue;
					}
					else if (!first_appended) {
						text = text + echo_text;
						first_appended = true;
					}
					else {
						text = text + '\n' + echo_text;
					}
				}
				file.close();
				return text;
			}
		}
	}

}

std::string CommandEcho::echo(std::istream& stream) {

	std::string line;

	if (std::getline(stream, line)) {
		setFirstArgument(line);
		setLoopFlag(true);
		return getFirstArgument();
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

std::string CommandEcho::echo() {
	return getFirstArgument();
}

