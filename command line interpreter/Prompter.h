#pragma once
#include <iostream>
#include <string>

class Prompter {
public:

	static Prompter* Instance();
	std::string getPrompt();
	void setPrompt(std::string prompt);

private:
	static Prompter* s_instance;
	Prompter();
	std::string prompt = "";
};

