#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP
#include "../models/Containers.hpp"

#include <string>
#include <vector>
#include <stdint.h>

class FileParser 
{
private:
	vector<Containers::Person*> cache;

	std::string getHeader(std::string & str, std::string::iterator & it);
	std::string getContents(std::string & str, std::string::iterator & it, bool & end_of_headers);
	int parseTime(std::string & str, std::string::iterator & it);
	std::string parseEmail(std::string & str);
	void load_plik(const char* path);
	bool CzyKatalog(const std::string & path);
	Containers::Person * add(string & email);
	const vector<Containers::Person*> getPersonList();
public:
	void load(const std::string path);
	Containers::Mail * build (string & str);
	string getMail (string & input);
};

#endif /* FILEPARSER_HPP */
		