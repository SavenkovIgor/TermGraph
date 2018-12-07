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

    void addLayerNeighbours(GraphTerm::List neighboursList);
    void clearNeighboursList();

    bool isRoot();
    bool isOrphan();
    bool isLeaf();

    bool isInTree();

    bool hasConnections();

    NodeType getNodeType();

    void setLevel(int level);
    int getPaintLevel() const;
    GraphEdge* getCycleEdge();

    void setTreeId(unsigned int treeId);
    unsigned int getTreeId() const;

    void addEdgeRef(GraphEdge *edge);
    static bool isInGroupEdge(GraphEdge* edge);
    bool hasConnectionsInGroup();

    // Delete section
    GraphTerm::List getRootNodes();
    GraphTerm::List getLeafNodes();

    GraphTerm::List getNeighbourNodes();
    GraphEdge::List getEdgesToLeafs();
    GraphEdge::List getEdgesToRoots();
    GraphEdge::List getAllConnectedEdges();

    void breakEdge(GraphEdge* breakingEdge);

    int cycleSearchFlag = 0;

    GraphEdge::List getBrokenEdges() const;

private:
    List rootNodes;
    List leafNodes;

    List neighbourNodes;

    GraphEdge::List edgesToOtherGroups;  // Пока никак не используются но будут.
    GraphEdge::List brokenEdges;  // Пока никак не используются но будут.

    GraphEdge::List edgesToLeafs;
    GraphEdge::List edgesToRoots;

    int paintLevel = -1;
    unsigned int treeId = 0; // 0 = no value
};

#endif // TERMGRAPH_H
