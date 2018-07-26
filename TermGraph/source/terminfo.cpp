#include "terminfo.h"

DBAbstract* TermInfo::db = nullptr;
qreal TermInfo::baseBlockWidth = 40.0;

TermInfo::TermInfo( QSqlRecord rec, QObject *parent ) :
    QObject(parent)
{
    uuid = QUuid(rec.value(db->nodeTbl->longUID).toString());
    groupID = rec.value(db->nodeTbl->termGroup).toInt();

    groupUuid = QUuid(rec.value(db->nodeTbl->termGroup).toString());
    groupType = db->groupTbl->getType( groupUuid );

    name = rec.value( db->nodeTbl->term ).toString();
    nameForms = rec.value( db->nodeTbl->termForms ).toString();
    definition = rec.value( db->nodeTbl->definition ).toString();
    description = rec.value( db->nodeTbl->description ).toString();

    examples = rec.value( db->nodeTbl->examples ).toString();
    wikiRef = rec.value( db->nodeTbl->wikiRef ).toString();
    wikiImg = rec.value( db->nodeTbl->wikiImg ).toString();

    lastRepeatDate = QDate::fromString(rec.value( db->nodeTbl->lastRemind).toString() );
    repNum = rec.value( db->nodeTbl->remindNum).toInt();
    atLearn = rec.value( db->nodeTbl->atLearn).toString() == "1";

    nameCompressing();
}

QUuid TermInfo::getUuid() const
{
    return uuid;
}

int TermInfo::getGroupID()
{
    return groupID;
}

QUuid TermInfo::getGroupUuid()
{
    return groupUuid;
}

QString TermInfo::getName() const
{
    return name;
}

QString TermInfo::getSmallName() const
{
    return smallName;
}

QSizeF TermInfo::getNameSize() const
{
    return nameSize;
}

int TermInfo::getGroupType() const
{
    return groupType;
}

void TermInfo::nameCompressing( )
{
    smallName = name;

    if( name.contains(" ") ) {
        //Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

        if( Fonts::getTextMetrics(smallName).width() + 15 > baseBlockWidth ) { //Пытаемся ужать в 2 строки

            int mid = smallName.size()/2;

            for(int i=0;i<mid;i++) {
                int l = qBound(0,mid-i,mid);
                int r = qBound(mid,mid+i,name.size()-1);

                if(smallName[l] == ' ') {
                    smallName[l] = '\n';
                    break;
                }

                if(smallName[r] == ' ') {
                    smallName[r] = '\n';
                    break;
                }
            }
        }
    }
    //Устанавливаем максимальную ширину сжатого имени
    QStringList lst = smallName.split("\n");
    for( QString s : lst ) {
        nameSize.setWidth( qMax( nameSize.width(), Fonts::getTextMetrics(s).width() ) );
        nameSize.setHeight( nameSize.height() + Fonts::getTextMetrics(s).height() );
    }
}

QJsonObject TermInfo::toJson()
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

bool TermInfo::atLearning()
{
    return atLearn;
}

bool TermInfo::needRemindToday()
{
    if( lastRepeatDate.addDays(getNextRepeatOffset(repNum)) <= QDate::currentDate() )
        return true;
    return false;
}

bool TermInfo::isRemindDateMissed()
{
    if( lastRepeatDate.addDays(getNextRepeatOffset(repNum)) < QDate::currentDate() )
        return true;
    return false;
}

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

int TermInfo::getRepNum() const
{
    return repNum;
}

void TermInfo::swithcAtLearnVar()
{
    atLearn = !atLearn;
    db->nodeTbl->setAtLearn(getUuid().toString(),atLearn);
}

bool TermInfo::fromJson(QJsonObject obj) {
    QStringList checkKeys;
    checkKeys<<"uid";
    checkKeys<<"longUID";
    checkKeys<<"name";
    checkKeys<<"nameForms";
    checkKeys<<"definition";
    checkKeys<<"description";
    checkKeys<<"examples";
    checkKeys<<"wikiRef";
    checkKeys<<"wikiImg";

    for( QString str : checkKeys ) {
        if (!obj.contains(str)) {
            qDebug()<<"noSuchKey"<<str;
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

int TermInfo::getNextRepeatOffset(int lvl)
{
    return getLevelDaysFromBase( lvl + 1 ) - getLevelDaysFromBase( lvl );
}

int TermInfo::getLevelDaysFromBase(int lvl)
{
    if( lvl <= 0 )
        return 0; //Варианты 0 и 1

    if( lvl >= 10 )
        return 512; //2^9

    lvl--;
    int ret = 1;
    for(int i = 0; i < lvl; i++) {
        ret *= 2;
    }
    return ret;
}

QString TermInfo::getNameFormStr() const
{
    return nameForms;
}

QStringList TermInfo::getNameFormList() const
{
    QStringList ret;
    ret<<getName();

    QStringList tmp = getNameFormStr().split(";",QString::SkipEmptyParts);
    for( QString s : tmp )
        ret << s.simplified();

    ret.removeDuplicates();

    return ret;
}

QString TermInfo::getDefinition() const
{
    return definition;
}

QStringList TermInfo::getTags() const
{
    QString error;
    auto tags = TagProcessor::extractTags(definition, error);
    if( !error.isEmpty() ) {
        qDebug() << getUuid().toString() << error;
    }
    return tags;
}

QString TermInfo::getDescription() const
{
    return description;
}

QString TermInfo::getExamples() const
{
    return examples;
}

QString TermInfo::getWikiRef() const
{
    return wikiRef;
}

QString TermInfo::getWikiImg() const
{
    return wikiImg;
}
