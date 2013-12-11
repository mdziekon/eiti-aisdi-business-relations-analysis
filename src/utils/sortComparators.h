//
//  classsorter.h
//  AISDI
//
//  Created by Daniel Obrebski on 10/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved.
//

#ifndef AISDI_classsorter_h
#define AISDI_classsorter_h

#include <iostream>
#include <cstdlib>
#include "Containers.hpp"
using namespace std;

template<class T> class SortComparator
{
public:
	virtual bool compare(T& left, T& right) = 0;
};

class SortByDateAsc : public SortComparator<class Mail>
{
	bool compare(Containers::Mail& left, Containers::Mail& right)
	{
		return (left.sendTimestamp > right.sendTimestamp);
	}
};

class SortByDateDsc : public SortComparator<class Mail>
{
	bool compare(Containers::Mail& left, Containers::Mail& right)
	{
        return (left.sendTimestamp < right.sendTimestamp);
	}
};

class SortBySenderAsc: public SortComparator<class Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
		return (left.sender > right.sender);
	}
};

class SortBySenderDsc: public SortComparator<class Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
        return (left.sender < right.sender);
	}
};

class SortByReceiverAsc: public SortComparator<class Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
		return (left.receiver > right.receiver);
	}
};

class SortByReceiverDsc: public SortComparator<class Mail>
{
    bool compare(Containers::Mail& left, Containers::Mail& right)
	{
        return (left.receiver < right.receiver);
	}
};

#endif
