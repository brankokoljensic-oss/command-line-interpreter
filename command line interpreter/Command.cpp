#include "Command.h"
#include "MissingFileError.h"
#include <fstream>

Command::Command(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
    std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
    : name (name)
    , options (options)
    , arguments (arguments)
    , input_redirection (input_redirection)
    , output_redirection_new (output_redirection_new)
    , output_redirection_append (output_redirection_append)
    , types_of_params (types_of_params)
{}



bool Command::checkFirstArgument() {
    if (arguments.size() > 0) {
        return arguments[0].empty();
    }
    else {
        return arguments.empty();
    }
}

bool Command::checkInputRedirection() {
    return input_redirection.empty();
}

bool Command::checkOutputRedirectionNew() {
    return output_redirection_new.empty();
}

bool Command::checkOutputRedirectionAppend() {
    return output_redirection_append.empty();
}

Command::Nature Command::checkNature(std::string str) {
    if (!str.empty() && str.front() == '"' && str.back() == '"') {
        return is_argument;
    }
    else {
        return is_file;
    };
}

std::string Command::stripArg(std::string str) {
    if (str.size() >= 2) {
        str.erase(0, 1);       //ukloni prvi  
        str.pop_back();         //ukloni poslednji
        //str.substr(1, str.size() - 2);
    }
    return str;
}

std::string Command::stripInRed() {
    std::string input_source = input_redirection;
    if (input_source.size() > 1) {
        input_source.erase(0, 1);
    }
    else {
        throw new CustomError("Error: bad input redirection for command \"" + getName() + "\"");
    }
    return input_source;
}

std::string Command::stripOutRedNew() {
    std::string output_flow = output_redirection_new;
    if (output_flow.size() > 1) {
        output_flow.erase(0, 1);
    }
    else {
        throw new CustomError("Error: bad output redirection for command \"" + getName() + "\"");
    }
    return output_flow;
}

std::string Command::stripOutRedAppend() {
    std::string output_flow = output_redirection_append;
    if (output_flow.size() > 2) {
        output_flow.erase(0, 2);
    }
    else {
        throw new CustomError("Error: bad output redirection for command \"" + getName() + "\"");
    }
    return output_flow;
}

std::string Command::getName() {
    return this -> name;
}

std::vector <std::string> Command::getOpt() {
    return this->options;
}

std::string Command::getFirstArgument() {
    if (!arguments.empty()) {
        return this->arguments[0];
    }
    else {
        return "";
    }
}

std::string Command::getSecondArgument() {
    if (arguments.size() > 1) {
        return this->arguments[1];
    }
    else {
        return "";
    }
}

std::string Command::getInputRedirection() {
    return input_redirection;
}

std::string Command::getOutputRedirectionNew() {
    return output_redirection_new;
}

std::string Command::getOutputRedirectionAppend() {
    return output_redirection_append;
}

std::vector<DataType> Command::getTypesOfParams() {
    return this->types_of_params;
}

void Command::setFirstArgument(std::string str) {
    if (!arguments.empty()) {
        this->arguments[0] = str;
    }
    else {
        arguments.push_back(str);
    }
}

void Command::setSecondArgument(std::string str) {
    if (arguments.size() > 1) {
        this->arguments[1] = str;
    }
    else if (arguments.size() == 1) {
        arguments.push_back(str);
    }
}

void Command::setOutputRedirectionNew(std::string str) {
    this->output_redirection_new = str;
}

void Command::setOutputRedirectionAppend(std::string str) {
    this->output_redirection_append = str;
}

bool Command::getLoopFlag()
{
    return this->loop_flag;
}

void Command::setLoopFlag(bool a) {
    this->loop_flag = a;
    return;
}

void Command::printResult(std::string result) {
    if (!output_redirection_new.empty()) {

        std::string filename = stripOutRedNew();

        std::ofstream file(filename);
        if (!file) {
            throw new CustomError("File\"" + filename + "\" can't be made");
        }
        file << result << std::flush;
    }
    else if (!output_redirection_append.empty()) {
        std::string filename = stripOutRedAppend();

        std::ofstream file(filename, std::ios::out | std::ios::app); //PROBATI I SAMO SA std::ios::app, trebalo bi da moze i tako
        if (!file) {
            throw new CustomError("File\"" + filename + "\" can't be made");
        }
        file << result << std::flush;
    }
    else {
        std::cout << result << std::endl;
    } 
}