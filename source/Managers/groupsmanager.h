#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <QObject>
#include "../glb.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(QObject *parent = nullptr);

signals:
    void groupsListChanged();

public slots:

    void addNewGroup(QString name, QString comment, int type);
    void deleteGroup(QString name);
};

#endif // GROUPSMANAGER_H
