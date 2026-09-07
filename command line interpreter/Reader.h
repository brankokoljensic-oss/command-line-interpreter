#pragma once
#include <string>
#include <vector>
#include "DataType.h"

class Reader {
public:
	static std::string Truncate(std::string str); // skracuje liniju na 512 karaktera
	static void ParsePipeline(std::string str);//deli ulaz sa terminala
	static void ParseCommand(std::string str); // deli komandu i smesta odg. parametre
	static void checkGrammar(std::string str);

	static std::string getCommandName() { return Reader::command_name; };
	static std::vector <std::string> getCommandOpt() { return Reader::command_options; };
	static std::vector <std::string> getCommandArguments() { return Reader::command_arguments; };
	static std::string getCommandInputRedirection() { return Reader::command_input_redirection; };
	static std::string getCommandOutputRedirectionNew() { return Reader::command_output_redirection_new; };
	static std::string getCommandOutputRedirectionAppend() { return Reader::command_output_redirection_append; };
	static std::vector <DataType> getCommandTypesOfParams() {return Reader::command_types_of_params;}

	static void ResetReader();
private:

	static std::string command_name;
	static std::vector <std::string> command_options;
	static std::vector <std::string> command_arguments;
	static std::string command_input_redirection;
	static std::string command_output_redirection_new;
	static std::string command_output_redirection_append;
	static std::vector <DataType> command_types_of_params;

	enum Read_state {
		reading_name, waiting_non_blank, reading_option, reading_argument, reading_input_redirection,
		reading_output_redirection_new, reading_output_redirection_append, waiting_non_blank_redirection, done };
	static Read_state curr_state;


	static size_t determineNextState(size_t i, std::string input, char c);

	static std::vector<char> banned_chars; 


};

