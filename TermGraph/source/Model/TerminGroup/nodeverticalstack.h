#ifndef NODEVERTICALSTACK_H
#define NODEVERTICALSTACK_H

#include <QList>

#include "../Termin/graphicitemterm.h"
#include "../TerminEdge/edge.h"

class NodeVerticalStack
{
public:
    typedef QList<NodeVerticalStack*> List;

    NodeVerticalStack();

    void addTerm(GraphicItemTerm* term);
    bool hasTerm(GraphicItemTerm* term);

    GraphicItemTerm::List getAllNodesInStack() const;

    QSizeF getSize() const;  // Clearly counted value. Ignoring real node positions

    void sortTerms();
    void placeTerms(QPointF centerPoint);

private:
    GraphicItemTerm::List terms;
};

#endif // NODEVERTICALSTACK_H
