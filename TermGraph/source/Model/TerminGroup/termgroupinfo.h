#ifndef TERMGROUPINFO_H
#define TERMGROUPINFO_H

#include <stdlib.h>
#include <QDebug>
#include <QUuid>

#include "./termtree.h"
#include "../Termin/paintedterm.h"
#include "../../Helpers/tagprocessor.h"

enum class GroupType {
    freeEdges = -1,
    terms = 0
};

class TermGroupInfo
{
public:
    TermGroupInfo();
    ~TermGroupInfo();

    // Group info
    QUuid getUuid() const;
    GroupType getType() const;

    // Group content
    PaintedTerm::List getAllNodes() const;

    // Edges tools
    EdgesList getAllEdges() const;

    EdgesList getAllEdgesForPainting() const;

protected:
    // Group info methods
    void setGroupUuid(QUuid uuid);
    void setType(const GroupType &value);

    // Group content methods
    void addNodeToList(PaintedTerm* node);
    void clearNodesList();

    // Nodes tools
    PaintedTerm::List getRootNodes() const;
    PaintedTerm::List getInTreeNodes() const;
    PaintedTerm::List getOrphanNodes() const;

    // Edges tools
    void loadEdges();
    void removeCycles();
    void removeExceedEdges();

    // Tree tools
    void setLevels();
    void initTrees();
    QSizeF getAllTreesSize();
    void setNeighbours();

    // Orphans tools
    QSizeF getOrphansSize();

    // Termgroup
    TermTree::List trees;

private:
    // Group info
    QUuid groupUuid;
    GroupType type = GroupType::freeEdges;

    // Group content
    PaintedTerm::List nodesList;
    PaintedTerm::List filterFromNodesList(std::function<bool(PaintedTerm*)> filterCheck) const;
            // Filters nodes from nodesList with lambda

    // Edges & tools
    EdgesList edgesList;
    Edge* addNewEdge(PaintedTerm* node1, PaintedTerm* node2);

    // Edge connections search
    EdgesList searchAllConnections();
    PaintedTerm* getRootNodeForTag(const QString& tag);

    EdgesList suggestConnections();  // TODO: Realize!
};

#endif // TERMGROUPINFO_H
