#include "groupsmanager.h"

GroupsManager::GroupsManager(QObject *parent) : QObject(parent)
{

}

void GroupsManager::addNewGroup(QString name, QString comment, int type)
{
    DBAbstract* db = Glb::db;
    if ( db->groupTbl->addGroup( name, comment, type ) )
        groupsListChanged();
    else
        qDebug()<<"Название группы не уникально"; //TODO: Сделать уведомлением!
}

void GroupsManager::deleteGroup(QString name)
{
    DBAbstract* db = Glb::db;
    db->groupTbl->deleteGroup( name );
    groupsListChanged();
}

QStringList GroupsManager::getAllGroupsNames(bool withAllVeiw)
{
    DBAbstract* db = Glb::db;
    QStringList ret = db->groupTbl->getAllGroupsNames();
    if (withAllVeiw)
        ret.push_front("Все группы");
    return ret;
}
