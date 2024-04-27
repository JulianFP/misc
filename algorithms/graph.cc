#include "graph.hh"
#include <limits>

unsigned int graph::countOutEdges(PosV vert) const{
    //return difference between position of vert and next PosV
    return _V[vert + 1] - _V[vert];
}

unsigned int graph::countV() const{
    return _V.size() - 1;
}

unsigned int graph::countE() const{
    return _E.size();
}

graph::PosV graph::nextV(PosV vert, unsigned int i) const{
    //get position of first edge of vert in _E
    PosE firstEdge = _V[vert];

    //return PosV of this edge + i offset
    return _E[firstEdge + i].first;
}

graph::Weight graph::weight(PosV startV, PosV endV) const{
    //iterate from first edge of startV to (excluding) fist edge of PosV after startV
    for(PosE i = _V[startV]; i < _V[startV + 1]; ++i){
        //if this edge points to endV then return its weight
        if(_E[i].first == endV) return _E[i].second;
    }

    //if this edge isn't found: return (practically) infinity
    //(this case should be avoided)
    return std::numeric_limits<Weight>::max();
}
