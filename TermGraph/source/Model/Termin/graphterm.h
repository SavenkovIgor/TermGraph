#ifndef TERMGRAPH_H
#define TERMGRAPH_H

#include <QList>
#include "./infoterm.h"
#include "../TerminEdge/graphedge.h"
#include "../../Helpers/handytypes.h"

enum class NodeType {
    orphan,
    root,
    endLeaf,
    middleLeaf
};

// This class contains all "graph" part of term
// like edges, place in graph hierarchy and so on
class GraphTerm : public InfoTerm
{

public:
    typedef QList<GraphTerm*> List;

    GraphTerm(QSqlRecord rec);

    List getUpDownNodes();
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
    GraphTerm::List getRootNodes();  // TODO: Maybe delete later
    void clearRootNodes();  // TODO: Delete later

    GraphTerm::List getLeafNodes();  // TODO: Maybe delete later
    void clearLeafNodes();  // TODO: Delete later

    GraphTerm::List getNeighbourNodes();  // TODO: Maybe delete later
    GraphEdge::List getEdgesToLeafs();  // TODO: Maybe delete later
    GraphEdge::List getEdgesToRoots();  // TODO: Maybe delete later

public:
//private:  // TODO: Uncomment. Incapsulation!!!

private:
    List rootNodes;
    List leafNodes;

    List neighbourNodes;

    GraphEdge::List edgesToOtherGroups;  // Пока никак не используются но будут.

    GraphEdge::List edgesToLeafs;
    GraphEdge::List edgesToRoots;

    int paintLevel = -1;  // TODO: Rename
};

#endif // TERMGRAPH_H
