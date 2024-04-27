#pragma once

#include <vector>

class graph{
    public:
    using PosV = unsigned int;
    using PosE = unsigned int;
    using Weight = int;

    //get number of outgoing edges of vert
    unsigned int countOutEdges(PosV vert) const;
    //get total number of vertices
    unsigned int countV() const;
    //get total number of edges
    unsigned int countE() const;

    //get i. vertex of all outgoing vertices of vert
    PosV nextV(PosV vert, unsigned int i) const;

    //get weight of edge that starts at startV and ends at endV
    Weight weight(PosV startV, PosV endV) const;

    graph(const std::vector<PosE> &V, const std::vector<std::pair<PosV, Weight>> &E) : _V(V), _E(E){};

    private:
    std::vector<PosE> _V;
    std::vector<std::pair<PosV, Weight>> _E;
};
