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
using namespace std;

class Mail
{
public:
    string name;
    int data;
};

template<class T> class SortComparator
{
public:
	virtual int compare(T& left, T& right) = 0;
};

class SortByDateAsc : public SortComparator<class Mail>
{
	int compare(Mail& left, Mail& right)
	{
        if(left.data > right.data)
            return 1;
		if(left.data == right.data)
            return 0;
        if(left.data < right.data)
            return -1;
        return -2;
	}
};

class SortByDateDesc : public SortComparator<class Mail>
{
	int compare(Mail& left, Mail& right)
	{
        if(left.data < right.data)
            return 1;
		if(left.data == right.data)
            return 0;
        if(left.data > right.data)
            return -1;
        return -2;
	}
};

class SortByNameAsc: public SortComparator<class Mail>
{
    int compare(Mail& left, Mail& right)
	{
        if(left.name > right.name)
            return 1;
		if(left.name == right.name)
            return 0;
        if(!(left.name > right.name))
            return -1;
        return -2;
	}
};

class SortByNameDsc: public SortComparator<class Mail>
{
    int compare(Mail& left, Mail& right)
	{
        if(left.name > right.name)
            return -1;
		if(left.name == right.name)
            return 0;
        if(!(left.name > right.name))
            return 1;
        return -2;
	}
};

#endif
