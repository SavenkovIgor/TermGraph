#pragma once

#include <QMap>
#include <QString>

#include "source/Model/Termin/paintedterm.h"

class GroupNameCache
{
public:
    GroupNameCache(const PaintedTerm::List& nodes);

    inline PaintedTerm* getIfExist(const QString& name) { return nodeMap.value(name, nullptr); }

private:
    QMap<QString, PaintedTerm*> nodeMap;
};
