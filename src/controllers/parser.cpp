#include "parser.hpp"
#include "../utils/Exceptions.hpp"
#include "../utils/MD5.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <vector>

bool FileParser::CzyKatalog(const std::string & path)
{
	bool is_dir = false;
	DIR *dir = opendir(path.c_str());
	is_dir = static_cast<bool>(dir);
	closedir(dir);
	return is_dir;
}

std::vector<Containers::Mail*> FileParser::load(const std::string path)
{
	std::vector<Containers::Mail*> vec;
	
	if (this->CzyKatalog(path) == true)
	{
		DIR* p_dir = opendir(path.c_str());

		struct dirent* p_file;

		while ((p_file = readdir(p_dir)) != NULL)
		{
			vec.push_back(this->load_plik(p_file->d_name));
		}
	}
	else
	{
		vec.push_back(this->load_plik(path.c_str()));
	}
	return vec;
}

Containers::Mail* FileParser::load_plik(const char* path)
{
	std::string Str;
	FILE *File = fopen(path, "r");

	if (!File) {
		throw(GenericException("Nie odnaleziono pliku"));
	}

	while (!feof(File)) {
		Str += fgetc(File);
	}
	return this->build(Str);
}

using namespace Containers;

Date FileParser::parseTime(const std::string & input)
{
	struct tm time_struct;
	time_t rawtime;

	std::string::const_iterator it = input.begin(); //spacja + dzień + spacja
	std::string weekday, day, month, year, hours, minutes, seconds;
	std::string days[7] {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	std::string months[12] {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
				"Aug", "Sep", "Oct", "Nov", "Dec"};
	
	time(&rawtime);
	time_struct = *localtime(&rawtime);
	
	/* reguły: spacja (pomijana), 3-literowa
	nazwa dnia (pomijana), spacja, dzień z , spacja, miesiąc słownie,
	spacja, rok, spacja, godzina oddzielana :, spacja, strefa czasowa */
	
	/* zachowanie dnia tygodnia */
	weekday.reserve(3);
	for(int i = 0; i < 3; ++i) {
		weekday += *it++;
	}
	it += 2;
	/* zachowanie dnia */
	day.reserve(2);
	day += *it++;
	day += *it++;
	it++;
	/* zachowanie miesiąca */
	month.reserve(3);
	month += *it++;
	month += *it++;
	month += *it++;
	it++;
	/* zachowanie roku */
	year.reserve(4);
	for(int i = 0; i < 4; ++i) {
		year += *it++;
	}
	it++;
	/* zachowanie godziny */
	hours.reserve(2);
	hours += *it++;
	hours += *it++;
	it++;
	/* zachowanie minut */
	minutes.reserve(2);
	minutes += *it++;
	minutes += *it++;
	it++;
	/* zachowanie sekund */
	seconds.reserve(2);
	seconds += *it++;
	seconds += *it++;
	
	/* zamiana dnia tygodnia na liczbę */
	for(int i = 0; i < 7; ++i) {
		if(days[i] == weekday) {
//			cout << "+++Setting weekday: " << i << endl;
			time_struct.tm_wday = i;
			break;
		}
	}
	
	/* zamiana miesiąca na liczbę */
	for(int i = 0; i < 12; ++i) {
		if(months[i] == month) {
//			cout << "+++Setting month: " << i << endl;
			time_struct.tm_mon = i;
			break;
		}
	}
	/* zamiana od roku do sekund na liczby */
	time_struct.tm_year = atoi(year.c_str()) - 1900;
	time_struct.tm_mday = atoi(day.c_str());
//	cout << "+++Setting day: " << time_struct.tm_mday << endl;
	time_struct.tm_hour = atoi(hours.c_str());
	time_struct.tm_min = atoi(minutes.c_str());
	time_struct.tm_sec = atoi(seconds.c_str());
	time_struct.tm_isdst = 0;

//	cout << "Form: " << time_struct.tm_year << "." << time_struct.tm_mon << "." << time_struct.tm_mday << "." << time_struct.tm_wday << "." << time_struct.tm_hour << "." << time_struct.tm_min << "." << time_struct.tm_sec << "." << "." << time_struct.tm_isdst << endl;
	return Date(time_struct);
}
	

std::string FileParser::parseEmail(const std::string & input)
{
	return input.substr(input.find_last_of('<') + 1, input.find_last_of('>') - 1);
}

vector<pair<Person*, Receiver>> FileParser::parseMultiple(
        const std::string & input, Receiver type)
{
    std::basic_string<char>::const_iterator it = input.begin();
    vector<pair<Person*, Receiver>> result;
    Person * current_person;
    std::string mail_address;

//	std::cout << "Emails list:\n" << input << endl;
	
    do {
        while(*it <= ' ' || *it == ',') {
            it++;
        }
        // remember begin point of analyzed block
        std::basic_string<char>::const_iterator begin_it = it;
        // has there been an email in <>? if so, this'll be true'd
        bool foundEmail = false;
        
        // keep iterating until the end of block (last block will be ended
        // with a '\r', others will end with a comma)
        do {
            // look for nested address
            if(*it == '<') {
                std::basic_string<char>::const_iterator begin_internal = it;
                while (*it != '>') {
                    it++;
                }
                mail_address = std::string(begin_internal + 1, it - 1);
                if(cache.find(mail_address) == cache.end()) {
//					std::cout << "Email: " << mail_address << endl;
                        current_person = new Person(mail_address);
                } else {
					current_person = cache.find(mail_address)->second;
				}
                foundEmail = true;
                // this'll iterate until the end of block
            }
            it++;
        } while (*it != ',' && *it >= ' ');
        // no email in <>? chunk the whole block into the vector
        if(!foundEmail) {
            //nie ma zagnieżdżonego adresu
            mail_address = std::string(begin_it, it);
			
            if(cache.find(mail_address) == cache.end()) {
//				std::cout << "Email: " << mail_address << endl;
                current_person = new Person(mail_address);
            } else {
					current_person = cache.find(mail_address)->second;
				}
        }
        result.push_back(std::pair<Person*, Receiver>(current_person, type));
        cache.insert(std::pair<std::string, Person*>(mail_address, current_person));
    } while (*it == ',' && *it > ' ');
    return result;
}

void FileParser::checkForwards(vector<Containers::Mail*>& mails)
{
	for(auto x: mails)
	{
		if (x->headers.getHeader("X-Forwarded-From").size() == 0)
		{
			continue;
		}
	}
}

Person * FileParser::addPerson(const std::string & email)
{
	Person * temporaryPerson;
	std::pair<std::unordered_map<std::string, Person *>::iterator, bool> result;

	result = cache.insert(std::pair<std::string, Person *>(email, NULL));

	if (result.second) {
		temporaryPerson =  new Person(email);
		cache.at(email) = temporaryPerson;
	}
	else {
		temporaryPerson = cache.at(email);
	}
	
	return temporaryPerson;
}


std::unordered_map<std::string, Person*> FileParser::getCache()
{
	return cache;
}	

int FileParser::cacheSize()
{
	return cache.size();
}

void FileParser::loadFwds(vector<Containers::Mail*>& mails) {

	for(auto x: mails)
	{
		cout << "a";
		if (!x->partOfForwardPath)
		{
			cout << "Not in forward---\n";
			continue;
		}
		if (x->headers.getHeader("X-Forwarded-Hash") == "")
		{
			cout << "Not forwarded---\n";
			continue;
		}
		
		auto it = this->forwards_hashes.find(x->headers.getHeader("X-Forwarded-Hash"));
		if (it == this->forwards_hashes.end())
		{
			continue;
		}
		Containers::Mail* node = it->second;
		if (node == NULL)
		{
			continue;
		}
		
		if (node->headers.getHeader("X-Forwarded-Hash") == "")
		{
			x->forwardBase = node;
			node->forwardAll.push_back(x);
		}
		else
		{
			x->forwardBase = node->forwardBase;
			node->forwardBase->forwardAll.push_back(x);
		}
	}
}


Containers::Mail * FileParser::build(std::string& str)
{
	this->parser_foundHeadersEnd = false;
	auto it = str.begin();
	Person * sender = NULL;
	vector<pair<Person*, Receiver>> receivers;
	Mail * result_mail = NULL;

	Headers headers;
	std::string title;
	std::string contents;
	Date time;
	unsigned int forwardHashDetectionState = 0;
	unsigned int hashBuilderState = 0;
	std::stringstream strStream;
	
	bool setAsPartOfForward = false;
	bool isBaseForward = true;
	
	cout << "=== BUILD\n";
	while(it != str.end())
	{		
		if (hashBuilderState == 2)
		{
//			cout << "Build hash stamp: " << time.getUnixTimestamp() << " (" << time.formatDate("%c") << ")" << endl;
			strStream.str("");
			strStream << sender->getEmail().getFull();
			strStream << "|" ;
			strStream << time.getUnixTimestamp();
			headers.addHeader("Fwd-Hash", strStream.str());
//			cout << headers.getHeader("Fwd-Hash") << endl;
			hashBuilderState = 0;
		}
		
		if (forwardHashDetectionState == 2)
		{
			isBaseForward = false;
			
			strStream.str("");
			strStream << headers.getHeader("X-Forwarded-From");
			strStream << "|" ;
			strStream << headers.getHeader("X-Forwarded-Original-Date");
			headers.addHeader("X-Forwarded-Hash", strStream.str());
			
			cout << ">>> Found HASH: " << strStream.str() << endl;
			auto fwdIt = this->forwards_hashes.find(strStream.str());
			if (fwdIt == this->forwards_hashes.end())
			{
				cout << "no match\n";
				this->forwards_hashes.insert({{strStream.str(), NULL}});
			}
			else if ((*fwdIt).second != NULL)
			{
				cout << "match\n";
				(*fwdIt).second->partOfForwardPath = true;
			}
			
			setAsPartOfForward = true;
			
//			cout << headers.getHeader("Fwd-Hash") << endl;
			forwardHashDetectionState = 0;
		}
		
//		cout << "^^^^^^^^^^^^\n";
//		cout << "Entity found\n";
		auto result = this->parseEntity(str, it);
//		cout << result.first << endl;
//		cout << "---" << endl;
//		cout << result.second << endl;
//		cout << "============\n";
		
		if(result.first == "X-Forwarded-Original-Date")
		{
//			cout << "Found XDATE: " << result.second << endl;
			strStream.str("");
			strStream << this->parseTime(result.second).getUnixTimestamp();
//			cout << "Date stamp: " << strStream.str() << " (" << this->parseTime(result.second).formatDate("%c") << ")" << endl;
			headers.addHeader(result.first, strStream.str());
			++forwardHashDetectionState;
		}
		else if(result.first == "Date")
		{
//			cout << "Found date: " << result.second << endl;
			time = this->parseTime(result.second);
//			cout << "Date stamp: " << time.getUnixTimestamp() << " (" << time.formatDate("%c") << ")" << endl;
			++hashBuilderState;
		}
		else if(result.first == "From")
		{
		    sender = this->addPerson(this->parseEmail(result.second));
			++hashBuilderState;
		} 
		else if (result.first == "To")
		{
			std::vector<std::pair<Person*, Receiver>> temp = 
				this->parseMultiple(result.second, Normal);
			receivers.insert(receivers.end(), temp.begin(), temp.end());
		} 
		else if (result.first == "Content")
		{
		    contents = result.second;
			result_mail = new Mail(*sender, receivers, contents, headers, time);
		} 
		else if (result.first == "Cc")
		{
			std::vector<std::pair<Person*, Receiver>> temp = 
				this->parseMultiple(result.second, Copy);
			receivers.insert(receivers.end(), temp.begin(), temp.end());
		}
		else if (result.first == "Bcc")
		{
			std::vector<std::pair<Person*, Receiver>> temp = 
				this->parseMultiple(result.second, CarbonCopy);
			receivers.insert(receivers.end(), temp.begin(), temp.end());
		}
		else if (result.first == "Reply-To")
		{
			std::vector<std::pair<Person*, Receiver>> temp = 
				this->parseMultiple(result.second, Reply);
			receivers.insert(receivers.end(), temp.begin(), temp.end());
		}
		else if(result.first == "Subject")
		{
			headers.addHeader(result.first, result.second);
			title = result.second;   
		}
		else
		{
			if (result.first == "X-Forwarded-From")
			{
				++forwardHashDetectionState;
			}
			
			headers.addHeader(result.first, result.second);
		} 
		
	}
	
	cout << "--- For mail: " << result_mail->headers.getHeader("Subject") << endl;
	cout << "Hash: " << headers.getHeader("Fwd-Hash") << endl;
	auto fwdIt = this->forwards_hashes.find(headers.getHeader("Fwd-Hash"));
	if (fwdIt == this->forwards_hashes.end())
	{
//		cout << "Not found\n";
		this->forwards_hashes.insert({{headers.getHeader("Fwd-Hash"), result_mail}});
	}
	else if ((*fwdIt).second == NULL)
	{
//		cout << "Found and null\n";
		(*fwdIt).second = result_mail;
		result_mail->partOfForwardPath = true;
	}
	if (setAsPartOfForward)
	{
		result_mail->partOfForwardPath = true;
	}
	
//	if (isBaseForward)
//	{
//		this->forwards_struct.insert({{result_mail, {NULL, {result_mail}}}});
//	}
	
//	cout << "=== BUILD END\n";
	return result_mail;
}

std::pair<std::string, std::string> FileParser::parseEntity(std::string& fullString, std::string::iterator& startIter)
{
	auto it = startIter;
	
	if (this->parser_foundHeadersEnd)
	{
		startIter = fullString.end();
		return std::pair<std::string, std::string>("Content", std::string(it, startIter - 1));
	}
	
	unsigned int goBack = 0;
	std::string key, value;
	
	while(*it != ':')
	{
		++it;
	}
	key = std::string(startIter, it);
	startIter = ++++it;
	
	while(it != fullString.end())
	{
		if (*it == 0x0A)
		{
			it += 1;
			// Check if it's end of header, line break of end of all headers
			if (*it == ' ' || *it == 0x9)
			{
				// Line break, continue;
				continue;
			}
			if (*it == 0x0D && *(it + 1) == 0x0A)
			{
				// End of all headers found
				this->parser_foundHeadersEnd = true;
				goBack += 2;
				break;
			}
			else if (*it == 0x0A)
			{
				// End of all headers found
				this->parser_foundHeadersEnd = true;
				goBack += 1;
				break;
			}
			goBack += 1;
			if ((it - 1) != fullString.begin() &&  *(it - 2) == 0x0D)
			{
				goBack += 1;
			}			
			break;
		}
		
		++it;
	}
	
	value = std::string(startIter, it - goBack);
	startIter = it;

	return std::pair<std::string&, std::string&>(key, value);
}
