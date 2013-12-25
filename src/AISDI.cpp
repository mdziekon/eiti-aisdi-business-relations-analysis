//============================================================================
// Name        : AISDI.cpp
// Authors     : Michał Dziekoński, Wioletta Klimczak, Szymon Mysiak, Daniel Obrębski, Jakub Skrętowski
// Copyright   : MIT License (http://opensource.org/licenses/MIT)
//============================================================================

#include "models/Containers.hpp"
#include "Filter.h"
using namespace std;

int main()
{
    Filter filter;
    TopicSubstringFilter tsf1("sd");

    filter.addNewFilter(tsf1);
	return 0;
}
