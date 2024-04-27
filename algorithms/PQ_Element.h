#ifndef __PQ_ELEMENT_H__ 
#define __PQ_ELEMENT_H__ 

#include <vector>

struct Element{
    int key, heapIndex;
    int content; //change this to whatever variable type you need;
    Element(int k, int index) : key(k), heapIndex(index) {};
};

#endif
