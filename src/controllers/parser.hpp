#ifndef PARSER_HPP
#define PARSER_HPP
#include "../models/Containers.hpp"
#include "../utils/MD5.h"

#include <string>
#include <vector>
#include <unordered_map>

class FileParser 
{
    friend class MainWindow;
private:
	/* statystyka */
	int howManyMails;
	int relationsCreated;
        
        MD5 * md5;

	bool parser_foundHeadersEnd;
	std::unordered_map<std::string, Containers::Person *> cache;
        std::unordered_map<std::string, Containers::Mail *> mail_cache;
	
	bool CzyKatalog(const std::string & path);
	std::pair<std::string, std::string> parseEntity(std::string& fullString, std::string::iterator& startIter);
	std::string parseEmail(const std::string & input);
	Containers::Date parseTime(const std::string & input);
	Containers::Person * addPerson(const std::string & email);
        vector<pair<Containers::Person*, Containers::Receiver>> parseMultiple(
                const std::string & input, Containers::Receiver type);
        void checkForwards(std::string & title, std::string & contents, Containers::Mail * mail);
public:
	std::unordered_map<std::string, Containers::Person*> getCache();
	std::vector<Containers::Mail*> load(const std::string path);
	Containers::Mail * load_plik(const char* path);
	Containers::Mail * build (std::string & str);
	int cacheSize();
};

#endif /* FILEPARSER_HPP */
		
