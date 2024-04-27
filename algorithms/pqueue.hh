#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

template<class Key, class Value>
class PQueue{
    public:
    virtual std::pair<Key, Value> min() const = 0;
    virtual std::pair<Key, Value> deleteMin() = 0;
    virtual void insert(const Key &key, const Value &value) = 0;
    virtual unsigned int size() const = 0;
    virtual void remove(const Value &value) = 0;
    virtual void decreaseKey(const Value &value, const Key &newKey) = 0;
};

template<class Key, class Value>
class BinaryHeap : public PQueue<Key, Value>{
    public:
    virtual std::pair<Key, Value> min() const override;
    virtual std::pair<Key, Value> deleteMin() override;
    virtual void insert(const Key &key, const Value &value) override;
    virtual unsigned int size() const override;
    virtual void remove(const Value &value) override;
    virtual void decreaseKey(const Value &value, const Key &newKey) override;

    private:
    using heapPos = unsigned int;

    void siftUp(heapPos pos);
    void siftDown(heapPos pos);

    //heap itself
    std::vector<std::pair<Key, Value>> h; 
    //hash-table to look up elements of heap and their position inside heap (for remove and decreaseKey operations)
    std::unordered_map<Value, heapPos> values;  
};

template<class Key, class Value>
class PairingHeap : public PQueue<Key, Value>{
    public:
    virtual std::pair<Key, Value> min() const override;
    virtual std::pair<Key, Value> deleteMin() override;
    virtual void insert(const Key &key, const Value &value) override;
    virtual unsigned int size() const override;
    virtual void remove(const Value &value) override;
    virtual void decreaseKey(const Value &value, const Key &newKey) override;

    private:
    struct node{
        Key key;
        Value value;
        node* leftSibling = nullptr;
        node* rightSibling = nullptr;
        node* child = nullptr;;
        node* parent = nullptr;
    };

    using root = node*;

    void cut(node* newRoot);
    void makeUnion(root firstRoot, root secondRoot);

    //main heap datastructure (forest of trees, use hash-set for this)
    std::unordered_set<root> forest;
    //minPtr always points to smallest root 
    root minPtr;
    //hash-table to look up elements of heap and their position
    std::unordered_map<Value, node*> values;
};

template<class Key, class Value>
std::pair<Key, Value> BinaryHeap<Key, Value>::min() const{
    return h[0]; 
}

template<class Key, class Value>
std::pair<Key, Value> BinaryHeap<Key, Value>::deleteMin(){
    std::pair<Key, Value> returnVal = h[0];

    //move last element to front
    h[0] = h.back();
    h.pop_back();

    //remove returnVal from hash-table as well
    values.erase(returnVal.second);

    //restore heap property
    siftDown(0);

    return returnVal;
}


template<class Key, class Value>
void BinaryHeap<Key, Value>::insert(const Key &key, const Value &value){
    //insert new element into heap
    h.push_back(std::make_pair(key, value));

    //insert it into hast-table as well
    heapPos pos = size() - 1;
    values.insert(std::make_pair(value, pos));

    //restore heap property
    siftUp(pos);
}

template<class Key, class Value>
unsigned int BinaryHeap<Key, Value>::size() const{
    return h.size();
}

template<class Key, class Value>
void BinaryHeap<Key, Value>::remove(const Value &value){
    //find element in heap using the hash-table
    auto it = values.find(value);
    heapPos pos = it->second;

    //move last element to front
    h[pos] = h[size() - 1];
    h.pop_back();

    //remove it from hash-table as well
    values.erase(it);

    //restore heap property
    siftDown(pos);
}

template<class Key, class Value>
void BinaryHeap<Key, Value>::decreaseKey(const Value &value, const Key &newKey){
    //find element in heap using the hash-table
    auto it = values.find(value);
    heapPos pos = it->second;

    //update key in heap
    h[pos].first = newKey;

    //restore heap property
    siftUp(pos);
}

template<class Key, class Value>
void BinaryHeap<Key, Value>::siftUp(heapPos pos){
    heapPos parentPos = (pos - 1) / 2;
    if(pos == 0 || h[parentPos].first <= h[pos].first) return;
    std::swap(h[pos], h[parentPos]);
    siftUp(parentPos);
}

template<class Key, class Value>
void BinaryHeap<Key, Value>::siftDown(heapPos pos){
    heapPos leftChild = 2 * pos + 1;
    heapPos rightChild = 2 * pos + 2;
    if(leftChild <= size()){
        heapPos m;
        if(rightChild > size() || h[leftChild] <= h[rightChild]){
            m = leftChild;
        }
        else m = rightChild;

        std::swap(h[pos], h[m]);
        siftDown(m);
    }
}



template<class Key, class Value>
std::pair<Key, Value> PairingHeap<Key, Value>::min() const{
    root min = forest[minPtr];
    return std::make_pair(min->key, min->value);
}

template<class Key, class Value>
std::pair<Key, Value> PairingHeap<Key, Value>::deleteMin(){
    root min = forest[minPtr];
    std::pair<Key, Value> returnVal = std::make_pair(min->key, min->value);



    //remove returnVal from hash-table as well
    values.erase(returnVal.second);

    //restore heap property
    siftDown(0);

    return returnVal;
}


template<class Key, class Value>
void BinaryHeap<Key, Value>::insert(const Key &key, const Value &value){
    //insert new element into heap
    h.push_back(std::make_pair(key, value));

    //insert it into hast-table as well
    heapPos pos = size() - 1;
    values.insert(std::make_pair(value, pos));

    //restore heap property
    siftUp(pos);
}

template<class Key, class Value>
unsigned int BinaryHeap<Key, Value>::size() const{
    return h.size();
}

template<class Key, class Value>
void BinaryHeap<Key, Value>::remove(const Value &value){
    //find element in heap using the hash-table
    auto it = values.find(value);
    heapPos pos = it->second;

    //move last element to front
    h[pos] = h[size() - 1];
    h.pop_back();

    //remove it from hash-table as well
    values.erase(it);

    //restore heap property
    siftDown(pos);
}

template<class Key, class Value>
void BinaryHeap<Key, Value>::decreaseKey(const Value &value, const Key &newKey){
    //find element in heap using the hash-table
    auto it = values.find(value);
    heapPos pos = it->second;

    //update key in heap
    h[pos].first = newKey;

    //restore heap property
    siftUp(pos);
}
