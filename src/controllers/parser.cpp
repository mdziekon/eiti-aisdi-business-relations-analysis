#include "parser.hpp"
#include "../utils/Exceptions.hpp"
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

void FileParser::load(const std::string path)
{
	if (this->CzyKatalog(path) == true)
	{
		DIR* p_dir = opendir(path.c_str());

		struct dirent* p_file;

		while ((p_file = readdir(p_dir)) != NULL)
		{
			this->load_plik(p_file->d_name);
		}
	}
	else
	{
		this->load_plik(path.c_str());
	}
}

void FileParser::load_plik(const char* path)
{
	std::string Str;
	FILE *File = fopen(path, "r");

	if (!File) {
		throw(GenericException("Nie odnaleziono pliku"));
	}

	while (!feof(File)) {
		Str += fgetc(File);
	}
	this->build(Str);
}

using namespace Containers;

int FileParser::parseTime(const std::string & input)
{
	tm * time_struct;
	time_t rawtime;

	std::string::const_iterator it = input.begin(); //spacja + dzień + spacja
	std::string weekday, day, month, year, hours, minutes, seconds;
	const char * days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	const char * months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
				"Aug", "Sep", "Oct", "Nov", "Dec"};

	time(&rawtime);
	time_struct = localtime(&rawtime);
	
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
	year.reserve(2);
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
	for(int i = 0; i < 12; ++i) {
		if(days[i] == weekday.c_str()) {
			time_struct->tm_wday = i;
			break;
		}
	}
	/* zamiana miesiąca na liczbę */
	for(int i = 0; i < 12; ++i) {
		if(months[i] == month.c_str()) {
			time_struct->tm_mon = i;
			break;
		}
	}
	/* zamiana od roku do sekund na liczby */
	time_struct->tm_year = atoi(year.c_str()) - 1900;
	time_struct->tm_mday = atoi(day.c_str());
	time_struct->tm_hour = atoi(hours.c_str());
	time_struct->tm_min = atoi(minutes.c_str());
	time_struct->tm_sec = atoi(seconds.c_str());
	time_struct->tm_isdst = -1;

	return mktime(time_struct);
}
	

std::string FileParser::parseEmail(const std::string & input)
{
	return input.substr(input.find_last_of('<') + 1, input.find_last_of('>') - 1);
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
	Person * sender;
	Person * receiver;

	Headers headers;
	std::string contents;
	int time = 0;
	
	while(it != str.end())
	{
		auto result = this->parseEntity(str, it);
		std::cout << result.first << '\n';
		if(result.first == "Date") {
			time = parseTime(result.second);
		}
		else if(result.first == "From") {
			sender = this->addPerson(this->parseEmail(result.second));
		} 
		else if (result.first == "To") {
			receiver = this->addPerson(this->parseEmail(result.second));
		} 
		else if (result.first == "Contents") {
			 contents = result.second;
		} 
		else {
			headers.addHeader(result.first, result.second);
		} 
	} 
	return new Mail (*sender, *receiver, contents, headers, time);
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
