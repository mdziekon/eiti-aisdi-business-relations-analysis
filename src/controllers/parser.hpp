#ifndef PARSER_HPP
#define PARSER_HPP
#include "../models/Containers.hpp"

#include <string>
#include <vector>
#include <unordered_set>

class FileParser 
{
private:
	/* statystyka */
	int howManyMails;
	int relationsCreated;

	bool parser_foundHeadersEnd;
	static std::unordered_set<Containers::Person*> cache;

	void load_plik(const char* path);
	bool CzyKatalog(const std::string & path);
	std::pair<std::string, std::string> parseEntity(std::string& fullString, std::string::iterator& startIter);
	std::string parseEmail(const std::string & input);
	int parseTime(const std::string & input);
	std::unordered_set<Containers::Person*> getCache();
	bool addPerson(Containers::Person * p);
public:
	void load(const std::string path);
	Containers::Mail * build (std::string & str);
	int cacheSize();
};

#endif /* FILEPARSER_HPP */
		
