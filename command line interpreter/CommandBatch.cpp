#include "CommandBatch.h"
#include <fstream>
#include "MissingFileError.h"
#include "InvalidOptionError.h"
#include "InvalidInputRedirectionError.h"
#include "MissingArgumentError.h"
#include "BadFormatError.h"
#include "Reader.h"
#include "Pipeline.h"
#include "ReadError.h"
#include "CustomError.h"

CommandBatch::CommandBatch(std::string name, std::vector <std::string> options, std::vector<std::string> arguments,
	std::string input_redirection, std::string output_redirection_new, std::string output_redirection_append, std::vector <DataType> types_of_params)
	: Command(name, options, arguments, input_redirection, output_redirection_new, output_redirection_append, types_of_params)
{}

void CommandBatch::checkFormat() {
	std::vector <DataType> types = getTypesOfParams();
	bool option_seen = false;
	bool argument_seen = false;
	bool input_red_seen = false;
	bool output_red_seen = false;
	for (size_t i = 1; i < types.size(); i++) {
		if (types[i] == DataType::argument) {
			if (input_red_seen || output_red_seen || argument_seen) {
				throw new BadFormatError(getName());
			}
			argument_seen = true;
		}
		else if (types[i] == DataType::input_redirection) {
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

void CommandBatch::checkParams() {
	if (!getOpt().empty()) {
		throw new InvalidOptionError(getName(), getOpt()[0]);
	}
	if (!checkFirstArgument() && !checkInputRedirection()) {
		throw new InvalidInputRedirectionError(getName());
	}
	else if (checkFirstArgument() && checkInputRedirection()) {
		throw new MissingArgumentError(getName());
	}
	else if (!checkFirstArgument()) {
		Nature nature = checkNature(getFirstArgument());
		if (nature != is_file) {
			throw new CustomError("Error: argument must be a file");
		}
	}

	return;
}

std::string CommandBatch::executeCommand() {
	batch();
	return "Batch done.";
}

void CommandBatch::printResult(std::string result){
	return;
}

void CommandBatch::determineRedirection() {
	std::string last_commands_redirection_new = Pipeline::pipeline.back() -> getOutputRedirectionNew();
	std::string last_commands_redirection_append = Pipeline::pipeline.back() -> getOutputRedirectionAppend();
	if (last_commands_redirection_new.empty() && last_commands_redirection_append.empty()) {
		if (!this->getOutputRedirectionNew().empty()) {
			Pipeline::pipeline.back()->setOutputRedirectionNew(this->getOutputRedirectionNew());
		}
		else if (!this->getOutputRedirectionAppend().empty()) {
			Pipeline::pipeline.back()->setOutputRedirectionAppend(this->getOutputRedirectionAppend());
		}
	}
	return;
}

void CommandBatch::batch() {
	std::vector <std::unique_ptr<Command>> save_pipeline = std::move(Pipeline::pipeline);
	std::vector <std::string> save_pipeline_strings = Pipeline::pipeline_strings;

	Pipeline::ResetPipelineStrings();
	Pipeline::ResetPipeline();

	std::string input_line;
	std::string filename;

	if (checkInputRedirection()) {
		filename = getFirstArgument();
	}
	else {
		filename = stripInRed();
	}

	std::ifstream file(filename);

	if (!file) {
		throw new MissingFileError(filename);
	}
	else {
		setLoopFlag(true);
		while (getLoopFlag()) { 
			try {
				if (std::getline(file, input_line)) {
					input_line = Reader::Truncate(input_line); //skracivanje na 512 karaktera
					input_line.push_back(' '); //Za lakse savladjivanje edge case-ova, NZM DA LI MI OVO TREBA OVDE

					Reader::checkGrammar(input_line);
					Reader::ParsePipeline(input_line); //deljenje

					Pipeline::BuildPipeline();
					Pipeline::checkPipeline();
					determineRedirection();
					Pipeline::ExecutePipeline();

					Pipeline::ResetPipelineStrings();
					Pipeline::ResetPipeline();
				}
				else if (file.eof()) { //na prethodnom citanju jesam li dobio EOF
					file.clear();
					setLoopFlag(false);
				}
				else {
					file.clear();
					throw;  //za svaki slucaj, ako negde pukne
				}

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
				std::cout << e->what() << std::endl;
				delete e;

				Reader::ResetReader();
				Pipeline::ResetPipelineStrings();  //NE ZNAM JOS JESTE LI OVO OVDE PREKO POTREBNO
				Pipeline::ResetPipeline();
			}
			catch (...) {
				std::cout << "Unexpected exception!" << std::endl;

				Reader::ResetReader();
				Pipeline::ResetPipelineStrings();
				Pipeline::ResetPipeline();
			}

		}
	}
	Pipeline::pipeline = std::move(save_pipeline);
	Pipeline::pipeline_strings = save_pipeline_strings;
}
