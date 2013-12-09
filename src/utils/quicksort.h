//
//  quicksort.h
//  AISDI
//
//  Created by Daniel Obrebski on 09/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved.
//

#ifndef AISDI_quicksort_h
#define AISDI_quicksort_h

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

template<class T> void quicksort(vector<T> &a, const long& const_left, const long& const_right)
{
    if (const_left < const_right) {
        
        T mid_value = a[const_left];
        long temp_left = const_left - 1;
        long temp_right = const_right + 1;
        
        while(1)
        {
            
            while (a[--temp_right] > mid_value); //jezeli przekroczymy nasz "srodek"
            while (a[++temp_left] < mid_value);
            
            if (temp_left >= temp_right) // w momencie krytycznym wychodzimy z sortowania
                break;
            
            T temp = a[temp_right];
            a[temp_right] = a[temp_left]; //swap
            a[temp_left] = temp;
        }
        
        long mid = temp_right;
        quicksort(a, const_left, mid); // rekurencyjnie wywolujemy
        quicksort(a, mid + 1, const_right);
    }
}

#endif
