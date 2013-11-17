#include "FileParser.hpp"

Containers::Mail & FileParser::build (string & str)
{
	const string lastHeaderKey = "Content-Transfer-Encoding"
	
	unsigned int currPos = 0;
	unsigned int maxPos = str.size();
	
	string key;
	string value;
	
	string from;
	string to;
	
	Containers::Headers headers;
	
	for(; currPos < str.size() && key != lastHeaderKey; key.clear(), value.clear() currPos++) {
		while(str[currPos++] != ':') {		/* wykrycie headera */
			key += buffer;				
		} 
		currPos++;
		
		while(str[currPos++] ==  ' ')  {	/* wykrycie dowolnej liczby linijek zawartosci */	
			while(str[currPos++] == ' ');	/* dzieki wcieciom na poczatku */
			while(str[currPos] != '\r')
				value = value + str[currPos++];
			currPos++;						//sekwencja \r\n
		}
		
		if(key == "From")
			from = value.getMail();
		else if(key == "To")
			to = value.getMail();
		else
			headers.addHeader(key, value);
	}
	
	return Mail(Containers::Person(from), Containers::Person(to), str.substr(currPos, str.size()-currPos), headers, 0);
	/* nie wiem, w jakim formacie ma być czasoznaczek */
}

string & FileParser::getMail (string & input)
{
	string output;
	
	while(input[startpos++] != '<')
		while(input[startpos] != '>')
			output += input[startpos];
	
	return output;
}