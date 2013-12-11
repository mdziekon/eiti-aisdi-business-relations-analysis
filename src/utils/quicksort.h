//
//  quicksort.h
//  AISDI
//
//  Created by Daniel Obrebski on 09/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved
//

#ifndef AISDI_quicksort_h
#define AISDI_quicksort_h

#include <iostream>
#include <cstdlib>
#include <vector>
#include "sortComparators.h"

using namespace std;

template<class T> void quicksort(vector<T> &tab, const long& const_left, const long& const_right, SortComparator<T>&cmp) //zamienic na wskazniki w vectorze.
{
    if (const_left < const_right) {
        
        T mid_value = tab[const_left]; //zamienic na wksaznik
        long temp_left = const_left - 1;
        long temp_right = const_right + 1;
        
        while(1)
        {
            
            while (cmp.compare(tab[--temp_right], mid_value) == 1); //jezeli przekroczymy nasz "srodek"
            while (cmp.compare(mid_value, tab[++temp_left]) == 1); //zamienic na wskaznik w compare
            
            if (temp_left >= temp_right) // w momencie krytycznym wychodzimy z sortowania
                break;
            
            T temp = tab[temp_right]; //pozamieniac na wskazniki
            tab[temp_right] = tab[temp_left]; //swap
            tab[temp_left] = temp;
        }
        
        long mid = temp_right;
        quicksort(tab, const_left, mid, cmp); // rekurencyjnie wywolujemy
        quicksort(tab, mid + 1, const_right, cmp);
    }
}

#endif
