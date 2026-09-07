#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Command.h"
#include "Reader.h"
#include "CommandBatch.h"

class Pipeline {
public:
	static void ResetPipelineStrings();
	static void ResetPipeline();
	static void checkPipeline();
	static void checkOrder();
	static void checkForIllegalParams();

	friend void Reader::ParsePipeline(std::string str);//deli ulaz sa terminala
	friend void Reader::ParseCommand(std::string str); // deli komandu i smesta odg. parametre 
	friend void CommandBatch::batch(); // NZM TREBA LI  "CommandBatch::"
	friend void CommandBatch::determineRedirection();

	static void BuildPipeline();
	static void ExecutePipeline();
private:
	static std::vector<std::string> pipeline_strings;
	static std::vector<std::unique_ptr<Command>> pipeline;
	static std::vector<std::string> only_at_start_commands;
	static std::vector<std::string> only_at_end_commands;
};

