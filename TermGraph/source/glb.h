#ifndef GLB_H
#define GLB_H

#include <QList>

#include <memory>

#include "./databaseWorks/database.h"

class Glb
{
public:
    static std::unique_ptr<Database> dbPtr;
};

#endif  // GLB_H
