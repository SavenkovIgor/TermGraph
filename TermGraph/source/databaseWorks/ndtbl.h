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

    void setName(const QUuid& uuid, const QString& name);
    void setWordForms(const QUuid& uuid, const QString& forms);
    void setDefinition(const QUuid& uuid, const QString& definition);
    void setDescription(const QUuid& uuid, const QString& description);
    void setExamples(const QUuid& uuid, const QString& example);
    void setWikiRef(const QUuid& uuid, const QString& wikiRef);
    void setWikiImg(const QUuid &uuid, const QString &wikiImage);
    void setAtLearn(const QUuid &uuid, const bool &learn);
    void setGroup(const QUuid &nodeUuid, const QUuid &groupUuid);

    int  getRemindNum(QUuid uuid);
    void setRemindNum(QUuid uuid, int num, QDate date);
//    void setRemindToday(int uid);

    void deleteNode(QUuid uuid);

private:
    void setFieldUpdateLastEdit(const QString &columnName, const QUuid &uuid, const QString &val);
    void updateLastEdit(const QUuid &uuid);
    bool isUuidExist(QUuid uuid);

    QUuid generateNewUuid();
};

#endif  // NDTBL_H
