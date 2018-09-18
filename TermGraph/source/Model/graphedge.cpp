#include "graphedge.h"

GraphEdge::GraphEdge(GraphTerm *toRoot, GraphTerm *toLeaf)
{
    this->toRoot = toRoot;
    this->toLeaf = toLeaf;
}

bool GraphEdge::hasNode(GraphTerm* node) const
{
    if (node == toRoot || node == toLeaf)
        return true;

    return false;
}

bool GraphEdge::isSameEdge(GraphTerm* rt, GraphTerm* br) const
{
    bool rOk = (toRoot == rt || toRoot == br);
    bool bOk = (toLeaf == rt || toLeaf == br);

    return rOk && bOk;
}

GraphTerm* GraphEdge::getOtherSide(GraphTerm* n) const
{
    if (n == toRoot) {
        return toLeaf;
    } else if (n == toLeaf) {
        return toRoot;
    }

    return nullptr;
}
