#include "CommandTime.h"
#include <chrono>
#include <ctime>
#include "InvalidOptionError.h"
#include "TooManyArgumentsError.h"
#include "AttemptedInputRedirectionError.h"
#include "AttemptedOutputRedirectionError.h"
#include "BadFormatError.h"

CommandTime::CommandTime(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
    std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
    : Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}

void CommandTime::checkFormat() {
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

void CommandTime::checkParams() {
    if (!getOpt().empty()) {
        throw new InvalidOptionError(getName(), getOpt()[0]);
    }
    else if (!getFirstArgument().empty()) {
        throw new TooManyArgumentsError(getName());
    }
    else if (!getInputRedirection().empty()) {
        throw new AttemptedInputRedirectionError(getName());
    }
    //else if (!getOutputRedirectionNew().empty() && !getOutputRedirectionAppend().empty()) {
    //    throw new AttemptedOutputRedirectionError(getName());
    //}
}

std::string CommandTime::executeCommand() {
    auto now = std::chrono::system_clock::now();
    time_t now_time = std::chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &now_time);
    std::string text = time(local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
    return text;
}

std::string CommandTime::time(int h, int min, int s) {
    std::string hours;
    std::string minutes;
    std::string seconds;

    hours = (h < 10 ? "0" : "") + std::to_string(h);
    minutes = (min < 10 ? "0" : "") + std::to_string(min);
    seconds = (s < 10 ? "0" : "") + std::to_string(s);

    std::string text = hours + ":" + minutes + ":" + seconds;

    return text;
}
