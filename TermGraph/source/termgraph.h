#ifndef TERMGRAPH_H
#define TERMGRAPH_H

#include <QList>
#include "terminfo.h"
#include "./Helpers/handytypes.h"

class TermGraph;

typedef QList<TermGraph*> NodesGraphList;

class TermGraph : public TermInfo
{

public:
    TermGraph(QSqlRecord rec);

    EdgesList getConnectedEdges();

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

    EdgesList getEdgesToLeafs();  // TODO: Maybe delete later
    void addEdgeToLeaf(Edge* edge);  // TODO: Delete later

    EdgesList getEdgesToRoots();  // TODO: Maybe delete later
    void addEdgeToRoot(Edge* edge);  // TODO: Delete later
public:
//private:  // TODO: Uncomment. Incapsulation!!!

private:
    NodesGraphList rootNodes;
    NodesGraphList leafNodes;

    NodesGraphList neighbourNodes;

    EdgesList edgesToOtherGroups;  // Пока никак не используются но будут.

    EdgesList edgesToLeafs;
    EdgesList edgesToRoots;
};

#endif // TERMGRAPH_H
