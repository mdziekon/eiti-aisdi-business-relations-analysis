#include <stdexcept>

#include "Containers.hpp"

bool Containers::Headers::addHeader(const string& key, const string& value)
{
	return this->headers.insert(pair<string, string>(key, value)).second;
}

const string Containers::Headers::getHeader(const string& key) const
{
	try
	{
		return this->headers.at(key);
	}
	catch(out_of_range e)
	{
		return "";
	}
}

const unordered_map<string, string>& Containers::Headers::getAllHeaders() const
{
	return this->headers;
}

bool Containers::Headers::removeHeader(const string& key)
{
	return this->headers.erase(key);
}