#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QList>

class GraphTerm;

class GraphEdge
{
public:
    GraphEdge(GraphTerm* toRoot, GraphTerm* toLeaf);

    GraphTerm* getRoot() const { return toRoot; }
    GraphTerm* getLeaf() const { return toLeaf; }

    bool hasNode(GraphTerm* node) const;
    bool isSameEdge(GraphTerm* rt, GraphTerm* br) const;
    GraphTerm* getOtherSide(GraphTerm* n) const;

    virtual ~GraphEdge() = default;

private:
    GraphTerm* toRoot;
    GraphTerm* toLeaf;
};

typedef QList<GraphEdge*> EdgesGraphList;

#endif // GRAPHEDGE_H
