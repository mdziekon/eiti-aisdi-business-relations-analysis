//============================================================================
// Containers tests
//============================================================================

#include <iostream>
#include "../utils/UTests.hpp"

#include "../utils/Exceptions.hpp"
#include "../models/Containers.hpp"

using namespace std;

int main()
{
	uTest_Init;
	
	// ---
	// - Test #00
	// - Model - Email
	// ---
	
	// Make Containers::Email construction public
	class EmailPublic: public Containers::Email
	{
	public:
		EmailPublic(const string& email)
		: Email(email)
		{}
	};
	
	// Constructor tests
	// 1. Check if constructor throws proper exception at zero-length of passed argument
	{
		bool exceptionCaught = false;
		try
		{
			auto test = EmailPublic("");
		}
		catch(GenericException e)
		{
			exceptionCaught = true;
		}
		uTest_True(exceptionCaught, "#00 - #01");
	}
	
	// 2. Check if constructor throws proper exception if email is incorrect (no @ sign)
	{
		bool exceptionCaught = false;
		try
		{
			auto test = EmailPublic("testMail.pl");
		}
		catch(GenericException e)
		{
			exceptionCaught = true;
		}
		uTest_True(exceptionCaught, "#00 - #02");
	}
	
	// 3. Check if constructor throws proper exception if email local part is empty
	{
		bool exceptionCaught = false;
		try
		{
			auto test = EmailPublic("@domain.com");
		}
		catch(GenericException e)
		{
			exceptionCaught = true;
		}
		uTest_True(exceptionCaught, "#00 - #03");
	}
	
	// 4. Check if constructor throws proper exception if email domain part is empty
	{
		bool exceptionCaught = false;
		try
		{
			auto test = EmailPublic("local@");
		}
		catch(GenericException e)
		{
			exceptionCaught = true;
		}
		uTest_True(exceptionCaught, "#00 - #04");
	}
	
	// 5. Check if constructor throws nothing if email is correct
	{
		bool exceptionCaught = false;
		try
		{
			auto test = EmailPublic("local@domain.com");
		}
		catch(GenericException e)
		{
			exceptionCaught = true;
		}
		uTest_False(exceptionCaught, "#00 - #05");
	}
	
	// 6. Check if method getLocal returns correct data
	{
		auto test1 = EmailPublic("local@domain.com");
		auto test2 = EmailPublic("1local@2domain.com");
		auto test3 = EmailPublic("_local@^domain.com");
		uTest_True(test1.getLocal() == "local", "#00 - #06a");
		uTest_True(test2.getLocal() == "1local", "#00 - #06b");
		uTest_True(test3.getLocal() == "_local", "#00 - #06c");
	}
	
	// 7. Check if method getDomain returns correct data
	{
		auto test1 = EmailPublic("local@domain.com");
		auto test2 = EmailPublic("1local@2domain.com");
		auto test3 = EmailPublic("_local@^domain.com");
		uTest_True(test1.getDomain() == "domain.com", "#00 - #07a");
		uTest_True(test2.getDomain() == "2domain.com", "#00 - #07b");
		uTest_True(test3.getDomain() == "^domain.com", "#00 - #07c");
	}
	
	// 8. Check if method getFull returns correct data
	{
		auto test1 = EmailPublic("local@domain.com");
		auto test2 = EmailPublic("1local@2domain.com");
		auto test3 = EmailPublic("_local@^domain.com");
		uTest_True(test1.getFull() == "local@domain.com", "#00 - #08a");
		uTest_True(test2.getFull() == "1local@2domain.com", "#00 - #08b");
		uTest_True(test3.getFull() == "_local@^domain.com", "#00 - #08c");
	}
	
	uTest_Abort("#00");
	
	cout << endl;
	
	return 0;
}
