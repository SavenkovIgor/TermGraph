#ifndef TERMGROUPINFO_H
#define TERMGROUPINFO_H

#include <stdlib.h>
#include <QDebug>
#include <QUuid>

#include "../Termin/graphicitemterm.h"
#include "../../Helpers/tagprocessor.h"

enum class GroupType {
    freeEdges = -1,
    terms = 0
};

class TermGroupInfo
{
public:
    TermGroupInfo();

    // Group info
    QUuid getUuid() const;
    GroupType getType() const;

    // Group content
    GraphicItemTerm::List getAllNodes() const;

    // Edges tools
    EdgesList getAllEdges() const;

protected:
    // Group info methods
    void setGroupUuid(QUuid uuid);
    void setType(const GroupType &value);

    // Group content methods
    void addNodeToList(GraphicItemTerm *node);
    void clearNodesList();

    // Nodes tools
    GraphicItemTerm::List getRootNodes() const;
    GraphicItemTerm::List getInTreeNodes() const;
    GraphicItemTerm::List getOrphanNodes() const;

    // Edges tools
    void loadEdges();

    // Tree tools
    void setLayers();
    int getLayersCount() const;  // TODO: Delete
    // Returns layer list in asc order
    QList<int> getLayerNumbersList(bool withRoot = true) const;
    GraphicItemTerm::List getNodesInLayer(int layer) const;
    QSizeF getTheoreticalTreeSize();

    void setNeighbours();

    // NodeStack tools
    QSizeF getVerticalStackedSize(GraphicItemTerm::List lst) const;
    qreal getMaxHeightInAllLevels() const;

    // Orphans tools
    QSizeF getOrphansSize();

private:
    // Group info
    QUuid groupUuid;
    GroupType type = GroupType::freeEdges;

    // Group content
    GraphicItemTerm::List nodesList;
    GraphicItemTerm::List filterFromNodesList(std::function<bool(GraphicItemTerm*)> filterCheck) const;
            // Filters nodes from nodesList with lambda

    // Edges & tools
    EdgesList edgesList;

    Edge* addNewEdge(GraphicItemTerm* node1, GraphicItemTerm* node2);

    // Edge connections search
    EdgesList searchConnections();
    EdgesList suggestConnections();  // TODO: Realize!
};

#endif // TERMGROUPINFO_H
