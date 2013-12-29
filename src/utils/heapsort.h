//
//  heapsort.h
//  AISDI
//
//  Created by Daniel Obrebski on 09/12/13.
//  Copyright (c) 2013 Daniel Obrebski. All rights reserved.
//

#ifndef AISDI_heapsort_h
#define AISDI_heapsort_h

#include <iostream>
#include <cstdlib>
#include <vector>
#include "sortComparators.h"
using namespace std;

template <class T> void heapify (vector<T> &tab, long root, long heap_size, SortComparator<T> &cmp)
 {
     long max;
     T swap; // musisz tutaj pozmieniac na wskaznik
     long left = 2 * root;
     long right = 2 * root + 1;
     
     if(left <= heap_size && cmp.compare(tab[left], tab[root])) // w cmp musisz pozmieniac na wskazniki
         max = left;
     else
         max = root;
     
     if(right <= heap_size && cmp.compare(tab[right], tab[max])) // zmiana na wskazniki
         max = right;
     
     if(max != root)
     {
         swap = tab[max]; //zmiana na wskazniki
         tab[max] = tab[root];
         tab[root] = swap;
         heapify(tab, max, heap_size, cmp);
     }
         
 }
template <class T> void build_heap (vector<T> &tab, long size, SortComparator<T> &cmp)
{
         for(long i = size/2; i > 0; i--)
             heapify(tab, i, size, cmp);
}

template <class T> void heapsort (vector<T> &tab, long size, SortComparator<T> &cmp)
{
    T swap; //zmiana na wskazniki
    build_heap(tab, size, cmp);
    for(long i = size; i > 1; i--) //zmiana na wskazniki
    {
        swap = tab[i];
        tab[i] = tab[1];
        tab[1] = swap;
        size--;
        heapify(tab, 1, size, cmp);
    }
}
#endif
