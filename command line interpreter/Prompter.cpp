#include "Prompter.h"

Prompter* Prompter::s_instance = nullptr;

Prompter::Prompter() : prompt("$") {}

Prompter* Prompter::Instance() {
    if (!s_instance) {
        s_instance = new Prompter(); 
    }
    return s_instance; 
}

std::string Prompter::getPrompt() {
	return this->prompt;
}

void Prompter::setPrompt(std::string prompt) {
	this->prompt = prompt;
	return;
}
