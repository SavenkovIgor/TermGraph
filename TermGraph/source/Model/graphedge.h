#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

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

private:
    TermGraph* toRoot;
    TermGraph* toLeaf;
};

#endif // GRAPHEDGE_H
