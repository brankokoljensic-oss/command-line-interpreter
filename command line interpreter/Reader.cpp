#include "Reader.h"
#include "Pipeline.h"
#include "UnexpectedCharactersError.h"



std::string Reader::command_name = "";
std::vector <std::string> Reader::command_options;
std::vector <std::string> Reader::command_arguments;
std::string Reader::command_input_redirection = "";
std::string Reader::command_output_redirection_new = "";
std::string Reader::command_output_redirection_append = "";
Reader::Read_state Reader::curr_state = reading_name;
std::vector <DataType> Reader::command_types_of_params;
std::vector <char> Reader::banned_chars = { '&', '*', '+', '?', '!', '@', '#', '$', '%', '^', '(', ')', '[', ']', '{', '}', ';', '\'', '~', '`' };

std::string Reader::Truncate(std::string str)
{
	if (str.length() > 512) {
		str = str.substr(0, 512);
	}
	return str;
}

size_t Reader::determineNextState(size_t i, std::string input, char c) {
	size_t index = i;
	if (std::isspace(c)) {
		curr_state = waiting_non_blank;
	}
	else if (c == '<') {
		curr_state = reading_input_redirection;
	}
	else if (c == '>') {
		if (input[i+1] == '>') { //ovo je safe jer je poslednji karakter uvek blanko znak
			curr_state = reading_output_redirection_append;
			index++;
		}
		else {
			curr_state = reading_output_redirection_new;
		}
	}
	return index;
}

void Reader::ParsePipeline(std::string str) {
	bool in_quotes = false;
	std::string temp;
	for (size_t i = 0; i < str.size(); ++i) {
		char c = str[i];

		if (c == '"') {
			in_quotes = !in_quotes;
			temp.push_back(c);
		}
		else if (c == '|' && !in_quotes) {
			Pipeline::pipeline_strings.push_back(temp);
			temp.clear();
		}
		else {
			temp.push_back(c);
		}
	}

	Pipeline::pipeline_strings.push_back(temp); //uvek doda poslednju komandu

	return;
}




void Reader::ParseCommand(std::string str) {
	bool in_quotes = false;
	std::string temp = "";
	for (size_t i = 0; i < str.length(); i++) {
		char c = str[i];

		switch (Reader::curr_state) {

		case reading_name:
			if ((std::isspace(c) && !temp.empty()) || c == '>' || c == '<') {
				Reader::command_name = temp;
				Reader::command_types_of_params.push_back(DataType::name);
				temp.clear();
				i = determineNextState(i, str, c);
			}
			else if (!std::isspace(c)){
				temp.push_back(c);
			}
			break;

		case waiting_non_blank:
			if (std::isspace(c)) {
			}
			else if (c == '-') {
				temp.push_back(c);
				curr_state = reading_option;
			}
			else if (c == '<' || c == '>') { 
				i = determineNextState(i, str, c);
			}
			else {
				temp.push_back(c);
				curr_state = reading_argument;
			}
			break;

		case reading_option:
			if (c == '"') {                      /*dodato zbog tr*/
				in_quotes = !in_quotes;			 /*dodato zbog tr*/
				temp.push_back(c);				 /*dodato zbog tr*/
			}
			else if (((std::isspace(c) || c == '<' || c == '>') && !in_quotes) || (i == str.length() - 1)) {  /*dodat drugi uslov zbog tr*/
				Reader::command_options.push_back(temp);
				Reader::command_types_of_params.push_back(DataType::option);
				temp.clear();
				i = determineNextState(i, str, c);
			}
			else {
				temp.push_back(c);
			}
			break;

		case reading_argument:
			if (temp[0] == '"') { //ovo je argument pod navodnicima
				if (c == '"' || (i == str.length() - 1)) {
					temp.push_back(c);
					Reader::command_arguments.push_back(temp);
					Reader::command_types_of_params.push_back(DataType::argument);
					temp.clear();
					curr_state = waiting_non_blank;
				}
				else {
					temp.push_back(c);
				}
			}
			else { //ovo je fajl
				if (std::isspace(c) || c == '<' || c == '>') {
					Reader::command_arguments.push_back(temp);
					Reader::command_types_of_params.push_back(DataType::argument);
					temp.clear();
					i = determineNextState(i, str, c);
				}
				else {
					temp.push_back(c);
				}
			}
			break;

		case reading_input_redirection:
			if (temp.size() < 1) {
				temp.push_back('<');
			}
			if (((std::isspace(c)) && (temp.size() > 1 || (i == str.length() - 1))) || c == '<' || c == '>') {
				Reader::command_input_redirection = temp;
				Reader::command_types_of_params.push_back(DataType::input_redirection);
				temp.clear();
				i = determineNextState(i, str, c);
			}
			else if(!std::isspace(c)){
				temp.push_back(c);
			}
			break;

		case reading_output_redirection_new:
			if (temp.size() < 1) {
				temp.push_back('>');
			}
			if ((std::isspace(c) && (temp.size() > 1 || (i == str.length() - 1))) || c == '<' || c == '>') {
				Reader::command_output_redirection_new = temp;
				Reader::command_types_of_params.push_back(DataType::output_redirection);
				temp.clear();
				i = determineNextState(i, str, c);
			}
			else if (!std::isspace(c)) {
				temp.push_back(c);
			}
			break;

		case reading_output_redirection_append:
			if (temp.size() < 1) {
				temp.push_back('>');
				temp.push_back('>');
			}
			if ((std::isspace(c) && (temp.size() > 2 || (i == str.length() - 1))) || c == '<' || c == '>') {
				Reader::command_output_redirection_append = temp;
				Reader::command_types_of_params.push_back(DataType::output_redirection);
				temp.clear();
				i = determineNextState(i, str, c); 
			}
			else if (!std::isspace(c)) {
				temp.push_back(c);
			}
			break;
		default:
			break;
		}
	}

}

void Reader::checkGrammar(std::string str) {
	bool in_quotes = false;
	std::vector <size_t> error_places;
	for (size_t i = 0; i < str.size(); ++i) {
		char c = str[i];
		if (c == '"') {
			in_quotes = !in_quotes;
		}
		else if ((std::find(banned_chars.begin(), banned_chars.end(), c) != banned_chars.end()) && !in_quotes) {
			error_places.push_back(i);
		}
	}
	if (error_places.size() > 0) {
		throw new UnexpectedCharactersError(str, error_places);
	}
	return;
}

void Reader::ResetReader() {
	Reader::command_name = "";
	Reader::command_options.clear();
	Reader::command_arguments.clear();
	Reader::curr_state = reading_name;
	Reader::command_input_redirection = "";
	Reader::command_output_redirection_new = "";
	Reader::command_output_redirection_append = "";
	command_types_of_params.clear();
}


