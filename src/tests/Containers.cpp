//============================================================================
// Containers tests
//============================================================================

#include <iostream>
#include <stdexcept>
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
	
	// ---
	// - Test #03
	// - Model - Headers
	// ---
	
	// Currectly, nothing to test
	
	// ---
	// - Test #04
	// - Model - Person
	// ---
	
	// 1. Check if method getEmail() works correctly (and proper Email is returned)
	{
		auto test = Containers::Person("Adam.Nowak@domena.pl").getEmail();
		uTest_True(test.getFull() == "Adam.Nowak@domena.pl", "#04 - #01");
	}
	
	// 2. Check if method getFirstName() works correctly
	{
		auto test1 = Containers::Person("Adam.Nowak@domena.pl");
		auto test2 = Containers::Person("adam.nowak@_domena.pl");
		auto test3 = Containers::Person("1Adam.5nowak@domena.pl");
		auto test4 = Containers::Person("_adam.!nowak@domena.pl");
		uTest_True(test1.getFirstName() == "Adam", "#04 - #02a");
		uTest_True(test2.getFirstName() == "Adam", "#04 - #02b");
		uTest_True(test3.getFirstName() == "1Adam", "#04 - #02c");
		uTest_True(test4.getFirstName() == "_adam", "#04 - #02d");
	}
	
	// 3. Check if method getLastName() works correctly
	{
		auto test1 = Containers::Person("Adam.Nowak@domena.pl");
		auto test2 = Containers::Person("adam.nowak@_domena.pl");
		auto test3 = Containers::Person("1Adam.5nowak@domena.pl");
		auto test4 = Containers::Person("_adam.!nowak@domena.pl");
		auto test5 = Containers::Person("Adam.Nowak.Kowalski@domena.pl");
		auto test6 = Containers::Person("adam.nowak.kowalski@domena.pl");
		auto test7 = Containers::Person("adam._nowak.1kowalski@domena.pl");
		auto test8 = Containers::Person("adam.1nowak._kowalski@domena.pl");
		uTest_True(test1.getLastName() == "Nowak", "#04 - #03a");
		uTest_True(test2.getLastName() == "Nowak", "#04 - #03b");
		uTest_True(test3.getLastName() == "5nowak", "#04 - #03c");
		uTest_True(test4.getLastName() == "!nowak", "#04 - #03d");
		uTest_True(test5.getLastName() == "Nowak Kowalski", "#04 - #03e");
		uTest_True(test6.getLastName() == "Nowak Kowalski", "#04 - #03f");
		uTest_True(test7.getLastName() == "_nowak 1kowalski", "#04 - #03g");
		uTest_True(test8.getLastName() == "1nowak _kowalski", "#04 - #03h");
	}
	
	// 4. Check if method getName() works correctly
	{
		auto test1 = Containers::Person("Adam.Nowak@domena.pl");
		auto test2 = Containers::Person("nowak@_domena.pl");
		auto test3 = Containers::Person("Adam.Nowak.kowalski@domena.pl");
		uTest_True(test1.getName() == "Adam Nowak", "#04 - #04a");
		uTest_True(test2.getName() == "Nowak", "#04 - #04b");
		uTest_True(test3.getName() == "Adam Nowak Kowalski", "#04 - #04c");
	}
	
	uTest_Abort("#04");
	
	cout << endl;
	
	// ---
	// - Test #05
	// - Model - Mail
	// ---
	
	// 1. Check if constructor works correctly
	{
		auto person1 = Containers::Person("test1@1domain.com");
		auto person2 = Containers::Person("test2@domain2.com");
		
		auto test = Containers::Mail(person1, person2, string("testContent"), Containers::Headers(), 123);
		uTest_True(test.sender->getEmail().getFull() == "test1@1domain.com", "#05 - #01a");
		uTest_True(test.receiver->getEmail().getFull() == "test2@domain2.com", "#05 - #01b");
		uTest_True(test.content == "testContent", "#05 - #01c");
		uTest_True(test.sendTimestamp == 123, "#05 - #01d");
	}
	
	// 2. Check if attachment adding and list reading works
	{
		auto person1 = Containers::Person("test1@1domain.com");
		auto person2 = Containers::Person("test2@domain2.com");
		
		auto test = Containers::Mail(person1, person2, string("testContent"), Containers::Headers(), 123);
		test.addAttachment(Containers::Attachment("file1.ext1", 1));
		test.addAttachment(Containers::Attachment("file2.ext2", 3));
		
		auto vect = test.getAttachments();
		uTest_True(vect.at(0).getFilename() == "file1.ext1", "#05 - #02a");
		uTest_True(vect.at(1).getFilename() == "file2.ext2", "#05 - #02b");
		bool exceptionCaught = false;
		try
		{
			vect.at(2).getFilename();
		}
		catch(out_of_range e)
		{
			exceptionCaught = true;
		}
		uTest_True(exceptionCaught, "#05 - #02c");
	}
	
	uTest_Abort("#05");
	
	cout << endl;
	
	uTest_Finish;
	
	return 0;
}
