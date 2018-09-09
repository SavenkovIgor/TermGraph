#ifndef TERMGRAPH_H
#define TERMGRAPH_H

#include <QList>
#include "./terminfo.h"
#include "./graphedge.h"
#include "../Helpers/handytypes.h"

enum class NodeType {
    orphan,
    root,
    endLeaf,
    middleLeaf
};

class TermGraph : public TermInfo
{

public:

    typedef QList<TermGraph*> NodesGraphList;

    TermGraph(QSqlRecord rec);

    NodesGraphList getUpDownNodes();
    EdgesGraphList getUpDownEdges();

    void addToNeighboursList(TermGraph *t);
    void clearNeighbours();

    bool isRoot();
    bool isOrphan();
    bool isLeaf();

    bool isInTree();

    bool hasConnections();

    NodeType getNodeType();

    void addEdgeRef(GraphEdge *edge);
    static bool isInGroupEdge(GraphEdge* edge);

    // Delete section
    TermGraph::NodesGraphList getRootNodes();  // TODO: Maybe delete later
    void clearRootNodes();  // TODO: Delete later

    TermGraph::NodesGraphList getLeafNodes();  // TODO: Maybe delete later
    void clearLeafNodes();  // TODO: Delete later

    TermGraph::NodesGraphList getNeighbourNodes();  // TODO: Maybe delete later
    EdgesGraphList getEdgesToLeafs();  // TODO: Maybe delete later
    EdgesGraphList getEdgesToRoots();  // TODO: Maybe delete later

public:
//private:  // TODO: Uncomment. Incapsulation!!!

private:
    NodesGraphList rootNodes;
    NodesGraphList leafNodes;

    NodesGraphList neighbourNodes;

    EdgesGraphList edgesToOtherGroups;  // Пока никак не используются но будут.

    EdgesGraphList edgesToLeafs;
    EdgesGraphList edgesToRoots;
};

#endif // TERMGRAPH_H
