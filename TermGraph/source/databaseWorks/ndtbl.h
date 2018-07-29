#ifndef NDTBL_H
#define NDTBL_H

#include "./tblbase.h"

class NodeColumn {
public:
//    static const QString uid;  // TODO: Грохнуть
};

class NodeTable : public TblBase
{
public:
    const QString uid         = "uid";
    const QString longUID     = "longUID";
    const QString term        = "term";
    const QString termForms   = "termForms";
    const QString definition  = "definition";
    const QString description = "description";
    const QString examples    = "examples";
    const QString wikiRef     = "wikiRef";
    const QString wikiImg     = "wikiImg";
    const QString termGroup   = "termGroup";
    const QString lastRemind  = "lastRemind";
    const QString remindNum   = "remindNum";
    const QString atLearn     = "atLearn";  // на изучении
    const QString lastEdit    = "lastEdit";

    NodeTable(QSqlDatabase* base):
        TblBase("termNode", base)
    {
        initColumn(uid,         "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        initColumn(longUID,     "TEXT");
        initColumn(term,        "TEXT");
        initColumn(termForms,   "TEXT");
        initColumn(definition,  "TEXT");
        initColumn(description, "TEXT");
        initColumn(examples,    "TEXT");
        initColumn(wikiRef,     "TEXT");
        initColumn(wikiImg,     "TEXT");
        initColumn(termGroup,   "INTEGER DEFAULT '-1'");  // -1 - because uid [0;+8)
        initColumn(lastRemind,  "TEXT");
        initColumn(remindNum,   "INTEGER DEFAULT '0'");
        initColumn(atLearn,     "INTEGER DEFAULT '0'");
        initColumn(lastEdit,    "TEXT");
    }

    QUuid addNode(QString name);
    QUuid addNode(QUuid uuid, QString name);
    QSqlRecord getNodeSqlRecord(QUuid uuid);

    bool isNodeWithUuidExist(QUuid uuid);

    QList<QUuid> getAllNodesUuids();
    QList<QUuid> getAllNodesUuidsInGroup(QUuid groupUuid);

    QDateTime getLastEdit(QUuid uuid);

    void setName(QUuid uuid, QString name);
    void setWordForms(QUuid uuid, QString forms);
    void setDefinition(QUuid uuid, QString definition);
    void setDescription(QUuid uuid, QString description);
    void setExamples(QUuid uuid, QString example);
    void setWikiRef(QUuid uuid, QString wikiRef);
    void setWikiImg(QUuid uuid, QString wikiImage);
    void setAtLearn(QUuid uuid, bool learn);
    void setGroup(QUuid nodeUuid, QUuid groupUuid);

    int  getRemindNum(QUuid uuid);
    void setRemindNum(QUuid uuid, int num, QDate date);
//    void setRemindToday(int uid);

    void deleteNode(QUuid uuid);

private:
    void setFieldUpdateLastEdit(QString columnName, QUuid uuid, QString val);
    void updateLastEdit(QUuid uuid);
    bool isUuidExist(QUuid uuid);

    QUuid generateNewUuid();
};

#endif  // NDTBL_H
