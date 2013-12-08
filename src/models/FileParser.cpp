#include "FileParser.hpp"
#include "../Exceptions.hpp"
#include <cstdio>
#include <ctime>
#include <dirent.h>
#include <fstream>
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
	if (CzyKatalog(path) == true)
	{
		DIR* p_dir = opendir(path.c_str());

		struct dirent* p_file;

		while ((p_file = readdir(p_dir)) != NULL)
		{
			load_plik(p_file->d_name);
		}
	}
	else
	{
		load_plik(path.c_str());
	}
}

void FileParser::load_plik(const char* path)
{
	std::string Str;
	FILE *File = fopen(path, "r");

	while (!feof(File)) {
		Str += fgetc(File);
	}

	build(Str);
}

using namespace Containers;

/* FileParser: wykrywa wszystkie widoczne znaki do momentu znalezienia
	dwukropka; przyjmuje iterator po mailu, który modyfikuje do wskazywania
	na ostatni parsowany znak*/

std::string FileParser::getHeader(std::string & str, std::string::iterator & it)
{
	// sprawdŸ, czy iterator nale¿y do stringa
	if (it < str.begin() || it > str.end()) {
		throw (GenericException("Niewazny iterator"));
	}

	std::string headerName = '\0';

	while (*it > 32 && *it < 127 && *it != ':') {
		headerName += *it;
		if (*it == EOF) {
			throw (GenericException("Nieoczekiwany koniec pliku"));
		}
		it++;
	}

	return headerName;
}

std::string FileParser::getContents(std::string & str, std::string::iterator & it, bool & end_of_headers)
{
	// sprawdŸ, czy iterator nale¿y do stringa
	if (it < str.begin() || it > str.end()) {
		throw (GenericException("Niewazny iterator"));
	}

	std::string contents;
	/* last_newline przyda siê do powrotu do koñca contentu,
	   gdy wkroczyliœmy na teren headera */
	std::string::iterator last_newline = it;
	std::string::iterator current_it = it;

	while (*current_it != ':') {
		
		/* jesli pojedyncza nowa linia, to jest to line break i zachowujemy go 
		   jako potencjalny koniec pola */

		if (*current_it == '\r') {
			if (*current_it == '\n') {
				last_newline = current_it;

				/* jesli druga nowa linia, to zaczyna sie pole wiadomosci i ustawiamy
				   znacznik end_of_headers */

				if (*current_it == '\r') {
					if (*current_it == '\n') {
						end_of_headers = true;
						break;
					}
					else {
						throw(GenericException("Blad parsowania nowej linii"));
					}
				}
			}
			else {
				throw (GenericException("Blad parsowania nowej linii"));
			}
		}
	}

	/* wszystkie line breaki zamieniane na odstêpy */

	contents = std::string(it, --last_newline);
	for (; current_it < contents.end(); current_it++) {
		if (*current_it == '\n') {
			contents.replace(current_it, current_it + 1, " ");
		}
	}

	return contents;
}

int FileParser::parseTime(std::string & str, std::string::iterator & it)
{
	//TODO: parser czasu
	bool generic_bool = false;
	getContents(str, it, generic_bool);
	return 0; 
}

std::string FileParser::parseEmail(std::string & str)
{
	return str.substr(str.find_last_of('<') + 1, str.find_last_of('>') - 1);
}

Person * FileParser::add(string & email)
{
	std::vector<Person*>::iterator it = cache.begin();
	for (; it != cache.end(); it++) {
		Person * temp = *it;
		if (temp->getEmail().getFull() == email) {
			return *it;
		}
	}
	cache.push_back(new Person(email));
	return *cache.end();
}

const FileParser::vector<Person*> getPersonList()
{
	return vector<Person*>(cache);
}

Mail * FileParser::build (string & str)
{
	Person * sender;
	Person * receiver;
	std::string contents;
	Headers headers;
	int time;

	std::string::iterator it = str.begin();
	std::string header;
	/* brzydkie rozwi¹zanie, ale nie mam innego pomys³u */
	bool end_of_headers = false; 

	while (!end_of_headers) {
		header = getHeader(header, it);
		if (header == "From") {
			std::string temp = getContents(str, it, end_of_headers);
			temp = parseEmail(temp);
			sender = add(temp);
		}
		else if (header == "To") {
			std::string temp = getContents(str, it, end_of_headers);
			temp = parseEmail(temp);
			receiver = add(temp);
		}
		else if (header == "Received") {
			time = parseTime(str, it);
		}
		else {
			headers.addHeader(header, getContents(str, it, end_of_headers));
		}
	}

	contents = std::string(it, str.end());
	Mail * mail = new Mail(*sender, *receiver, contents, headers, time);
	delete sender;
	delete receiver;
	return mail;
}