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
	// - Test #01
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
		uTest_True(exceptionCaught, "#01 - #01");
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
		uTest_True(exceptionCaught, "#01 - #02");
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
		uTest_True(exceptionCaught, "#01 - #03");
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
		uTest_True(exceptionCaught, "#01 - #04");
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
		uTest_False(exceptionCaught, "#01 - #05");
	}
	
	// 6. Check if method getLocal returns correct data
	{
		auto test1 = EmailPublic("local@domain.com");
		auto test2 = EmailPublic("1local@2domain.com");
		auto test3 = EmailPublic("_local@^domain.com");
		uTest_True(test1.getLocal() == "local", "#01 - #06a");
		uTest_True(test2.getLocal() == "1local", "#01 - #06b");
		uTest_True(test3.getLocal() == "_local", "#01 - #06c");
	}
	
	// 7. Check if method getDomain returns correct data
	{
		auto test1 = EmailPublic("local@domain.com");
		auto test2 = EmailPublic("1local@2domain.com");
		auto test3 = EmailPublic("_local@^domain.com");
		uTest_True(test1.getDomain() == "domain.com", "#01 - #07a");
		uTest_True(test2.getDomain() == "2domain.com", "#01 - #07b");
		uTest_True(test3.getDomain() == "^domain.com", "#01 - #07c");
	}
	
	// 8. Check if method getFull returns correct data
	{
		auto test1 = EmailPublic("local@domain.com");
		auto test2 = EmailPublic("1local@2domain.com");
		auto test3 = EmailPublic("_local@^domain.com");
		uTest_True(test1.getFull() == "local@domain.com", "#01 - #08a");
		uTest_True(test2.getFull() == "1local@2domain.com", "#01 - #08b");
		uTest_True(test3.getFull() == "_local@^domain.com", "#01 - #08c");
	}
	
	uTest_Abort("#01");
	
	cout << endl;
	
	// ---
	// - Test #02
	// - Model - Attachment
	// ---
	
	// 1. Check if constructor throws proper exception at zero-length of passed filename
	{
		bool exceptionCaught = false;
		try
		{
			auto test = Containers::Attachment("", 1);
		}
		catch(GenericException e)
		{
			exceptionCaught = true;
		}
		uTest_True(exceptionCaught, "#02 - #01");
	}
	
	// 2. Check if method getFilename() works correctly
	{
		auto test1 = Containers::Attachment("filename.ext", 1);
		auto test2 = Containers::Attachment("4filename._ext", 2);
		auto test3 = Containers::Attachment("!filename.8ext", 3);
		uTest_True(test1.getFilename() == "filename.ext", "#02 - #02a");
		uTest_True(test2.getFilename() == "4filename._ext", "#02 - #02b");
		uTest_True(test3.getFilename() == "!filename.8ext", "#02 - #02c");
	}
	
	// 3. Check if method getName() works correctly
	{
		auto test1 = Containers::Attachment("filename.ext", 1);
		auto test2 = Containers::Attachment("4filename._ext", 2);
		auto test3 = Containers::Attachment("!filename.8ext", 3);
		uTest_True(test1.getName() == "filename", "#02 - #03a");
		uTest_True(test2.getName() == "4filename", "#02 - #03b");
		uTest_True(test3.getName() == "!filename", "#02 - #03c");
	}
	
	// 4. Check if method getExtension() works correctly
	{
		auto test1 = Containers::Attachment("filename.ext", 1);
		auto test2 = Containers::Attachment("4filename._ext", 2);
		auto test3 = Containers::Attachment("!filename.8ext", 3);
		uTest_True(test1.getExtension() == "ext", "#02 - #04a");
		uTest_True(test2.getExtension() == "_ext", "#02 - #04b");
		uTest_True(test3.getExtension() == "8ext", "#02 - #04c");
	}
	
	// 5. Check if method getSize() works correctly
	{
		auto test1 = Containers::Attachment("filename.ext", 10);
		auto test2 = Containers::Attachment("4filename._ext", 0);
		uTest_True(test1.getSize() == 10, "#02 - #05a");
		uTest_True(test2.getSize() == 0, "#02 - #05b");
	}
	
	// 6. Check if extension is empty if filename doesn't contain "." (dot)
	{
		auto test = Containers::Attachment("filename", 1);
		uTest_True(test.getExtension() == "", "#02 - #06");
	}
	
	uTest_Abort("#02");
	
	cout << endl;
	
	return 0;
}
