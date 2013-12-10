//
//  quicksort_test.cpp
//  AISDI
//
//  Created by Daniel Obrebski on 09/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "quicksort.h"
#include "classsorter.h"

using namespace std;



int main(void)
{

    cout << "Sortowanie intow: " << endl;
    vector<Mail>Kutasy(40);

    
    for (int i = 0; i < 40; i++)
    {
        Kutasy[i].data = rand();
        cout << Kutasy[i].data << " ";
    };
    cout << endl;
    SortByDateAsc cmp;
    
    quicksort<Mail>(Kutasy, 0, Kutasy.size() - 1, cmp);
    
    for (int i = 0; i < 40; i++)
        cout << Kutasy[i].data << " ";
    cout << endl;
    cout << endl;
    

    
    cout << "Sortowanie stringow: " << endl;
    
    
    for (int i = 0; i < 40; i++)
    {
        Kutasy[i].name = rand();
        cout << Kutasy[i].name << " ";
    };
    cout << endl;
    
    SortByNameAsc cmp2;
    quicksort<Mail>(Kutasy, 0, Kutasy.size() - 1, cmp2);
    
    for (int i = 0; i < 40; i++)
        cout << Kutasy[i].name << " ";
    cout << endl;
    return 0;
}