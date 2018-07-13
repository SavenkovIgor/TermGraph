#include "groupsmanager.h"

GroupsManager::GroupsManager(QObject *parent) : QObject(parent)
{

}

void GroupsManager::addNewGroup(QString name, QString comment, int type)
{
    if ( Glb::db->groupTbl->addGroup( name, comment, type ) )
        groupsListChanged();
    else
        qDebug()<<"Название группы не уникально"; //TODO: Сделать уведомлением!
}
