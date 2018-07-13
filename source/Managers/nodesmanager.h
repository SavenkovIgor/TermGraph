#ifndef NODESMANAGER_H
#define NODESMANAGER_H

#include <QObject>

class NodesManager : public QObject
{
    Q_OBJECT
public:
    explicit NodesManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NODESMANAGER_H