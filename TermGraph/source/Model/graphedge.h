#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QList>

class TermGraph;

class GraphEdge
{
public:
    GraphEdge(TermGraph* toRoot, TermGraph* toLeaf);

    TermGraph* getRoot() const { return toRoot; }
    TermGraph* getLeaf() const { return toLeaf; }

    bool hasNode(TermGraph* node) const;
    bool isSameEdge(TermGraph* rt, TermGraph* br) const;
    TermGraph* getOtherSide(TermGraph* n) const;

    virtual ~GraphEdge() = default;

private:
    TermGraph* toRoot;
    TermGraph* toLeaf;
};

typedef QList<GraphEdge*> EdgesGraphList;

#endif // GRAPHEDGE_H
