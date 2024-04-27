#include <vector>
#include <limits>
#include "shortest_path.hh"

void dijkstra(graph &G, graph::PosV startV, std::vector<graph::PosV> &parent, std::vector<graph::Weight> &distance){
    //initialize priority queue
    BinaryHeap<graph::Weight, graph::PosV> Q;
    Q.insert(0, startV);

    //initialize parent array
    parent.resize(G.countV());
    parent[startV] = startV;

    //initialize distance array (stores current known distance of vertex to startV)
    distance.resize(G.countV(), std::numeric_limits<graph::Weight>::max());
    distance[startV] = 0;

    // initialize visited array which stores weither vertex has been inserted into Q at some point
    std::vector<bool> visited(G.countV(), false);
    visited[startV] = true;

    while(Q.size() != 0){
        //get next vertex from Q
        std::pair<graph::Weight, graph::PosV> current = Q.deleteMin();
        graph::PosV currentV = current.second;

        //iterate through all vertices that are reachable from currentV
        int count = G.countOutEdges(currentV);
        for(unsigned int i = 0; i < count; ++i){
            //get data of this vertex
            graph::PosV nextV = G.nextV(currentV, i);
            graph::Weight newD = distance[currentV] + G.weight(currentV, nextV);

            //check if new path is shorter than old one
            if(newD < distance[nextV]){
                //update its distance
                distance[nextV] = newD;

                //set its parent pointer to currentV 
                parent[nextV] = currentV;

                if(visited[nextV]){
                    //if that vertex was already inserted into Q then decrease its key
                    Q.decreaseKey(nextV, newD);
                }
                else{
                    //if not then insert it now
                    Q.insert(newD, nextV);
                    //...and update visited array
                    visited[nextV] = true;
                }
            }
        }
    }
}
