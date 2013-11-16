#include "Exceptions.hpp"

GenericException::GenericException(const char* text):
text(text)
{}

const char* GenericException::what() const noexcept
{
	return this->text.c_str();
}
