//============================================================================
// Name        : AISDI.cpp
// Authors     : Michał Dziekoński, Wioletta Klimczak, Szymon Mysiak, Daniel Obrębski, Jakub Skrętowski
// Copyright   : MIT License (http://opensource.org/licenses/MIT)
//============================================================================

#include <iostream>
#include "containers/Containers.hpp"

using namespace std;

int main()
{
	Containers::Person p1("admin.kowalski@mail.com"), p2("admin2@mail.com");
	Containers::Mail mail(p1, p2, "", Containers::Headers(), 1);
	
	cout << mail.sender->getName() << endl;;

	return 0;
}
