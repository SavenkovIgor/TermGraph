#ifndef TERMINFO_H
#define TERMINFO_H

#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QSqlRecord>

#include "databaseWorks/dbabstract.h"
#include "Helpers/tagprocessor.h"
#include "Helpers/fonts.h"

class TermInfo : public QObject
{
    Q_OBJECT
public:

    enum KnowLevel{
        dontKnowLvl = 0,
        remindLvl,
        wellRemindLvl
    };

    explicit TermInfo(QSqlRecord rec, QObject *parent = nullptr);

//    bool isNull(); //TODO: Realize!

    int getGroupID(); //TODO: Delete
    QUuid getGroupUuid();

    QUuid getUuid() const;
    QString getName() const;
    QString getNameFormStr() const;
    QStringList getNameFormList() const;

    QString getDefinition() const;
    QStringList getTags() const;
    QString getDescription() const;
    QString getExamples() const;
    QString getWikiRef() const;
    QString getWikiImg() const;

    QJsonObject toJson();

    // Learning
    bool atLearning();
//    void swithcAtLearnVar();

    bool needRemindToday();
    bool isRemindDateMissed();
//    void setRemind(KnowLevel lvl);

    int getRepNum() const;

protected:
    QString getSmallName()  const;
    QSizeF getNameSize()    const;

private:
    //Блок не менее...
    static qreal baseBlockWidth; //TODO: Вынести в Sizes

    void nameCompressing();

    int groupID = -1;
    QUuid groupUuid;

    QUuid uuid;
    QString name;
    QString smallName; //То же самое название но ужатое до 2х строчек"
    QSizeF  nameSize;
    QString nameForms;

    QString definition;
    QString description;
    QString examples;
    QString wikiRef;
    QString wikiImg;

    QDate lastRepeatDate;
    int repNum = -1;
    bool atLearn;

    bool fromJson(QJsonObject obj);

    // Learning
    int getNextRepeatOffset(int lvl);
    int getLevelDaysFromBase(int lvl);
};

#endif // TERMINFO_H
