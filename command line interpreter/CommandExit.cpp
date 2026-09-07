#include "CommandExit.h"
#include "CommandlineInterpreter.h"
#include "InvalidOptionError.h"
#include "TooManyArgumentsError.h"
#include "AttemptedInputRedirectionError.h"
#include "AttemptedOutputRedirectionError.h"
#include "BadFormatError.h"

CommandExit::CommandExit(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
    std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
    : Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}

void CommandExit::checkParams(){
    if (!getOpt().empty()) {
        throw new InvalidOptionError(getName(), getOpt()[0]);
    }
    else if (!getFirstArgument().empty()) {
        throw new TooManyArgumentsError(getName());
    }
    else if (!getInputRedirection().empty()) {
        throw new AttemptedInputRedirectionError(getName());
    }
    else if (!getOutputRedirectionNew().empty() || !getOutputRedirectionAppend().empty()) {
        throw new AttemptedOutputRedirectionError(getName());
    }
}

std::string CommandExit::executeCommand() {  
    std::string text = exit();
    return text;
}

std::string CommandExit::exit() {
    std::string text = "Closing program...";
    CommandLineInterpreter::Instance()->setProgramRunning(false);
    return text;
}
