#ifndef TERMGRAPH_H
#define TERMGRAPH_H

#include <QList>
#include "terminfo.h"

class TermGraph;

typedef QList<TermGraph*> NodesGraphList;

class TermGraph : public TermInfo
{

public:
    TermGraph(QSqlRecord rec);

public:
//private:  // TODO: Uncomment. Incapsulation!!!

    NodesGraphList rootNodes;
};

#endif // TERMGRAPH_H
