#include "CommandlineInterpreter.h"
#include "Prompter.h"
#include "Reader.h"
#include "CommandMaker.h"
#include "Command.h"
#include "CustomError.h" //OVO JE VISAK PORED READ ERRORA
#include "ReadError.h"
#include "Pipeline.h"

CommandLineInterpreter* CommandLineInterpreter::s_instance = nullptr;

CommandLineInterpreter* CommandLineInterpreter::Instance() {
	if (s_instance == nullptr) {
		s_instance = new CommandLineInterpreter();
	}
	return s_instance;
}

void CommandLineInterpreter::StartSystem() {
	programRunning = true;

	while (programRunning) {
		try {
			std::cout << Prompter::Instance()->getPrompt() << " " << std::flush;
			std::string input_line;
			std::getline(std::cin, input_line);
			input_line = Reader::Truncate(input_line); //skracivanje na 512 karaktera
			input_line.push_back(' '); //Za lakse savladjivanje edge case-ova

			Reader::checkGrammar(input_line); 
			Reader::ParsePipeline(input_line); //deljenje

			Pipeline::BuildPipeline();
			Pipeline::checkPipeline();
			Pipeline::ExecutePipeline();

			Pipeline::ResetPipelineStrings();
			Pipeline::ResetPipeline();

		}
		catch (ReadError* e) {
			std::cout << e->what() << std::endl;
			e->BonusInfo();

			Reader::ResetReader();
			Pipeline::ResetPipelineStrings();
			Pipeline::ResetPipeline();
			delete e;
		}
		catch (CustomError* e) {
			std::cout << e -> what() << std::endl;
			delete e;

			Reader::ResetReader();
			Pipeline::ResetPipelineStrings();  
			Pipeline::ResetPipeline();
		}
		catch(...){
			std::cout << "Unexpected exception!" << std::endl;

			Reader::ResetReader();
			Pipeline::ResetPipelineStrings();
			Pipeline::ResetPipeline();
		}
	}
	delete Prompter::Instance();
	delete CommandMaker::Instance();
}

void CommandLineInterpreter::setProgramRunning(bool a) {
	programRunning = a;
	return;
}


