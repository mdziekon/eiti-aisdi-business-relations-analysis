#ifndef CONTAINERS_HPP
#define	CONTAINERS_HPP

#include <string>
#include <vector>

using namespace std;

class Containers
{
protected:
    class Exception: public exception
    {
	protected:
		const string text;

	public:
		Exception(const char* text);
		virtual const char* what() const noexcept;
    };
    
public: 
    class Person;
    class Mail;
    
    class Person
    {	
	protected:
		class Email
		{
		protected:
			string local, domain;

		public:
			Email(const string& email);

			const string& getLocal() const;
			const string& getDomain() const;
			string getFull() const;
		};

		string firstName, lastName;
		Email email;
		vector<Mail> sentMails;

	public:
		Person(const string& email);

		const string& getFirstName() const;
		const string& getLastName() const;
		string getName() const;
		const Email& getEmail() const;
    };
    
    class Mail
    {
	public:
		class Attachment;
		
	protected:
		vector<Attachment> attachments;
		
    public:
		class Attachment
		{
		protected:
			string name, extension;
			const unsigned int size;

		public:
			Attachment(const string& filename, const unsigned int& size);

			const string& getName() const;
			const string& getExtension() const;
			string getFilename() const;
			const unsigned int& getSize() const;
		};
		
		class Headers
		{
			
		public:
			bool addHeader(const string& key, const string& value);
		};
		
		const Person& sender, receiver;
		const string content;
		const Headers headers;
		const int sendTimestamp;
		
		Mail(const Person& sender, const Person& receiver, const string& content, const Headers& headers, const int& sendTimestap);
		
		void addAttachment(const Attachment& attachment);
		const vector<Attachment>& getAttachments() const;
    };
};

#endif	/* CONTAINERS_HPP */

