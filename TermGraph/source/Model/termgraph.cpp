#include "termgraph.h"

TermGraph::TermGraph(QSqlRecord rec) : TermInfo(rec)
{

}

EdgesGraphList TermGraph::getUpDownEdges()
{
    EdgesGraphList ret;
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

void TermGraph::addEdgeRef(GraphEdge *edge)
{
    if (edge->getRoot() == this && edge->getLeaf() != this) {  // We are source - connection up
        edgesToLeafs << edge;
        leafNodes << edge->getLeaf();
    }

    if (edge->getLeaf() == this && edge->getRoot() != this) {  // We are acceptor - connection down
        edgesToRoots << edge;
        rootNodes << edge->getRoot();
    }
}

bool TermGraph::isInGroupEdge(GraphEdge *edge)
{
    return edge->getRoot()->getGroupUuid() == edge->getLeaf()->getGroupUuid();
}

TermGraph::NodesGraphList TermGraph::getUpDownNodes()
{
    NodesGraphList ret;
    ret << rootNodes;
    ret << leafNodes;
    return ret;
}

TermGraph::NodesGraphList TermGraph::getRootNodes()
{
    return rootNodes;
}

void TermGraph::clearRootNodes()
{
    rootNodes.clear();
}

TermGraph::NodesGraphList TermGraph::getLeafNodes()
{
    return leafNodes;
}

void TermGraph::clearLeafNodes()
{
    leafNodes.clear();
}

TermGraph::NodesGraphList TermGraph::getNeighbourNodes()
{
    return neighbourNodes;
}

EdgesGraphList TermGraph::getEdgesToLeafs()
{
    return edgesToLeafs;
}

EdgesGraphList TermGraph::getEdgesToRoots()
{
    return edgesToRoots;
}
