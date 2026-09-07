#include "CommandHead.h"
#include <fstream>
#include <sstream>
#include "MissingFileError.h"
#include "InvalidOptionError.h"
#include "InvalidInputRedirectionError.h"
#include "BadFormatError.h"

CommandHead::CommandHead(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
    std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
    : Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}

bool CommandHead::validateOption() {
    std::string option = getOpt()[0];
    if (option.size() < 3 || option.size() > 7) {
        return false;
    }
    else if (option[0] != '-' || option[1] != 'n') {
        return false;
    }
    if (option[2] == '0') //ne moze sa nulom da krece
        return false;

    for (size_t i = 2; i < option.size(); i++) {
        if (!std::isdigit(option[i])) {
            return false;
        }
    }
    return true;
}


void CommandHead::checkFormat() {
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

void CommandHead::checkParams() { 
	if (!validateOption()){
		throw new InvalidOptionError(getName(), getOpt()[0]);
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

std::string CommandHead::executeCommand() {
    std::string text = "";
    std::string option_to_num = getOpt()[0];
    option_to_num = option_to_num.substr(2);
    int number_of_lines = std::stoi(option_to_num);

    if (getLoopFlag()) {
        text = head(number_of_lines, std::cin);
        return text;
    }
    else {
        if (checkNature(getFirstArgument()) == is_argument) {
            std::string line = getFirstArgument();
            line = stripArg(line);
            setFirstArgument(line);
            text = head(number_of_lines);
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
            	text = head(number_of_lines, file);
            	file.close();
            	return text;
            }
        }
    }
}


std::string CommandHead::head(int count, std::istream & stream) { 
    std::string line;
    std::string text = "";
    bool first_appended = false;
    int i = 0;
    while (true) {
        if (std::getline(stream, line)) {
            if (!first_appended && i < count) {
                text = text + line;
                first_appended = true;
            }
            else if(i < count){
                text = text + '\n' + line;
            }
            i++;
        }
        else if (stream.eof()) { //pita da li sam na prethodnom citanju stigao do EOF
            stream.clear(); 
            break;
        }
        else {
            stream.clear();
            throw;  //za svaki slucaj, ako negde pukne
        }
    }
    return text;
}


std::string CommandHead::head(int count) { 
    std::string line = getFirstArgument();
    std::istringstream line_stream(line);
    std::string text = "";
    bool first_appended = false;
    for (int i = count; i > 0; i--) {
        if (std::getline(line_stream, line)) {
            if (!first_appended) {
                text = text + line;
                first_appended = true;
            }
            else {
                text = text + '\n' + line;
            }
        }
        else if (line_stream.eof()) { //na prethodnom citanju jesam li dobio EOF
            line_stream.clear();
            break;
        }
        else {
            throw;  //za svaki slucaj, ako negde pukne
        }
    }
    return text;
}
