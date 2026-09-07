#pragma once
#include <iostream>
#include <string>
//#include <memory>
class CommandLineInterpreter {
public:
	static CommandLineInterpreter* Instance();
	void StartSystem();  //Pokrece program
	~CommandLineInterpreter() {};

	void setProgramRunning(bool a);
private:
	static CommandLineInterpreter* s_instance;
	CommandLineInterpreter() {};
	bool programRunning = true;
};


