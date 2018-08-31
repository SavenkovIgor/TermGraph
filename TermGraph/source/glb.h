#ifndef GLB_H
#define GLB_H

#include <QList>

#include "./databaseWorks/dbabstract.h"

class TermNode;
class Edge;

// TODO: Remove unneeded type
typedef QList<TermNode*> NodesList;
typedef QList<Edge*> EdgesList;

class Glb
{
public:
    static DBAbstract* db;
};

#include "./termnode.h"
#include "./edge.h"

#endif  // GLB_H
