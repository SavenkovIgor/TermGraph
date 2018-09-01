#include "termgraph.h"

TermGraph::TermGraph(QSqlRecord rec) : TermInfo(rec)
{

}

EdgesList TermGraph::getConnectedEdges()
{
    EdgesList ret;
    ret << edgesToLeafs;
    ret << edgesToRoots;
    return ret;
}

NodesGraphList TermGraph::getRootNodes()
{
    return rootNodes;
}

void TermGraph::addNodeToRootNodes(TermGraph *node)
{
    rootNodes << node;
}

void TermGraph::clearRootNodes()
{
    rootNodes.clear();
}

NodesGraphList TermGraph::getLeafNodes()
{
    return leafNodes;
}

void TermGraph::addNodeToLeafNodes(TermGraph *node)
{
    leafNodes << node;
}

void TermGraph::clearLeafNodes()
{
    leafNodes.clear();
}

NodesGraphList TermGraph::getNeighbourNodes()
{
    return neighbourNodes;
}

void TermGraph::addNeighbourNode(TermGraph *node)
{
    neighbourNodes << node;
}

void TermGraph::clearNeighbourNodes()
{
    neighbourNodes.clear();
}

EdgesList TermGraph::getEdgesToLeafs()
{
    return edgesToLeafs;
}

void TermGraph::addEdgeToLeaf(Edge *edge)
{
    edgesToLeafs << edge;
}

EdgesList TermGraph::getEdgesToRoots()
{
    return edgesToRoots;
}

void TermGraph::addEdgeToRoot(Edge *edge)
{
    edgesToRoots << edge;
}
