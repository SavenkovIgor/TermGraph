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

    PaintedTerm::List getAllNodesInStack() const;

    QSizeF getSize() const;  // Clearly counted value. Ignoring real node positions

private:
    PaintedTerm::List terms;
};

#endif // NODEVERTICALSTACK_H
