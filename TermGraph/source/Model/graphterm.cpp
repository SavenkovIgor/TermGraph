#include "graphterm.h"

GraphTerm::GraphTerm(QSqlRecord rec) : TermInfo(rec)
{

}

EdgesGraphList GraphTerm::getUpDownEdges()
{
    EdgesGraphList ret;
    ret << edgesToLeafs;
    ret << edgesToRoots;
    return ret;
}

void GraphTerm::addToNeighboursList(GraphTerm *t)
{
    neighbourNodes << t;
}

void GraphTerm::clearNeighbours()
{
    neighbourNodes.clear();
}

bool GraphTerm::isRoot() {
    return getNodeType() == NodeType::root;
}

bool GraphTerm::isOrphan()
{
    return getNodeType() == NodeType::orphan;
}

bool GraphTerm::isLeaf()
{
    NodeType type = getNodeType();
    return type == NodeType::endLeaf || type == NodeType::middleLeaf;
}

bool GraphTerm::isInTree()
{
    return !isOrphan();
}

bool GraphTerm::hasConnections()
{
    return !( rootNodes.isEmpty() && leafNodes.isEmpty() );
}

NodeType GraphTerm::getNodeType()
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

void GraphTerm::setLevel(int lev)
{
    if (lev > paintLevel)
        paintLevel = lev;

    for (GraphTerm* n : getLeafNodes()) {
        if (getGroupUuid() != n->getGroupUuid()) {
            continue;
        }
        n->setLevel(lev + 1);
    }
}

int GraphTerm::getPaintLevel() const
{
    return paintLevel;
}

void GraphTerm::addEdgeRef(GraphEdge *edge)
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

bool GraphTerm::isInGroupEdge(GraphEdge *edge)
{
    return edge->getRoot()->getGroupUuid() == edge->getLeaf()->getGroupUuid();
}

bool GraphTerm::hasConnectionsInGroup()
{
    for (GraphEdge *e : edgesToLeafs) {
        if (e->getLeaf()->getGroupUuid() == getGroupUuid()) {
            return true;
        }
    }

    for (GraphEdge *e : edgesToRoots) {
        if (e->getLeaf()->getGroupUuid() == getGroupUuid()) {
            return true;
        }
    }
    return false;
}

GraphTerm::NodesGraphList GraphTerm::getUpDownNodes()
{
    NodesGraphList ret;
    ret << rootNodes;
    ret << leafNodes;
    return ret;
}

GraphTerm::NodesGraphList GraphTerm::getRootNodes()
{
    return rootNodes;
}

void GraphTerm::clearRootNodes()
{
    rootNodes.clear();
}

GraphTerm::NodesGraphList GraphTerm::getLeafNodes()
{
    return leafNodes;
}

void GraphTerm::clearLeafNodes()
{
    leafNodes.clear();
}

GraphTerm::NodesGraphList GraphTerm::getNeighbourNodes()
{
    return neighbourNodes;
}

EdgesGraphList GraphTerm::getEdgesToLeafs()
{
    return edgesToLeafs;
}

EdgesGraphList GraphTerm::getEdgesToRoots()
{
    return edgesToRoots;
}
