#pragma once

#include <QList>

class GraphTerm;

class GraphEdge
{
public:
    using List = QList<GraphEdge*>;

    GraphEdge(GraphTerm* toRoot, GraphTerm* toLeaf);
    virtual ~GraphEdge() = default;

    GraphTerm* getRoot() const { return toRoot; }
    GraphTerm* getLeaf() const { return toLeaf; }

    bool hasNode(GraphTerm* node) const;
    bool isSameEdge(GraphTerm* rt, GraphTerm* br) const;
    GraphTerm* getOtherSide(GraphTerm* n) const;

    bool needPaint = true;  // Flag for placing in paintQueue
    bool needBroke = false;
    bool needCutOut = false;

private:
    GraphTerm* toRoot;
    GraphTerm* toLeaf;
};
