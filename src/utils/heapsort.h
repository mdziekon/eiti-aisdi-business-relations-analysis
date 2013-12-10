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
using namespace std;

template <class T> void heapify (vector<T> &tab, long root, long heap_size)
 {
     long max;
     T swap;
     long left = 2 * root;
     long right = 2 * root + 1;
     if (left <= heap_size && tab[left]>tab[root])
         max = left;
     else
         max = root;
     if (right <= heap_size && tab[right] > tab[max])
         max = right;
     
     if(max != root)
     {
         swap = tab[max];
         tab[max] = tab[root];
         tab[root] = swap;
         heapify(tab, max, heap_size);
     }
         
 }
template <class T> void build_heap (vector<T> &tab, long size)
{
         for(long i = size/2; i > 0; i--)
             heapify(tab, i, size);
}

template <class T> void heapsort (vector<T> &tab, long size)
{
    T swap;
    build_heap(tab, size);
    for(long i = size; i > 1; i--)
    {
        swap = tab[i];
        tab[i] = tab[1];
        tab[1] = swap;
        size--;
        heapify(tab, 1, size);
    }
}
#endif
