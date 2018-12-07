#include "graphterm.h"

GraphTerm::GraphTerm(QSqlRecord rec) : InfoTerm(rec)
{

}

GraphEdge::List GraphTerm::getUpDownEdges()
{
    GraphEdge::List ret;
    ret << edgesToLeafs;
    ret << edgesToRoots;
    return ret;
}

void GraphTerm::addLayerNeighbours(GraphTerm::List neighboursList)
{
    for (GraphTerm* term : neighboursList) {
        if (term == this) {
            continue;
        }
        neighbourNodes << term;
    }
}

void GraphTerm::clearNeighboursList()
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

void GraphTerm::setLevel(int level)
{
    if (level > paintLevel)
        paintLevel = level;

    for (auto n : getLeafNodes()) {
        if (getGroupUuid() != n->getGroupUuid()) {
            continue;
        }
        n->setLevel(level + 1);
    }
}

int GraphTerm::getPaintLevel() const
{
    return paintLevel;
}

void GraphTerm::getCycleEdge()
{
    cycleSearchFlag = 1;
    for (auto edge : edgesToLeafs) {
        if (edge->needBroke) {
            continue;
        }

        auto leafNode = edge->getOtherSide(this);
        if (leafNode->cycleSearchFlag == 1) {
            edge->needBroke = true;
        } else {
            if (leafNode->cycleSearchFlag == 0) {
                leafNode->getCycleEdge();
            }
        }
    }
    cycleSearchFlag = 2;
}

void GraphTerm::setTreeId(unsigned int treeId)
{
    if (this->treeId == 0) {
        this->treeId = treeId;
    } else {
        return;
    }

    for (auto edge : getAllConnectedEdges()) {
        edge->getOtherSide(this)->setTreeId(treeId);
    }
}

unsigned int GraphTerm::getTreeId() const
{
    return treeId;
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

GraphTerm::List GraphTerm::getUpDownNodes()
{
    List ret;
    ret << rootNodes;
    ret << leafNodes;
    return ret;
}

GraphTerm::List GraphTerm::getRootNodes()
{
    return rootNodes;
}

GraphTerm::List GraphTerm::getLeafNodes()
{
    return leafNodes;
}

GraphTerm::List GraphTerm::getNeighbourNodes()
{
    return neighbourNodes;
}

GraphEdge::List GraphTerm::getEdgesToLeafs()
{
    return edgesToLeafs;
}

GraphEdge::List GraphTerm::getEdgesToRoots()
{
    return edgesToRoots;
}

GraphEdge::List GraphTerm::getAllConnectedEdges()
{
    GraphEdge::List ret;
    ret << edgesToRoots;
    ret << edgesToLeafs;
    return ret;
}

void GraphTerm::removeEdgeToLeafs(GraphEdge *edge)
{
    auto node = edge->getOtherSide(this);
    edgesToLeafs.removeOne(edge);
    leafNodes.removeOne(node);
}

void GraphTerm::removeEdgeToRoots(GraphEdge *edge)
{
    auto node = edge->getOtherSide(this);
    edgesToRoots.removeOne(edge);
    rootNodes.removeOne(node);
}

void GraphTerm::addBrokenEdge(GraphEdge *edge)
{
    if (!brokenEdges.contains(edge)) {
        brokenEdges << edge;
    }
}

GraphEdge::List GraphTerm::getBrokenEdges() const {
    return brokenEdges;
}
