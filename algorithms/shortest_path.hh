#pragma once

#include "pqueue.hh"
#include "graph.hh"
#include <vector>

void dijkstra(graph &G, graph::PosV startV, std::vector<graph::PosV> &parent, std::vector<graph::Weight> &distance);
