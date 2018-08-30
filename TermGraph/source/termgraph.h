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


    // Delete section
    NodesGraphList getRootNodes();  // TODO: Maybe delete later
    void addNodeToRootNodes(TermGraph* node);  // TODO: Delete later
    void clearRootNodes();  // TODO: Delete later

    NodesGraphList getLeafNodes();  // TODO: Maybe delete later
    void addNodeToLeafNodes(TermGraph* node);  // TODO: Delete later
    void clearLeafNodes();  // TODO: Delete later

    NodesGraphList getNeighbourNodes();  // TODO: Maybe delete later
    void addNeighbourNode(TermGraph* node);  // TODO: Delete later
    void clearNeighbourNodes();  // TODO: delete later
public:
//private:  // TODO: Uncomment. Incapsulation!!!

private:
    NodesGraphList rootNodes;
    NodesGraphList leafNodes;

    NodesGraphList neighbourNodes;
};

#endif // TERMGRAPH_H
