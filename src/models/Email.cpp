#include "Containers.hpp"
#include "../utils/Exceptions.hpp"
#include <sstream>

Containers::Email::Email(const string& email)
{
	if (email.length() == 0)
	{
		throw GenericException("Email cannot be empty");
	}
	auto posAt = email.find("@");
	if (posAt == string::npos)
	{
		throw GenericException("Email is incorrect ");
	}
	this->local = email.substr(0, posAt);
	this->domain = email.substr(posAt + 1);
	if (this->local.length() == 0)
	{
		throw GenericException("Email's local part cannot is empty");
	}
	if (this->domain.length() == 0)
	{
		throw GenericException("Email's domain part cannot be empty");
	}
}

const string& Containers::Email::getLocal() const
{
	return this->local;
}

const string& Containers::Email::getDomain() const
{
	return this->domain;
}


string Containers::Email::getFull() const
{
	ostringstream email;
	email << this->local << '@' << this->domain;
	
	return email.str();
}

