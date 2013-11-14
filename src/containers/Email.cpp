#include "Containers.hpp"
#include <sstream>

Containers::Person::Email::Email(const string& email)
{
	if (email.length() == 0)
	{
		throw Exception("Email cannot be empty");
	}
	auto posAt = email.find("@");
	if (posAt == string::npos)
	{
		throw Exception("Email is incorrect ");
	}
	this->local = email.substr(0, posAt);
	this->domain = email.substr(posAt + 1);
	if (this->local.length() == 0)
	{
		throw Exception("Email's local part cannot is empty");
	}
	if (this->domain.length() == 0)
	{
		throw Exception("Email's domain part cannot be empty");
	}
}

const string& Containers::Person::Email::getLocal() const
{
	return this->local;
}

const string& Containers::Person::Email::getDomain() const
{
	return this->domain;
}


string Containers::Person::Email::getFull() const
{
	ostringstream email;
	email << this->local << '@' << this->domain;
	
	return email.str();
}

