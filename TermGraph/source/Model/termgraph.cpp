#include "termgraph.h"

TermGraph::TermGraph(QSqlRecord rec) : TermInfo(rec)
{

}

EdgesList TermGraph::getUpDownEdges()
{
    EdgesList ret;
    ret << edgesToLeafs;
    ret << edgesToRoots;
    return ret;
}

void TermGraph::addToNeighboursList(TermGraph *t)
{
    neighbourNodes << t;
}

void TermGraph::clearNeighbours()
{
    neighbourNodes.clear();
}

bool TermGraph::isRoot() {
    return getNodeType() == NodeType::root;
}

bool TermGraph::isOrphan()
{
    return getNodeType() == NodeType::orphan;
}

bool TermGraph::isLeaf()
{
    NodeType type = getNodeType();
    return type == NodeType::endLeaf || type == NodeType::middleLeaf;
}

bool TermGraph::isInTree()
{
    return !isOrphan();
}

bool TermGraph::hasConnections()
{
    return !( rootNodes.isEmpty() && leafNodes.isEmpty() );
}

NodeType TermGraph::getNodeType()
{
    if ( edgesToRoots.isEmpty()) {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::orphan;      // Оба пустые
        } else {
            return NodeType::root;        // Вниз связей нет, вверх - есть
        }
    } else {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::endLeaf;     // Вниз есть, а вверх - нету
        } else {
            return NodeType::middleLeaf;  // Есть и вверх и вниз
        }
    }
}

NodesGraphList TermGraph::getUpDownNodes()
{
    NodesGraphList ret;
    ret << rootNodes;
    ret << leafNodes;
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
