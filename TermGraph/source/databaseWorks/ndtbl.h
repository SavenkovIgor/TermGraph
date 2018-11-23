#ifndef NDTBL_H
#define NDTBL_H

#include "./tblbase.h"

class NodeColumn {
public:
    static const QString uid;  // TODO: Грохнуть
    static const QString longUID;
    static const QString term;
    static const QString termForms;
    static const QString definition;
    static const QString description;
    static const QString examples;
    static const QString wikiRef;
    static const QString wikiImg;
    static const QString termGroup;
    static const QString lastRemind;
    static const QString remindNum;
    static const QString atLearn;
    static const QString lastEdit;
};

class NodeTable : public TblBase
{
public:
    NodeTable(QSqlDatabase* base):
        TblBase("termNode", base)
    {
        initColumn(NodeColumn::uid,         "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        initColumn(NodeColumn::longUID,     "TEXT");
        initColumn(NodeColumn::term,        "TEXT");
        initColumn(NodeColumn::termForms,   "TEXT");
        initColumn(NodeColumn::definition,  "TEXT");
        initColumn(NodeColumn::description, "TEXT");
        initColumn(NodeColumn::examples,    "TEXT");
        initColumn(NodeColumn::wikiRef,     "TEXT");
        initColumn(NodeColumn::wikiImg,     "TEXT");
        initColumn(NodeColumn::termGroup,   "INTEGER DEFAULT '-1'");  // -1 - because uid [0;+8)
        initColumn(NodeColumn::lastRemind,  "TEXT");
        initColumn(NodeColumn::remindNum,   "INTEGER DEFAULT '0'");
        initColumn(NodeColumn::atLearn,     "INTEGER DEFAULT '0'");
        initColumn(NodeColumn::lastEdit,    "TEXT");
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
    void updateLastEdit(const QUuid &uuid);
    bool isUuidExist(QUuid uuid);

    QUuid generateNewUuid();
};

#endif  // NDTBL_H
