#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP
#include "../containers/Containers.hpp"

#include <string>

class FileParser 
{
public:
	Containers::Mail * build (string & str);
	string getMail (string & input);
};

#endif /* FILEPARSER_HPP */
		