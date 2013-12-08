//============================================================================
// Name        : AISDI.cpp
// Authors     : Michał Dziekoński, Wioletta Klimczak, Szymon Mysiak, Daniel Obrębski, Jakub Skrętowski
// Copyright   : MIT License (http://opensource.org/licenses/MIT)
//============================================================================

#include <iostream>
#include <sstream>
#include "containers/Containers.hpp"

using namespace std;

int main()
{
//	Containers::Person p1("admin.kowalski@mail.com"), p2("admin2@mail.com");
//	Containers::Mail mail(p1, p2, "", Containers::Headers(), 1);
//	
//	cout << mail.sender->getName() << endl;;

	vector<Containers::Person*> people;
	
	for(int i = 0; i < 20; i++)
	{
		stringstream ss;
		ss<<i<<"mail@ds.pl";
		people.push_back(new Containers::Person(ss.str()));
	}
	for(int i = 0; i < 20; i++)
	{
		cout<<people[i]->getName()<<endl;
	}
	
	return 0;
}
