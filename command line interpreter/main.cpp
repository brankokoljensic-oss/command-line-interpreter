# include <iostream>
# include <string>
# include <exception>
# include "CommandlineInterpreter.h"


int main() {
	
	CommandLineInterpreter* CLI = CommandLineInterpreter::Instance();
	CLI->StartSystem();
	

	delete CLI;
	return 0;
}


//Oznaka da je ovo verzija na kompu