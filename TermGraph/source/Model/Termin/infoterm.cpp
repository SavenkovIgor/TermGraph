#include "infoterm.h"

#include <QDebug>

#include "../../Helpers/appstyle.h"
#include "../../Helpers/fonts.h"
#include "../../Helpers/helpstuff.h"
#include "../../Helpers/tagprocessor.h"
#include "../../Helpers/textprocessor.h"

InfoTerm::InfoTerm(const NodeInfoContainer& info, QObject* parent)
    : QObject(parent)
{
    lastRepeatDate = QDate::currentDate();
    repNum = 0;
    atLearn = false;
    this->info = info;
}

QUuid InfoTerm::getUuid() const
{
    return info.uuid;
}

QUuid InfoTerm::getGroupUuid() const
{
    return info.groupUuid;
}

QString InfoTerm::getTerm() const
{
    return info.term;
}

QString InfoTerm::getTermAndDefinition(bool decorated) const
{
    if (decorated) {
        auto ret = "<font color=\"#00a693\">" + getTerm() + "</font>";
        ret += " - это " + TagProcessor::decorateTags(getDefinition());
        return ret;
    }
    return getTerm() + " - это " + getDefinition();
}

QString InfoTerm::getSmallName()
{
    if (smallName.isNull()) {
        smallName = getTerm();

        if (smallName.contains(" ")) {
            // Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

            if (Fonts::getTextMetrics(smallName).width() + 15 > AppStyle::Sizes::baseBlockWidth) {
                // Пытаемся ужать в 2 строки
                smallName = TextProcessor::insertNewLineNearMiddle(smallName);
            }
        }
    }

    return smallName;
}

QSizeF InfoTerm::getNameSize()
{
    if (!nameSize.isValid()) {
        SizesList sizes;
        auto nameParts = getSmallName().split("\n");

        for (auto& part : nameParts)
            sizes.push_back(Fonts::getTextMetrics(part));

        nameSize = HelpStuff::getStackedSize(sizes, Qt::Vertical);
    }

    return nameSize;
}

NodeInfoContainer InfoTerm::infoContainer() const
{
    return info;
}

bool InfoTerm::atLearning()
{
    return atLearn;
}

bool InfoTerm::needRemindToday()
{
    if (lastRepeatDate.addDays(getNextRepeatOffset(repNum)) <= QDate::currentDate()) {
        return true;
    }
    return false;
}

bool InfoTerm::isRemindDateMissed()
{
    if (lastRepeatDate.addDays(getNextRepeatOffset(repNum)) < QDate::currentDate()) {
        return true;
    }
    return false;
}

/*
void TermInfo::setRemind(KnowLevel lvl)
{
    switch (lvl) {

    case TermInfo::dontKnowLvl:
        repNum = qBound(0,repNum-1,10);
        break;

    case TermInfo::remindLvl:
        repNum = qBound(0,repNum+1,10);
        break;

    case TermInfo::wellRemindLvl:
        repNum = qBound(0,repNum+2,10);
        break;
    }

    lastRepeatDate = QDate::currentDate();
    db->nodeTbl->setRemindNum(getUuid(), repNum, QDate::currentDate());
}
*/

int InfoTerm::getRepNum() const
{
    return repNum;
}

// void TermInfo::swithcAtLearnVar()
// {
//     atLearn = !atLearn;
//     db->nodeTbl->setAtLearn(getUuid().toString(),atLearn);
// }

int InfoTerm::getNextRepeatOffset(int lvl)
{
    return getLevelDaysFromBase( lvl + 1 ) - getLevelDaysFromBase( lvl );
}

int InfoTerm::getLevelDaysFromBase(int lvl)
{
    if (lvl <= 0) {
        return 0;  // Варианты 0 и 1
    }

    if (lvl >= 10) {
        return 512;  // 2^9
    }

    lvl--;
    int ret = 1;
    for (int i = 0; i < lvl; i++) {
        ret *= 2;
    }
    return ret;
}

QString InfoTerm::getTermForms() const
{
    return info.termForms;
}

QStringList InfoTerm::termFormsList() const
{
    QStringList ret;
    ret << getTerm();

    QStringList tmp = getTermForms().split(";", QString::SkipEmptyParts);
    for (QString s : tmp) {
        ret << s.simplified();
    }

    ret.removeDuplicates();

    return ret;
}

QString InfoTerm::getDefinition() const
{
    return info.definition;
}

QStringList InfoTerm::getDefinitionTags() const
{
    QString error;
    auto tags = TagProcessor::extractTags(getDefinition());
    // TODO: Fix error work in tagProcessor
    if (!error.isEmpty()) {
        qDebug() << getUuid().toString() << error;
    }
    return tags;
}

QString InfoTerm::getDescription() const
{
    return info.description;
}

QString InfoTerm::getExamples() const
{
    return info.examples;
}

QString InfoTerm::getWikiUrl() const
{
    return info.wikiUrl;
}

QString InfoTerm::getWikiImage() const
{
    return info.wikiImage;
}
