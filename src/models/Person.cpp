#include "Containers.hpp"
#include <algorithm>
#include <sstream>

Containers::Person::Person(const string& email):
email(email)
{
	auto posDot = this->email.getLocal().find_first_of(".");
	string::iterator firstLetterCapitalize;
	if (posDot == string::npos)
	{
		this->firstName = string("");
		this->lastName = this->email.getLocal();
	}
	else
	{
		this->firstName = this->email.getLocal().substr(0, posDot);
		this->lastName = this->email.getLocal().substr(posDot + 1);
		
		firstLetterCapitalize = this->firstName.begin();
		if (*firstLetterCapitalize >= 'a' && *firstLetterCapitalize <= 'z')
		{
			*firstLetterCapitalize -= 32;
		}
	}
	replace(this->lastName.begin(), this->lastName.end(), '.', ' ');
	
	firstLetterCapitalize = this->lastName.begin();
	if (*firstLetterCapitalize >= 'a' && *firstLetterCapitalize <= 'z')
	{
		*firstLetterCapitalize -= 32;
	}
	
	for (auto iter = this->lastName.begin() + 1, nextIter = this->lastName.end(); iter != this->lastName.end(); ++iter)
	{
		if (*iter == ' ')
		{
			nextIter = iter + 1;
			if (nextIter != this->lastName.end() && *nextIter >= 'a' && *nextIter <= 'z')
			{
				*nextIter -= 32;
			}
		}
	}
}

const string& Containers::Person::getFirstName() const
{
	return this->firstName;
}

const string& Containers::Person::getLastName() const
{
	return this->lastName;
}

string Containers::Person::getName() const
{
	ostringstream name;
	if (this->firstName.length() > 0)
	{
		name << this->firstName << ' ';
	}
	name << this->lastName;
	
	return name.str();
}

const Containers::Email& Containers::Person::getEmail() const
{
	return this->email;
}
