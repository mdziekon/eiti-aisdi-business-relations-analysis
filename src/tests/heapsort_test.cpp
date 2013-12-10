//
//  heapsort_test.cpp
//  AISDI
//
//  Created by Daniel Obrebski on 09/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "heapsort.h"

using namespace std;

int main(void)
{
    cout << "Sortowanie intow: " << endl;
    vector<int>tab(40);
    tab[0] = NULL;
    
    for (int i = 1; i < 40; i++)
    {
        tab[i] = rand();
        cout << tab[i] << " ";
    };
    cout << endl;
    
    heapsort<int>(tab, tab.size() - 1);
    
    for (int i = 1; i < 40; i++)
        cout << tab[i] << " ";
    cout << endl;
    cout << endl;
    
    cout << "Sortowanie double'i: " << endl;
    
    
    vector<double>tab2(40);
    tab[0] = NULL;
    
    for (int i = 1; i < 40; i++)
    {
        tab2[i] = rand();
        cout << tab2[i] << " ";
    };
    cout << endl;
    
    heapsort<double>(tab2, tab2.size() - 1);
    
    for (int i = 1; i < 40; i++)
        cout << tab2[i] << " ";
    cout << endl;
    cout << endl;
    
    cout << "Sortowanie stringow: " << endl;
    
    
    vector<string>tab3 {"asd", "oeuw", "shdfi", "qowe", "jcxv", "ewoutw", "asdhiqwe", "sdhfs", "orteuio", "sfhasf", "qwiqewq", "dsfiuertre", "iuasdias", "qweyuxnxv", "hasdyqwe", "iuwrewkhs", "qweqgb", "ascbxvcx", "asfuer"};
    tab[0] = NULL;
    
    for (int i = 1; i < tab3.size(); i++)
    {
        cout << tab3[i] << " ";
    };
    cout << endl;
    
    heapsort<string>(tab3, tab3.size() - 1);
    
    for (int i = 1; i < tab3.size(); i++)
        cout << tab3[i] << " ";
    cout << endl;
    return 0;
}