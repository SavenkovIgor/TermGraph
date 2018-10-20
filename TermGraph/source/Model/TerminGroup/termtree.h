#ifndef TERMTREE_H
#define TERMTREE_H

#include <QList>
#include <QGraphicsItem>
#include <stdlib.h>

#include "../Termin/graphicitemterm.h"
#include "./nodeverticalstack.h"

class TermTree
{
public:

    typedef QList<TermTree> List;

    TermTree();

    void addTerm(PaintedTerm* term);
    bool hasTerm(PaintedTerm* term);

    void setSceneParent(QGraphicsItem* parent);

private:
    NodeVerticalStack::List stacks;

    PaintedTerm::List getAllNodesInTree() const;
};

#endif // TERMTREE_H
