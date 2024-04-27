#include <vector>
#include <iostream>
#include "pqueue.hh"
#include "graph.hh"
#include "shortest_path.hh"

int main (int argc, char *argv[]) {
    std::vector<graph::PosE> V {0, 2, 3, 5, 5, 7};
    std::vector<std::pair<graph::PosV, graph::Weight>> E {{1,2}, {3,10}, {2,3}, {4,5}, {3,1}, {2,1}, {0,2}};

    std::vector<graph::PosV> parentPointers;
    std::vector<graph::Weight> distance;

    graph G(V, E);
    dijkstra(G, 0, parentPointers, distance);
    for(int i = 0; i < parentPointers.size(); ++i){
        std::cout << i << ". Vertex: parent " << parentPointers[i] << ", distance " << distance[i] << "\n";
    }

    return 0;
}
