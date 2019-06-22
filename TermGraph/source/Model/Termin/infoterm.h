#ifndef TERMINFO_H
#define TERMINFO_H

#include <QObject>
#include <QDebug>
#include <QSqlRecord>

#include "nodeinfocontainer.h"
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

    explicit InfoTerm(const NodeInfoContainer& info, QObject* parent = nullptr);
    virtual ~InfoTerm() = default;
//    bool isNull(); //TODO: Realize!

    QUuid getUuid() const;
    QUuid getGroupUuid() const;
    QString getTerm() const;
    QString getTermAndDefinition(bool decorated = false) const;
    QString getSmallName();  // Lazy
    QString getTermForms() const;
    QStringList termFormsList() const;

    QString getDefinition() const;
    QStringList getDefinitionTags() const;
    QString getDescription() const;
    QString getExamples() const;
    QString getWikiUrl() const;
    QString getWikiImage() const;

    NodeInfoContainer infoContainer() const;

    // Learning
    bool atLearning();
//    void swithcAtLearnVar();

    bool needRemindToday();
    bool isRemindDateMissed();
//    void setRemind(KnowLevel lvl);

    int getRepNum() const;

protected:
    QSizeF getNameSize();  // Lazy

private:
    NodeInfoContainer info;

    QString smallName = QString();  // То же самое название но ужатое до 2х строчек"
    QSizeF  nameSize = QSizeF();

    QDate lastRepeatDate;
    int repNum = -1;
    bool atLearn;

    // Learning
    int getNextRepeatOffset(int lvl);
    int getLevelDaysFromBase(int lvl);
};

#endif  // TERMINFO_H
