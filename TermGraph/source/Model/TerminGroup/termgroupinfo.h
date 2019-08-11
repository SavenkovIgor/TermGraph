#pragma once

#include <stdlib.h>
#include <QDebug>
#include <QUuid>

#include "source/Model/TerminGroup/termtree.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

class TermGroupInfo
{
public:
    TermGroupInfo() = delete;
    TermGroupInfo(const GroupInfoContainer& info);
    ~TermGroupInfo();

    // Group info
    QUuid getUuid() const;
    QString getName() const;

    // Group content
    PaintedTerm::List getAllNodes() const;

    // Edges tools
    EdgesList getAllEdges() const;
    EdgesList getAllEdgesForPainting() const;

protected:
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
    GroupInfoContainer info;

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
