#include "termgraph.h"

TermGraph::TermGraph(QSqlRecord rec) : TermInfo(rec)
{

}

NodesGraphList TermGraph::getRootNodes()
{
    return rootNodes;
}

void TermGraph::addNodeToRootNodes(TermGraph *node)
{
    rootNodes << node;
}

void TermGraph::clearRootNodes()
{
    rootNodes.clear();
}
