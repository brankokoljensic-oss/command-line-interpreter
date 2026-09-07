#include "CommandDate.h"
#include <chrono>
#include <ctime>
#include "InvalidOptionError.h"
#include "TooManyArgumentsError.h"
#include "AttemptedInputRedirectionError.h"
#include "AttemptedOutputRedirectionError.h"
#include "BadFormatError.h"

CommandDate::CommandDate(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
    std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
    : Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}


void CommandDate::checkFormat() {
    std::vector <DataType> types = getTypesOfParams();
    //bool option_seen = false;
    //bool argument_seen = false;
    bool input_red_seen = false;
    bool output_red_seen = false;
    for (size_t i = 1; i < types.size(); i++) {
        if (types[i] == DataType::input_redirection) {
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

void CommandDate::checkParams() {
    if (!getOpt().empty()) {
        throw new InvalidOptionError(getName(), getOpt()[0]);
    }
    else if (!getFirstArgument().empty()) {
        throw new TooManyArgumentsError(getName());
    }
    else if(!getInputRedirection().empty()){
        throw new AttemptedInputRedirectionError(getName());
    }
    //else if (!getOutputRedirectionNew().empty() && !getOutputRedirectionAppend().empty()) {
    //    throw new AttemptedOutputRedirectionError(getName());
    //}
}

std::string CommandDate::executeCommand() {
    auto now = std::chrono::system_clock::now();
    time_t now_time = std::chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &now_time);
    std::string result = date(local_time.tm_mday, local_time.tm_mon, local_time.tm_year);
    return result;
}

std::string CommandDate::date(int d, int m, int y) {
    std::string days;
    std::string months;
    std::string years;

    days = (d < 10 ? "0" : "") + std::to_string(d);
    months = ((m+1) < 10 ? "0" : "") + std::to_string(m + 1);
    years = std::to_string(y + 1900);
    std::string result = days + "." + months + "." + years;
    return result;
}