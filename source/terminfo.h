#ifndef TERMINFO_H
#define TERMINFO_H

#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QFontMetricsF>
#include <QSqlRecord>

#include "databaseWorks/dbabstract.h"
#include "Helpers/tagprocessor.h"

class TermInfo : public QObject
{
    Q_OBJECT
public:
    explicit TermInfo(QSqlRecord rec, QObject *parent = 0);
    TermInfo(QJsonObject jsonObject, QObject *parent = 0);

    int     getUid();
    QString getLongUid(bool withPrefix = false);
    int     getGroupID();

    bool isRoot();

    QString     getName()           const;
    QString     getNameFormStr()    const;
    QStringList getNameFormList()   const;

    QString     getDefinition()     const;
    QStringList getTags()           const;
    QString     getDescription()    const;
    QString     getExamples()       const;
    QString     getWikiRef()        const;
    QString     getWikiImg()        const;

    QJsonObject toJson();

    static DBAbstract *db;

protected:
    QString getSmallName()  const;
    qreal getNameWidth()    const;
    qreal getNameHeight()   const;

    int   getGroupType()    const;

private:
    //Блок не менее...
    static qreal baseBlockWidth;

    void nameCompressing();

    int uid = -1;
    int groupID = -1;
    int groupType = -1;

    QString longUid;

    bool root = false;

    QString name;
    QString smallName; //То же самое название но ужатое до 2х строчек"
    QSizeF  nameSize;
    QString nameForms;

    QString definition;
    QString description;
    QString examples;
    QString wikiRef;
    QString wikiImg;

    void testFunc();

    bool fromJson(QJsonObject obj);
};

#endif // TERMINFO_H
