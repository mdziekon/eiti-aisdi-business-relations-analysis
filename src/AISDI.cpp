//============================================================================
// Name        : AISDI.cpp
// Authors     : Michał Dziekoński, Wioletta Klimczak, Szymon Mysiak, Daniel Obrębski, Jakub Skrętowski
// Copyright   : MIT License (http://opensource.org/licenses/MIT)
//============================================================================

#include "models/Containers.hpp"
#include "controllers/Parser.hpp"

using namespace std;

int main()
{	
	FileParser fp;
	fp.load("typowy_list.eml");
	return 0;
}
