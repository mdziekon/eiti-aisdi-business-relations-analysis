//
//  quicksort_test.cpp
//  AISDI
//
//  Created by Daniel Obrebski on 09/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved.
// TEST zrobiony generalnie, tylko trzeba popodstawiac.

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "quicksort.h"
#include "sortComparators.h"
#include "Containers.hpp"

using namespace std;


int main(void)
{
    vector<Containers::Mail*> mails;
    vector<Containers::Person*> people;
    
    people.push_back(new Containers::Person("kasia.kowalska@gmail.pl"));
    people.push_back(new Containers::Person("karolina.nowak@gmail.pl"));
    people.push_back(new Containers::Person("patryk.wozniak@gmail.pl"));
    people.push_back(new Containers::Person("slawek.dudek@gmail.pl"));
    people.push_back(new Containers::Person("marek.kwiatkowski@gmail.pl"));
    
    mails.push_back(new Containers::Mail(*people[0], *people[1], "", Containers::Headers(), 10)); //kasia do karoliny
    mails.push_back(new Containers::Mail(*people[1], *people[0], "", Containers::Headers(), 13)); //karolina do kasi
    mails.push_back(new Containers::Mail(*people[0], *people[2], "", Containers::Headers(), 8)); //kasia do patryka
    mails.push_back(new Containers::Mail(*people[3], *people[2], "", Containers::Headers(), 5)); //slawek do patryka
    mails.push_back(new Containers::Mail(*people[4], *people[0], "", Containers::Headers(), 2)); //marek do kasi

    cout << "Sortowanie po dacie, przed sortowaniem: " << endl;
    
    for (int i = 0; i < mails.size()-1; i++)
    {
        cout << mails[i]->sendTimestamp << " ";
    };
    cout << endl;
    
    SortByDateAsc cmp;
    
    cout << "Sortowanie po dacie, po sortowaniu: " << endl;
    
    quicksort<Containers::Mail>(mails, 0, mails.size() - 1, cmp);
    
    for (int i = 0; i < mails.size()-1; i++)
        cout << mails[i]->sendTimestamp << " ";
    cout << endl;
   
    
    cout << "Sortowanie po receiver, przed sortowaniem" << endl;
    
    for (int i = 0; i < mails.size()-1; i++)
    {
        cout << mails[i]->receiver << " ";
    };
    cout << endl;
    
    SortByReceiverAsc cmp2;
    
    cout << "Sortowanie po receiver, po sortowaniu: " << endl;
    
    quicksort<Containers::Mail>(mails, 0, mails.size() - 1, cmp2);
    
    for (int i = 0; i < mails.size()-1; i++)
        cout << mails[i]->receiver << " ";
    cout << endl;
}