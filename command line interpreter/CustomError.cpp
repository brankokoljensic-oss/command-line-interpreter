#include "CustomError.h"

CustomError::CustomError(std::string message)
	: m_message(message)
{}

char const* CustomError::what() const {
	return m_message.c_str();
}
