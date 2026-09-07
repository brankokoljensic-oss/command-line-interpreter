#pragma once
# include <iostream>
# include <string>
# include <vector>
# include "DataType.h"

class Command {
public:
	enum Nature {is_file, is_argument};
	virtual ~Command() {};

	virtual void checkFormat() = 0; 
	virtual void checkParams() = 0; // OVO MORA PONOVO SVIMA DA SE ISPISE

	bool checkFirstArgument(); //Proverava da li postoji argument
	bool checkInputRedirection();
	bool checkOutputRedirectionNew();
	bool checkOutputRedirectionAppend();
	Nature checkNature(std::string str);            

	std::string stripArg(std::string str); //sklanja navodnike
	std::string stripInRed();
	std::string stripOutRedNew(); 
	std::string stripOutRedAppend();


	std::string getName();
	std::vector <std::string> getOpt();
	std::string getFirstArgument();
	std::string getSecondArgument();
	std::string getInputRedirection();
	std::string getOutputRedirectionNew();
	std::string getOutputRedirectionAppend();
	std::vector <DataType> getTypesOfParams();


	void setFirstArgument (std::string str);
	void setSecondArgument(std::string str);
	void setOutputRedirectionNew(std::string str);
	void setOutputRedirectionAppend(std::string str);

	bool getLoopFlag();
	void setLoopFlag(bool a);

	virtual std::string executeCommand() = 0;
	virtual void printResult(std::string result);  

protected:
	Command(std::string name, std::vector<std::string> options, std::vector<std::string> arguments, std::string input_redirection,
		std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_or_params);

private:
	std::string name = "";
	std::vector <std::string> options;
	std::vector <std::string> arguments;
	std::string input_redirection = "";
	std::string output_redirection_new = "";
	std::string output_redirection_append = "";
	bool loop_flag = false;

	Nature nature_of_first_argument = is_argument;
	std::vector <DataType> types_of_params;
};

