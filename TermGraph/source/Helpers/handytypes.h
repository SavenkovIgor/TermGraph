#ifndef HANDYTYPES_H
#define HANDYTYPES_H

#include <vector>
#include <QList>
#include <QUuid>
#include <QSizeF>

class Edge;

using EdgesList = QList<Edge*>;
using UuidList = std::vector<QUuid>;
using SizesList = std::vector<QSizeF>;

#endif // HANDYTYPES_H
