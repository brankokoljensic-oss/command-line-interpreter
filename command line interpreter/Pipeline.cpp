#include "Pipeline.h"
#include "Command.h" //MISLIM DA MOZE I BEZ OVOGA
#include "CommandMaker.h"
#include "CustomError.h"
#include "PipelineOrderError.h"


std::vector<std::unique_ptr<Command>> Pipeline::pipeline;
std::vector<std::string>Pipeline::pipeline_strings;
std::vector<std::string>Pipeline::only_at_start_commands = {"time", "date", "exit"};
std::vector<std::string>Pipeline::only_at_end_commands = {"prompt", "touch", "rm", "truncate", "exit"};

bool checkIfInVector (std::string str , std::vector<std::string> v){
	return std::find(v.begin(), v.end(), str) != v.end(); //vraca true ako jeste u vektoru, false ako suprotno
}

void Pipeline::ResetPipelineStrings() {
	pipeline_strings.clear();
}

void Pipeline::ResetPipeline() {
	pipeline.clear();
}

void Pipeline::checkPipeline(){ 
	for (size_t i = 0; i < pipeline.size(); i++) {
		pipeline[i]->checkFormat();
	}
	if (pipeline.size() > 1) {
		Pipeline::checkOrder();
		Pipeline::checkForIllegalParams();
	}
	return;
}

void Pipeline::checkOrder() {
	for (size_t i = 0; i < pipeline.size(); i++) {
		if (i == 0 && (checkIfInVector (pipeline[i]->getName(), only_at_end_commands))) {
			throw new PipelineOrderError(pipeline[i]->getName());
		}
		else if (i > 0 && (i < pipeline.size() - 1) &&  (checkIfInVector(pipeline[i]->getName(), only_at_end_commands) || checkIfInVector(pipeline[i]->getName(), only_at_start_commands))) {
			throw new PipelineOrderError(pipeline[i]->getName());
		}
		else if ((i == pipeline.size() - 1 ) && (checkIfInVector (pipeline[i]->getName(), only_at_start_commands))) {
			throw new PipelineOrderError(pipeline[i]->getName());
		}
	}
	return;
}
void Pipeline::checkForIllegalParams() {
	for (size_t i = 0; i < pipeline.size(); i++) {
		if (i == 0 && (!(pipeline[i]->checkOutputRedirectionNew() && pipeline[i]->checkOutputRedirectionAppend()))) {
			throw new CustomError("Error: pipeline can only have it's output flow redirected at the end");
		}
		else if (i > 0 && (i < pipeline.size() - 1)) {
			if (!(pipeline[i]->checkOutputRedirectionNew() && pipeline[i]->checkOutputRedirectionAppend() && pipeline[i]->checkInputRedirection())) {
				throw new CustomError("Error: pipeline can only have it's input source redirected at the start or output flow redirected at the end");
			}
			else if (!pipeline[i]->checkFirstArgument()) {
				throw new CustomError("Error: argument can be set only at the start of the pipeline");
			}
		}
		else if ((i == pipeline.size() - 1)) {
			if (!pipeline[i]->checkInputRedirection()) {
				throw new CustomError("Error: pipeline can only have it's input source redirected at the start");
			}
			else if (!pipeline[i]->checkFirstArgument()) {
				throw new CustomError("Error: argument can be set only at the start of the pipeline");
			}
		}
	}
}

void Pipeline::BuildPipeline() {

	std::string name = "";
	std::vector <std::string> options;
	std::vector <std::string> arguments;
	std::string input_redirection = "";
	std::string output_redirection_new = "";
	std::string output_redirection_append = "";
	std::vector <DataType> types_of_params;

	for (size_t i = 0; i < pipeline_strings.size(); i++) {
		pipeline_strings[i].push_back(' '); //za lakse savladjivanje edgecase-ova
		Reader::ParseCommand(pipeline_strings[i]); //deljenje

		name = Reader::getCommandName();
		options = Reader::getCommandOpt();
		arguments = Reader::getCommandArguments();
		input_redirection = Reader::getCommandInputRedirection();
		output_redirection_new = Reader::getCommandOutputRedirectionNew();
		output_redirection_append = Reader::getCommandOutputRedirectionAppend();
		types_of_params = Reader::getCommandTypesOfParams();

		Reader::ResetReader();

		std::unique_ptr<Command> com = CommandMaker::Instance()->makeCommand(name, options, arguments, input_redirection, 
																			output_redirection_new, output_redirection_append, types_of_params);
		pipeline.push_back(std::move(com));
	}
}

void Pipeline::ExecutePipeline() { 
	std::string argument_pipe = "";
	for (size_t i = 0; i < pipeline.size(); i++) {
		pipeline[i]->checkParams();
		std::string argument_pipe = pipeline[i]->executeCommand();

		if (i == pipeline.size() - 1) {
			pipeline[i]->printResult(argument_pipe);
		}
		else {
			if (pipeline[i + 1]->getName() == "touch" || pipeline[i + 1]->getName() == "rm" || pipeline[i + 1]->getName() == "batch"|| pipeline[i + 1]->getName() == "truncate") {
				pipeline[i + 1]->setFirstArgument(argument_pipe);
			}
			else {
				pipeline[i + 1]->setFirstArgument("\"" + argument_pipe + "\"");
			}
		}
	}
}
