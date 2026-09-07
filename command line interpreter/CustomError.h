#pragma once
#include <exception>
#include <string>

class CustomError : public std::exception {
public:
	CustomError(std::string message);

	char const* what() const override;

private:
	std::string m_message;
};


