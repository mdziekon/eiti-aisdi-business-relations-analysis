#ifndef PARSER_HPP
#define PARSER_HPP
#include "../models/Containers.hpp"
#include "../utils/MD5.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

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
	
	bool CzyKatalog(const std::string & path);
	std::pair<std::string, std::string> parseEntity(std::string& fullString, std::string::iterator& startIter);
	std::string parseEmail(const std::string & input);
	Containers::Date parseTime(const std::string & input);
	Containers::Person * addPerson(const std::string & email);
        vector<pair<Containers::Person*, Containers::Receiver>> parseMultiple(
                const std::string & input, Containers::Receiver type);
public:
	std::unordered_map<std::string, Containers::Mail*> forwards_hashes;
	void checkForwards(vector<Containers::Mail*>& mails);
		
	std::unordered_map<std::string, Containers::Person*> getCache();
	std::vector<Containers::Mail*> load(const std::string path);
	Containers::Mail * load_plik(const char* path);
	Containers::Mail * build (std::string & str);
	int cacheSize();
	void loadFwds(vector<Containers::Mail*>& mails);
};

#endif /* FILEPARSER_HPP */
		
