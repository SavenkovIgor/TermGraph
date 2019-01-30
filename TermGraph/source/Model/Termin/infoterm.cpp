#include "infoterm.h"

InfoTerm::InfoTerm(QSqlRecord rec, QObject *parent) :
    QObject(parent)
{
    uuid = QUuid(rec.value(NodeColumn::longUID).toString());
    groupUuid = QUuid(rec.value(NodeColumn::termGroup).toString());

    name = rec.value(NodeColumn::term).toString();
    nameForms = rec.value(NodeColumn::termForms).toString();
    definition = rec.value(NodeColumn::definition).toString();
    description = rec.value(NodeColumn::description).toString();

    examples = rec.value(NodeColumn::examples).toString();
    wikiRef = rec.value(NodeColumn::wikiRef).toString();
    wikiImg = rec.value(NodeColumn::wikiImg).toString();

    lastRepeatDate = QDate::fromString(rec.value(NodeColumn::lastRemind).toString() );
    repNum = rec.value(NodeColumn::remindNum).toInt();
    atLearn = rec.value(NodeColumn::atLearn).toString() == "1";

    nameCompressing();
}

QUuid InfoTerm::getUuid() const
{
    return uuid;
}

QUuid InfoTerm::getGroupUuid() const
{
    return groupUuid;
}

QString InfoTerm::getName() const
{
    return name;
}

QString InfoTerm::getNameAndDefinition(bool decorated) const
{
    if (decorated) {
        return "<font color=\"#00a693\">" + name + "</font>" + " - это " + TagProcessor::decorateTags(definition);
    }
    return name + " - это " + definition;
}

QString InfoTerm::getSmallName() const
{
    return smallName;
}

QSizeF InfoTerm::getNameSize() const
{
    return nameSize;
}

void InfoTerm::nameCompressing()
{
    smallName = name;

    if (name.contains(" ")) {
        // Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

        if (Fonts::getTextMetrics(smallName).width() + 15 > AppStyle::Sizes::baseBlockWidth) {
            // Пытаемся ужать в 2 строки
            int mid = smallName.size()/2;

            for (int i = 0; i < mid; i++) {
                int l = qBound(0, mid-i, mid);
                int r = qBound(mid, mid+i, name.size()-1);

                if (smallName[l] == ' ') {
                    smallName[l] = '\n';
                    break;
                }

                if (smallName[r] == ' ') {
                    smallName[r] = '\n';
                    break;
                }
            }
        }
    }
    // Устанавливаем максимальную ширину сжатого имени
    QStringList lst = smallName.split("\n");
    for (QString s : lst) {
        nameSize.setWidth(qMax(nameSize.width(), Fonts::getTextMetrics(s).width()));
        nameSize.setHeight(nameSize.height() + Fonts::getTextMetrics(s).height());
    }
}

QJsonObject InfoTerm::toJson()
{
    QJsonObject ret;
//    ret.insert("uid",         QJsonValue(uid));
    ret.insert("longUID",     QJsonValue(getUuid().toString()));
    ret.insert("name",        QJsonValue(name));
    ret.insert("nameForms",   QJsonValue(nameForms));
    ret.insert("definition",  QJsonValue(definition));
    ret.insert("description", QJsonValue(description));
    ret.insert("examples",    QJsonValue(examples));
    ret.insert("wikiRef",     QJsonValue());
    ret.insert("wikiImg",     QJsonValue());
    return ret;
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

bool InfoTerm::fromJson(QJsonObject obj) {
    QStringList checkKeys;  // TODO: Переделать на colums
    checkKeys << "uid";
    checkKeys << "longUID";
    checkKeys << "name";
    checkKeys << "nameForms";
    checkKeys << "definition";
    checkKeys << "description";
    checkKeys << "examples";
    checkKeys << "wikiRef";
    checkKeys << "wikiImg";

    for (QString str : checkKeys) {
        if (!obj.contains(str)) {
            qDebug() << "noSuchKey" << str;
            return false;
        }
    }

    this->uuid     = QUuid(obj["longUID"].toString());
    this->name        = obj["name"].toString();
    this->nameForms   = obj["nameForms"].toString();
    this->definition  = obj["definition"].toString();
    this->description = obj["description"].toString();
    this->examples    = obj["examples"].toString();
    this->wikiRef     = obj["wikiRef"].toString();
    this->wikiImg     = obj["wikiImg"].toString();

    return true;
}

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

QString InfoTerm::getNameFormStr() const
{
    return nameForms;
}

QStringList InfoTerm::getNameFormList() const
{
    QStringList ret;
    ret << getName();

    QStringList tmp = getNameFormStr().split(";", QString::SkipEmptyParts);
    for (QString s : tmp) {
        ret << s.simplified();
    }

    ret.removeDuplicates();

    return ret;
}

QString InfoTerm::getDefinition() const
{
    return definition;
}

QStringList InfoTerm::getDefinitionTags() const
{
    QString error;
    auto tags = TagProcessor::extractTags(definition);
    // TODO: Fix error work in tagProcessor
    if (!error.isEmpty()) {
        qDebug() << getUuid().toString() << error;
    }
    return tags;
}

QString InfoTerm::getDescription() const
{
    return description;
}

QString InfoTerm::getExamples() const
{
    return examples;
}

QString InfoTerm::getWikiRef() const
{
    return wikiRef;
}

QString InfoTerm::getWikiImg() const
{
    return wikiImg;
}
