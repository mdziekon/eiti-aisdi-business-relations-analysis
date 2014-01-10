/* 
 * File:   Containers.hpp
 * Author: michal
 */

#ifndef CONTAINERSNS_HPP
#define	CONTAINERSNS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ctime>

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
	protected:
		unordered_map<string, string> headers;
		
	public:
		bool addHeader(const string& key, const string& value);
		const string getHeader(const string& key) const;
		const unordered_map<string, string>& getAllHeaders() const;
		bool removeHeader(const string& key);
	};
	
	class Date
	{
	protected:
		tm timeStruct;
		
	public:
		Date();
		Date(tm timeStruct);
		
		std::string formatDate(const char* format) const;
		unsigned int getUnixTimestamp() const;
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
	
	enum Receiver
	{
		Normal, Copy, CarbonCopy, Reply
	};
	
	class Mail
	{
	protected:
		vector<Attachment> attachments;
		

	public:
		Person * const sender;
		const vector<pair<Person*, Receiver>> receivers;
                
        vector<Person *> forwarded_to;
        bool complete_cycle;    // cykl pełny
        bool part_of_a_cycle;   // cykl składowy
		
		const string content;
		const Headers headers;
		const Date sendDate;
        string md5hash;

		Mail(Person& sender, vector<pair<Person*, Receiver>>& receivers, const string& content, const Headers& headers, const Date& sendDate);

		void addAttachment(const Attachment& attachment);
		const vector<Attachment>& getAttachments() const;
	};	
}

#endif	/* CONTAINERSNS_HPP */
