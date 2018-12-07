#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QList>

class GraphTerm;

class GraphEdge
{
public:
    typedef QList<GraphEdge*> List;

    GraphEdge(GraphTerm* toRoot, GraphTerm* toLeaf);

    GraphTerm* getRoot() const { return toRoot; }
    GraphTerm* getLeaf() const { return toLeaf; }

    bool hasNode(GraphTerm* node) const;
    bool isSameEdge(GraphTerm* rt, GraphTerm* br) const;
    GraphTerm* getOtherSide(GraphTerm* n) const;

    virtual ~GraphEdge() = default;

    bool needPaint = true;  // Flag for placing in paintQueue
    bool needBroke = false;

private:
    GraphTerm* toRoot;
    GraphTerm* toLeaf;
};

#endif  // GRAPHEDGE_H
