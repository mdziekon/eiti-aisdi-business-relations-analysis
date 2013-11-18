#include "FileParser.hpp"
#include <cstdio>

using namespace Containers;

Mail * FileParser::build (string & str)
{
	const string lastHeaderKey = "Content-Transfer-Encoding";
	
	string key;
	string value;
	
	string from;
	string to;
	
	Headers headers;
        
        std::string::iterator it = str.begin();
	
	for(; it < str.end() && key != lastHeaderKey; key.clear(), value.clear(), it++) {
            
                /* petla pobierajaca klucz headera*/
            
		while(*it++ != ':') {	
			key += *--it;				
		} 
		*it++;
		
                /* petla pobierajaca zawartosc headera - 
                  jesli wiersz zaczyna sie od spacji, to
                  jest to kontynuacja zawartosci */
                
		while(*it++ ==  ' ')  {	
                        
                        /* pominiecie wciecia */
                    
			while(*it++ == ' ');
                        
                        /* koniec linii - sekwencja \r\n
                           przesuwamy az do \r, potem 
                           pomijamy \n */
                        
			while(*it != '\r') {
				value = value + *it++;
                        }
			it++;
		}
		
                /* warunki potrzebne do stworzenia 
                   obiektow nadawcy i odbiorcy */
                
		if(key == "From") {
			from = getMail(value);
                }
		else if(key == "To") {
			to = getMail(value);
                }
		else {
			headers.addHeader(key, value);
                }
                
                printf("%s %s \n", key.c_str(), value.c_str());
	}
        
	return new Mail(Person(from), Person(to), std::string(it++, str.end()), headers, 0);
	/* TODO czasoznaczek */
}

string FileParser::getMail (string & input)
{       
        std::string::iterator it = input.begin();
        std::string::iterator end_it;
	
	while(*it++ != '<');
	for(end_it = it; *end_it != '>'; end_it++);

	
	return string(++it, --end_it);
}