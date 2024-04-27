#include <string>
#include <iostream>
#include <limits>

class minEditDistance{
    private:
    unsigned int** table;
    const std::string start;
    const std::string end;

    public:
    unsigned int recursion(unsigned int startPos, unsigned int endPos){
        if(startPos == 0 && endPos == 0) return 0;

        //dynamic programming: check table
        unsigned int tableEntry = table[startPos][endPos];
        if(tableEntry != 0) return tableEntry;

        unsigned int min = std::numeric_limits<unsigned int>::max();
        //delete
        if(startPos > 0){
            min = std::min(min, recursion(startPos-1, endPos) + 1);
        }
        //insert
        if(endPos > 0){
            min = std::min(min, recursion(startPos, endPos-1) + 1);
        }
        if(startPos > 0 && endPos > 0){
            //match
            if(start[startPos-1] == end[endPos-1]){
                min = std::min(min, recursion(startPos-1, endPos-1));
            }
            //substitute
            else{
                min = std::min(min, recursion(startPos-1, endPos-1) + 2);
            }
        }

        //dynamic programming: update table
        table[startPos][endPos] = min;

        return min;
    }

    minEditDistance(const std::string &startString, const std::string &endString) : start(startString), end(endString) {
        table = new unsigned int*[start.size()+1];
        for(unsigned int i = 0; i <= start.size(); ++i){
            table[i] = new unsigned int[end.size()+1]{0};
        }
    }

    ~minEditDistance(){
        for(unsigned int i = 0; i <= start.size(); ++i){
            delete[] table[i];
        }
        delete[] table;
    }

};

unsigned int calcMinEditDistance(const std::string &startString, const std::string &endString){
    minEditDistance object(startString, endString);
    return object.recursion(startString.size(), endString.size());
}

int main (int argc, char *argv[]) {
    std::string start, end;
    std::cout << "Enter start string: " << std::flush;
    std::cin >> start;
    std::cout << "Enter end string: " << std::flush;
    std::cin >> end;
    std::cout << "minimum edit distance is: " << calcMinEditDistance(start, end) << std::endl;
    return 0;
}
