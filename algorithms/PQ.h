#ifndef __PRIORITYQUEUEJ__
#define __PRIORITYQUEUEJ__

#include <vector>
#include "PQ_Element.h"

class pq{
    private:
    typedef int key;
    std::vector<std::pair<key, Element*>> heap;

    void siftUp(int heapPos);

    void siftDown(int heapPos);

    public:
    int size();

    void insert(Element*, key Key);

    Element min();

    Element deleteMin();

    void remove(Element*);

    void decreaseKey(Element*, key newKey);
};
#endif
