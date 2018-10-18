#ifndef TERMTREE_H
#define TERMTREE_H

#include <QList>

#include "./nodeverticalstack.h"

class TermTree
{
public:
    TermTree();

private:
    QList<NodeVerticalStack> stacks;
};

#endif // TERMTREE_H
