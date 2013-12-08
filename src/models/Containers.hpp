/* 
 * File:   Containers.hpp
 * Author: michal
 */

#ifndef CONTAINERSNS_HPP
#define	CONTAINERSNS_HPP

#include <string>
#include <vector>

using namespace std;

namespace Containers
{	
	class Email
	{
		friend class Person;
		
		protected:
			string local, domain;

			Email(const string& email);
			
		public:			
			const string& getLocal() const;
			const string& getDomain() const;
			string getFull() const;
	};
	
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
	
	class Person
	{
	protected:
		string firstName, lastName;
		Email email;
		
	public:
		Person(const string& email);

		const string& getFirstName() const;
		const string& getLastName() const;
		string getName() const;
		const Email& getEmail() const;
	};
	
	class Mail
	{
	protected:
		vector<Attachment> attachments;

	public:
		Person * const sender, * const receiver;
		const string content;
		const Headers headers;
		const int sendTimestamp;

		Mail(Person& sender, Person& receiver, const string& content, const Headers& headers, const int& sendTimestap);

		void addAttachment(const Attachment& attachment);
		const vector<Attachment>& getAttachments() const;
	};	
}

#endif	/* CONTAINERSNS_HPP */
