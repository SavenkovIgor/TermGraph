#ifndef GLB_H
#define GLB_H

#include <QList>

#include <memory>

#include "./databaseWorks/dbabstract.h"

class Glb
{
public:
    static std::unique_ptr<DBAbstract> dbPtr;
};

#endif  // GLB_H
