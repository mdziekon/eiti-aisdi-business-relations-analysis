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
	const char * days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	const char * months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
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
		if(days[i] == weekday.c_str()) {
			time_struct.tm_wday = i;
			break;
		}
	}
	/* zamiana miesiąca na liczbę */
	for(int i = 0; i < 12; ++i) {
		if(months[i] == month.c_str()) {
			time_struct.tm_mon = i;
			break;
		}
	}
	/* zamiana od roku do sekund na liczby */
	time_struct.tm_year = atoi(year.c_str()) - 1900;
	time_struct.tm_mday = atoi(day.c_str());
	time_struct.tm_hour = atoi(hours.c_str());
	time_struct.tm_min = atoi(minutes.c_str());
	time_struct.tm_sec = atoi(seconds.c_str());
	time_struct.tm_isdst = -1;

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

	std::cout << "Emails list:\n" << input << endl;
	
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
					std::cout << "Email: " << mail_address << endl;
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
				std::cout << "Email: " << mail_address << endl;
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

void FileParser::checkForwards(std::string & title, std::string & contents, Mail * mail) 
{
    vector<std::string> hashes;
    int depth;
    int counter = 0;
    Mail * base_mail = mail;
            
    std::string::iterator it = title.begin();
    while(it < title.end() - 4) {
        if(std::string(it, it + 4) == "Fwd:") {
            counter++;
            it += 3;
        }
        it++;
    }
    
    depth = counter;
    
    //wyczysc puste znaki z przodu i z tylu
    {
        std::string::iterator rear_it = contents.end();
        std::string::iterator front_it = contents.begin();
        while(*rear_it-- <= ' ');
        while(*front_it++ <= ' ');
        contents = std::string(front_it-1, rear_it+2);
    }
    
    std::size_t found = 0;
    while(counter --> -1) {
        int i;
        found = contents.find("Forwarded message from", found);
        hashes.push_back(md5->md5ify(contents));
        if(found < contents.size())
            contents = contents.substr(found);     // get from "live" to the end
    }
    /* look for base mail sender in end mail vector of receivers to check
       completeness of an eventual cycle */

    // done with storing hashes, let's find forwards
    while(hashes.size() > 1) {
        Mail * current_mail = mail_cache.find(*(hashes.end() - 1))->second;
        hashes.pop_back();
        for(std::pair<Person*, Receiver> p : current_mail->receivers) {
            base_mail->forwarded_to.push_back(p.first);
        }
        bool exit_conditional = false;
        for ( Person * bs : base_mail->forwarded_to) {
            for ( std::pair<Person*, Receiver> ct : current_mail->receivers ) {
                if(bs == ct.first) {
                    exit_conditional = true;
                    current_mail->part_of_a_cycle = true;
                    break;
                }
            }
            if(exit_conditional) {
                break;
            }
        }
    }
    
    if(depth > 0) {
        for(Person * p : base_mail->forwarded_to) {
            if(base_mail->sender == p) {
                base_mail->complete_cycle = true;
                break;
            }
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

Containers::Mail * FileParser::build(std::string& str)
{	
	this->parser_foundHeadersEnd = false;
	auto it = str.begin();
	Person * sender = NULL;
	vector<pair<Person*, Receiver>> receivers;
        Mail * result_mail = NULL;
                
        md5 = new MD5();

	Headers headers;
        std::string title;
	std::string contents;
	Date time;
	
	while(it != str.end())
	{
		auto result = this->parseEntity(str, it);
		if(result.first == "Date") {
			time = this->parseTime(result.second);
		}
		else if(result.first == "From") {
		    sender = this->addPerson(this->parseEmail(result.second));
		} 
		else if (result.first == "To") {
                    std::vector<std::pair<Person*, Receiver>> temp = 
                    this->parseMultiple(result.second, Normal);
                    receivers.insert(receivers.end(), temp.begin(), temp.end());
		} 
		else if (result.first == "Content") {
		    contents = result.second;
                    result_mail = new Mail(*sender, receivers, contents, headers, time);
		} 
                else if (result.first == "Cc") {
                    std::vector<std::pair<Person*, Receiver>> temp = 
                        this->parseMultiple(result.second, Copy);
                    receivers.insert(receivers.end(), temp.begin(), temp.end());
                }
                else if (result.first == "Bcc") {
                    std::vector<std::pair<Person*, Receiver>> temp = 
                        this->parseMultiple(result.second, CarbonCopy);
                    receivers.insert(receivers.end(), temp.begin(), temp.end());
                }
                else if (result.first == "Reply-To") {
                    std::vector<std::pair<Person*, Receiver>> temp = 
                        this->parseMultiple(result.second, Reply);
                    receivers.insert(receivers.end(), temp.begin(), temp.end());
                }
                else if(result.first == "Subject") {
                    headers.addHeader(result.first, result.second);
                    title = result.second;   
                }
                else {
		    headers.addHeader(result.first, result.second);
		} 
	}
//        this->checkForwards(title, contents, result_mail);
        mail_cache.insert(std::pair<std::string, Mail*>(std::string(md5->md5ify(contents)), result_mail));
        delete md5;
	return result_mail;
}

std::pair<std::string, std::string> FileParser::parseEntity(std::string& fullString, std::string::iterator& startIter)
{
	auto it = startIter;
	
	if (this->parser_foundHeadersEnd)
	{
		startIter = fullString.end();
		return std::pair<std::string, std::string>("Content", std::string(it, startIter));
	}
	
	unsigned int goBack = 0;
	std::string key, value;
	
	while(*it != ':')
	{
		++it;
	}
	key = std::string(startIter, it);
	startIter = ++++it;
	
	while(true)
	{
		if (*it == 0x0D && *(it + 1) == 0x0A)
		{
			it += 2;
			// Check if it's end of header, line break of end of all headers
			if (*it == ' ')
			{
				// Line break, continue;
				continue;
			}
			if (*it == 0x0D && *(it + 1) == 0x0A)
			{
				// End of all headers found
				this->parser_foundHeadersEnd = true;
				goBack = 4;
				break;
			}
			goBack = 2;
			break;
		}
		
		++it;
	}
	
	value = std::string(startIter, it - goBack);
	startIter = it;

	return std::pair<std::string&, std::string&>(key, value);
}
