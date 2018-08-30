#include "termgraph.h"

TermGraph::TermGraph(QSqlRecord rec) : TermInfo(rec)
{

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
