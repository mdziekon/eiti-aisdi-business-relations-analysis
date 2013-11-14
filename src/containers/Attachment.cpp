#include "Containers.hpp"
#include <sstream>

Containers::Mail::Attachment::Attachment(const string& filename, const unsigned int& size):
size(size)
{
	if (filename.length() == 0)
	{
		throw Exception("Attachment cannot have empty filename");
	}
	
	auto pos = filename.find_last_of(".");
	this->name = filename.substr(0, pos);
	if (pos != string::npos)
	{
		this->extension = filename.substr(pos + 1);
	}
	else
	{
		this->extension = "";
	}
}

const string& Containers::Mail::Attachment::getName() const
{
	return this->name;
}

const string& Containers::Mail::Attachment::getExtension() const
{
	return this->extension;
}

string Containers::Mail::Attachment::getFilename() const
{
	ostringstream temp;
	temp << this->name << '.' << this->extension;
	
	return temp.str();
}

const unsigned int& Containers::Mail::Attachment::getSize() const
{
	return this->size;
}
