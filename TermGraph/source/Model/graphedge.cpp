#include "graphedge.h"

GraphEdge::GraphEdge(TermGraph *toRoot, TermGraph *toLeaf)
{
    this->toRoot = toRoot;
    this->toLeaf = toLeaf;
}

bool GraphEdge::hasNode(TermGraph* node) const
{
    if (node == toRoot || node == toLeaf)
        return true;

    return false;
}

bool GraphEdge::isSameEdge(TermGraph* rt, TermGraph* br) const
{
    bool rOk = (toRoot == rt || toRoot == br);
    bool bOk = (toLeaf == rt || toLeaf == br);

    return rOk && bOk;
}

TermGraph* GraphEdge::getOtherSide(TermGraph* n) const
{
    if (n == toRoot) {
        return toLeaf;
    } else if (n == toLeaf) {
        return toRoot;
    }

    return nullptr;
}
