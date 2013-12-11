//
//  sortComparators.h
//  AISDI
//
//  Created by Daniel Obrebski on 10/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved.
//TUTAJ MUSISZ POZMIENIAC tez na wskazniki, compare powininen przyjmowac nie obiekty a wskazniki

#ifndef AISDI_sortComparators_h
#define AISDI_sortComparators_h

#include <iostream>
#include <cstdlib>
#include "Containers.hpp"

using namespace std;

template<class T> class SortComparator
{
public:
	virtual bool compare(T& left, T& right) = 0;
};

class SortByDateAsc : public SortComparator<class Containers::Mail>
{
	bool compare(Containers::Mail& left, Containers::Mail& right)
	{
		return (left.sendTimestamp > right.sendTimestamp);
	}
};

class SortByDateDsc : public SortComparator<class Containers::Mail>
{
	bool compare(Containers::Mail& left, Containers::Mail& right)
	{
        return (left.sendTimestamp < right.sendTimestamp);
	}
};

class SortBySenderAsc: public SortComparator<class Containers::Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
		return (left.sender->getName() > right.sender->getName());
	}
};

class SortBySenderDsc: public SortComparator<class Containers::Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
        return (left.sender->getName() < right.sender->getName());
	}
};

class SortByReceiverAsc: public SortComparator<class Containers::Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
		return (left.receiver->getName() > right.receiver->getName());
	}
};

class SortByReceiverDsc: public SortComparator<class Containers::Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
        return (left.receiver->getName() < right.receiver->getName());
	}
};

#endif
