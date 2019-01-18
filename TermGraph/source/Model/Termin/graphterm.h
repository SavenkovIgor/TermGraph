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
    ~GraphTerm() override;

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

    QString getHierarchyDefinition();

    void setLevel(int level);
    int getPaintLevel() const;
    void getCycleEdge();

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

    int cycleSearchFlag = 0;

    void removeEdgeToLeafs(GraphEdge* edge);
    void removeEdgeToRoots(GraphEdge* edge);

    void addBrokenEdge(GraphEdge* edge);
    GraphEdge::List getBrokenEdges() const;

    void checkForExceedEdges();

private:
    List neighbourNodes;

    GraphEdge::List edgesToOtherGroups;  // Пока никак не используются но будут.
    GraphEdge::List brokenEdges;  // Пока никак не используются но будут.

    GraphEdge::List edgesToLeafs;
    GraphEdge::List edgesToRoots;

    int paintLevel = -1;
    unsigned int treeId = 0; // 0 = no value

    bool hasTermInRoots(GraphTerm* term);

    GraphEdge *findLongPathToNode(GraphTerm* node);

    void fillAllParentsList(GraphTerm *searchNode, GraphTerm::List &lst);
};

#endif // TERMGRAPH_H
