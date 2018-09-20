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

class GraphTerm : public TermInfo  // TODO: Rename TermGraph to GraphNode
{

public:

    typedef QList<GraphTerm*> NodesGraphList;

    GraphTerm(QSqlRecord rec);

    NodesGraphList getUpDownNodes();
    GraphEdge::List getUpDownEdges();

    void addToNeighboursList(GraphTerm *t);
    void clearNeighbours();

    bool isRoot();
    bool isOrphan();
    bool isLeaf();

    bool isInTree();

    bool hasConnections();

    NodeType getNodeType();

    void setLevel(int lev);  // TODO: Rename
    int getPaintLevel() const;  // TODO: Rename

    void addEdgeRef(GraphEdge *edge);
    static bool isInGroupEdge(GraphEdge* edge);
    bool hasConnectionsInGroup();

    // Delete section
    GraphTerm::NodesGraphList getRootNodes();  // TODO: Maybe delete later
    void clearRootNodes();  // TODO: Delete later

    GraphTerm::NodesGraphList getLeafNodes();  // TODO: Maybe delete later
    void clearLeafNodes();  // TODO: Delete later

    GraphTerm::NodesGraphList getNeighbourNodes();  // TODO: Maybe delete later
    GraphEdge::List getEdgesToLeafs();  // TODO: Maybe delete later
    GraphEdge::List getEdgesToRoots();  // TODO: Maybe delete later

public:
//private:  // TODO: Uncomment. Incapsulation!!!

private:
    NodesGraphList rootNodes;
    NodesGraphList leafNodes;

    NodesGraphList neighbourNodes;

    GraphEdge::List edgesToOtherGroups;  // Пока никак не используются но будут.

    GraphEdge::List edgesToLeafs;
    GraphEdge::List edgesToRoots;

    int paintLevel = -1;  // TODO: Rename
};

#endif // TERMGRAPH_H
