#ifndef NDTBL_H
#define NDTBL_H

#include <QObject>
#include "tblbase.h"

class NdTbl: public TblBase
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
    const QString techComment = "techComment";
    const QString termGroup   = "termGroup";
    const QString lastRemind  = "lastRemind";
    const QString remindNum   = "remindNum";
    const QString atLearn     = "atLearn"; // на изучении
    const QString lastEdit    = "lastEdit";

    NdTbl(QSqlDatabase *base):
    TblBase("termNode",base)
    {
        initColumn( uid,         "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        initColumn( longUID,     "TEXT");
        initColumn( term,        "TEXT");
        initColumn( termForms,   "TEXT");
        initColumn( definition,  "TEXT");
        initColumn( description, "TEXT");
        initColumn( examples,    "TEXT");
        initColumn( wikiRef,     "TEXT");
        initColumn( wikiImg,     "TEXT");
        initColumn( techComment, "TEXT");
        initColumn( termGroup,   "INTEGER DEFAULT '-1'"); //-1 - because uid [0;+8)
        initColumn( lastRemind,  "TEXT");
        initColumn( remindNum,   "INTEGER DEFAULT '0'");
        initColumn( atLearn,     "INTEGER DEFAULT '0'");
        initColumn( lastEdit,    "TEXT");
    }

    int addNode( QString name );
    QSqlRecord  getNode(int id);

    QList<int>  getAllNodesUid();
    QList<int>  getGroupNodeID(int groupID);

    void setName        ( int uid, QString name);
    void setTForms      ( int uid, QString forms);
    void setDefinition  ( int uid, QString def);
    void setDescription ( int uid, QString desc);
    void setExamples    ( int uid, QString examp);
    void setWikiRef     ( int uid, QString wRef);
    void setWikiImg     ( int uid, QString wImg);
    void setGroup       ( int uid, int group);
    void setAtLearn     ( int uid, bool learn);

    int  getRemindNum( int uid );
    void setRemindNum(int uid, int num, QDate date );
//    void setRemindToday(int uid);

    void deleteNode(int id);

    // normalization

    void normalizeUuid();
private:
    void updateLastEdit(int uid);
    bool isUuidExist(QString longUuid);
};

#endif // NDTBL_H
