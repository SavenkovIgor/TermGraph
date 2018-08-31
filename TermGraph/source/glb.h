#ifndef GLB_H
#define GLB_H

#include <QList>

class TermNode;
class Edge;
class DBAbstract;
class CloudServicesWrapper;

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
#include "./databaseWorks/dbabstract.h"
#include "./databaseWorks/cloudservices.h"

#endif  // GLB_H
