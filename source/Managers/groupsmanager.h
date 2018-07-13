#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <QObject>

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // GROUPSMANAGER_H