#ifndef TERMINFO_H
#define TERMINFO_H

#include <QObject>
#include <QDebug>
#include <QSqlRecord>

#include "../../databaseWorks/ndtbl.h"
#include "../../Helpers/appstyle.h"
#include "../../Helpers/tagprocessor.h"
#include "../../Helpers/fonts.h"

// This class contains all "information" part of term, like name, definition and other...
class InfoTerm : public QObject
{
    Q_OBJECT
public:
    enum KnowLevel{
        dontKnowLvl = 0,
        remindLvl,
        wellRemindLvl
    };

    explicit InfoTerm(QSqlRecord rec, QObject *parent = nullptr);
    virtual ~InfoTerm() = default;
//    bool isNull(); //TODO: Realize!

    QUuid getUuid() const;
    QUuid getGroupUuid() const;
    QString getName() const;
    QString getNameAndDefinition(bool decorated = false) const;
    QString getSmallName()  const;
    QString getNameFormStr() const;
    QStringList getNameFormList() const;

    QString getDefinition() const;
    QStringList getDefinitionTags() const;
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
    QSizeF getNameSize()    const;

private:
    void nameCompressing();

    QUuid groupUuid;

    QUuid uuid;
    QString name;
    QString smallName;  // То же самое название но ужатое до 2х строчек"
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

#endif  // TERMINFO_H
