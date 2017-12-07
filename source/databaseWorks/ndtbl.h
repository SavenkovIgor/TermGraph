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
    const QString isRoot      = "isRoot";
    const QString termGroup   = "termGroup";
    const QString lastRemind  = "lastRemind";
    const QString remindNum   = "remindNum";
    const QString atLearn     = "atLearn"; // на изучении
    const QString lastEdit    = "lastEdit";

    NdTbl(QSqlDatabase *base):
    TblBase("termNode",base)
    {
        addCol( uid,         "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        addCol( longUID,     "TEXT");
        addCol( term,        "TEXT");
        addCol( termForms,   "TEXT");
        addCol( definition,  "TEXT");
        addCol( description, "TEXT");
        addCol( examples,    "TEXT");
        addCol( wikiRef,     "TEXT");
        addCol( wikiImg,     "TEXT");
        addCol( techComment, "TEXT");
        addCol( isRoot,      "INTEGER DEFAULT '0'");  //0 - because false
        addCol( termGroup,   "INTEGER DEFAULT '-1'"); //-1 - because uid [0;+8)
        addCol( lastRemind,  "TEXT");
        addCol( remindNum,   "INTEGER DEFAULT '0'");
        addCol( atLearn,     "INTEGER DEFAULT '0'");
        addCol( lastEdit,    "TEXT");
    }

    int addNode( QString name );
    QSqlRecord  getNode(int id);

    QList<int>  getAllNodesList();
    QList<int>  getGroupNodeID(int groupID);

    void setName        ( int uid, QString name);
    void setTForms      ( int uid, QString forms);
    void setDefinition  ( int uid, QString def);
    void setDescription ( int uid, QString desc);
    void setExamples    ( int uid, QString examp);
    void setWikiRef     ( int uid, QString wRef);
    void setWikiImg     ( int uid, QString wImg);
    void setIsRoot      ( int uid, bool isRoot);
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
