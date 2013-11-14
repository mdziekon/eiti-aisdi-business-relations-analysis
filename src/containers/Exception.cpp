#include "Containers.hpp"

Containers::Exception::Exception(const char* text):
text(text)
{}

const char* Containers::Exception::what() const noexcept
{
	return this->text.c_str();
}
