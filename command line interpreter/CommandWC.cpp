#include "CommandWC.h"
#include <fstream>
#include <sstream>
#include "MissingOptionError.h"
#include "MissingFileError.h"
#include "InvalidOptionError.h"
#include "InvalidInputRedirectionError.h"
#include "BadFormatError.h"

CommandWC::CommandWC(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
	std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
	: Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}

void CommandWC::checkFormat() {
	std::vector <DataType> types = getTypesOfParams();
	bool option_seen = false;
	bool argument_seen = false;
	bool input_red_seen = false;
	bool output_red_seen = false;
	for (size_t i = 1; i < types.size(); i++) {
		if (types[i] == DataType::option) {
			if (input_red_seen || output_red_seen || option_seen) {
				throw new BadFormatError(getName());
			}
			option_seen = true;
		}
		else if (types[i] == DataType::argument) {
			if (input_red_seen || output_red_seen || !option_seen || argument_seen) {
				throw new BadFormatError(getName());
			}
			argument_seen = true;
		}
		else if (types[i] == DataType::input_redirection) {
			if (input_red_seen) {
				throw new BadFormatError(getName());
			}
			input_red_seen =true;
		}
		else if (types[i] == DataType::output_redirection) {
			if (output_red_seen) {
				throw new BadFormatError(getName());
			}
			output_red_seen = true;
		} 
	}
}

void CommandWC::checkParams() {
	if (!getOpt().empty() && !(getOpt()[0] == "-w" || getOpt()[0] == "-c")) {
		throw new InvalidOptionError(getName(), getOpt()[0]);
	}
	else if (getOpt().empty()){
		throw new MissingOptionError(getName());
	}
	if (!checkFirstArgument() && !checkInputRedirection()) {
		throw new InvalidInputRedirectionError(getName());
	}
	if (checkFirstArgument() && checkInputRedirection()) {
		setLoopFlag(true);  //Moram sam da pisem
	}
	else {
		setLoopFlag(false);
	}
}

std::string CommandWC::executeCommand() {
	int count = 0;
	
	if (getLoopFlag()) {
		while (getLoopFlag()) {
			count = count + wc(std::cin, getOpt()[0]);
		}
		std::string text = std::to_string(count);
		return text;
	}
	else {
		if (checkNature(getFirstArgument()) == is_argument) {
			std::string line = getFirstArgument();
			line = stripArg(line);
			setFirstArgument(line);
			count = wc(getOpt()[0]);
			std::string text = std::to_string(count);
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
					count = count + wc(file, getOpt()[0]);
				}
				file.close();
				std::string text = std::to_string(count);
				return text;
			}
		}
	}
}

int CommandWC::wc(std::string opt) {
	int count = 0;
	std::string line = getFirstArgument();
	//OVO BIH SA SWITCH RADIO, ALI NE RADI SWITCH SA STRINGOVIMA
	if (opt == "-w") {
		std::istringstream iss(line); //Pravi stream od stringa sa kog mogu da citam
		std::string word;
		while (iss >> word) {
			count++;
		}
	}
	else if (opt == "-c") {
		line = getFirstArgument();
		count = line.length();
	}

	return count;
}

int CommandWC::wc(std::istream& stream, std::string opt) {
	int count = 0;
	std::string line;

	if (std::getline(stream, line)) {
		setFirstArgument(line);
		if (opt == "-w") {
			std::istringstream iss(line);
			std::string word;
			while (iss >> word) {
				count++;
			}
		}
		else if (opt == "-c") {
			line = getFirstArgument();
			count = line.length() + 1; // +1 da uracuna i \n
		}

		setLoopFlag(true);
	}
	else if (stream.eof()) { //ne prethodnom citanju pita da li je video EOF
		std::cin.clear();
		setLoopFlag(false);
	}
	else {
		std::cin.clear();
		throw; //za svaki slucaj, ako negde pukne
	}
	return count;
}
