template<typename t>
class SLL{
private:
    struct link{
        t value;
        link *next;
    };
    const link *first;
    link *last;

public:
    class Iterator{
    public:
        Iterator(const link* pointr) : p(pointr){}

        t& operator*(){
            return p->value;
        }
        const t& operator*() const{
            return p->value;
        }
        const t* operator->() const{
            return p;
        }

        Iterator operator++(){
            p = p->next;
            return *this;
        }
        Iterator operator++(int x){
            Iterator old = *this;
            p = p->next;
            return old;
        }

        bool operator==(const Iterator &other){
            return p == other.p;
        }
        bool operator !=(const Iterator &other){
            return p != other.p;
        }


    private:
        link *p;
    };

    const Iterator begin() const{
        return Iterator(first);
    }
    const Iterator end() const{
        return Iterator(last);
    }
    SLL() : first(new link), last(first){}
    SLL(const SLL<t> &other) : first(new link), last(first){
        Iterator it = other.begin();
        while(it != other.end()){

            ++it;
        }
    }
    SLL<t> operator=(const SLL<t> &other){

    }
    ~SLL();

    void pushFront(const t &element){
        if(first == last){

        }
    }
    void pushBack(const t &element);
    void popFront();
};


int main(){
}
