#include "PQ.h"

void pq::siftUp(int heapPos){
    if(heapPos > 0 && heap[(heapPos+1)/2-1] > heap[heapPos]){
        std::pair<key, Element*> old_parent = heap[(heapPos+1)/2-1];
        heap[(heapPos+1)/2-1] = heap[heapPos];
        heap[heapPos] = old_parent;
        heap[heapPos].second->heapIndex = heapPos;
        heap[(heapPos+1)/2-1].second->heapIndex = (heapPos+1)/2-1;
        siftUp((heapPos+1)/2-1);
    }
}

void pq::siftDown(int heapPos){
    int m;
    if(2*(heapPos+1)-1 <= heap.size()-1){
        if(2*(heapPos+1) > heap.size()-1 || heap[2*(heapPos+1)-1] <= heap[2*(heapPos+1)]){
            m = 2*(heapPos+1)-1;
        } 
        else{
            m = 2*(heapPos+1);
        }
        if(heap[heapPos] > heap[m]){
            std::pair<key, Element*> old_parent = heap[heapPos];
            heap[heapPos] = heap[m];
            heap[m] = old_parent;
            heap[heapPos].second->heapIndex = heapPos;
            heap[m].second->heapIndex = m;
            siftDown(m);
        }
    }
}

int pq::size(){
    return heap.size();
}

void pq::insert(Element* element, int key){
    std::pair<int, Element*> insertion;
    insertion.first = key;
    insertion.second = element;
    heap.resize(size()+1);
    heap.back() = insertion;
    heap.back().second->heapIndex = size()-1;
    siftUp(size()-1);
}

Element pq::min(){
    return *heap.front().second;
}

Element pq::deleteMin(){
    Element output = *heap.front().second;
    output.heapIndex = -1;
    heap.front() = heap.back();
    heap.front().second->heapIndex = 0;
    heap.pop_back();
    if(size() > 0) siftDown(0);
    return output;
}

void pq::remove(Element* element){
    heap[element->heapIndex] = heap.back();
    heap[element->heapIndex].second->heapIndex = element->heapIndex;
    heap.pop_back();
    siftDown(element->heapIndex);
}

void pq::decreaseKey(Element* element, key newKey){
    heap[element->heapIndex].first = newKey;
    siftUp(element->heapIndex);
}
