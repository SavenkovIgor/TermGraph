#ifndef NODEVERTICALSTACK_H
#define NODEVERTICALSTACK_H

#include <QList>

#include "../Termin/paintedterm.h"
#include "../TerminEdge/edge.h"

class NodeVerticalStack
{
public:

    typedef QList<NodeVerticalStack> List;

    NodeVerticalStack();

    void addTerm(PaintedTerm* term);
    bool hasTerm(PaintedTerm* term);
    bool hasEdge(Edge* edge);

    PaintedTerm::List getAllNodesInStack() const;

private:
    PaintedTerm::List terms;
};

#endif // NODEVERTICALSTACK_H
