#include "terminfo.h"

DBAbstract* TermInfo::db = nullptr;
qreal TermInfo::baseBlockWidth = 40.0;

TermInfo::TermInfo( QSqlRecord rec, QObject *parent ) :
    QObject(parent)
{
    uid     = rec.value(db->nodeTbl->uid).toInt();
    longUid = rec.value(db->nodeTbl->longUID).toString();
    groupID = rec.value(db->nodeTbl->termGroup).toInt();

    groupType = db->groupTbl->getType( groupID );

    testFunc();

    name        = rec.value( db->nodeTbl->term ).toString();
    nameForms   = rec.value( db->nodeTbl->termForms ).toString();
    root        = false;
    definition  = rec.value( db->nodeTbl->definition ).toString();

    QString error;
    tags = TagProcessor().extractTags(definition,error);
    if( !error.isEmpty() ) {
        qDebug() << uid << error;
    }

    description = rec.value( db->nodeTbl->description ).toString();

    examples    = rec.value( db->nodeTbl->examples ).toString();
    wikiRef     = rec.value( db->nodeTbl->wikiRef ).toString();
    wikiImg     = rec.value( db->nodeTbl->wikiImg ).toString();

    nameCompressing( );
}

int TermInfo::getUid()
{
    return uid;
}

QString TermInfo::getLongUid(bool withPrefix)
{
    if( longUid.isEmpty() )
        return "";

    if( withPrefix )
        return "Uuid " + longUid;
    return longUid;
}

int TermInfo::getGroupID()
{
    return groupID;
}

bool TermInfo::isRoot()
{
//    if( groupType == 0 ) {
//        return getTags().isEmpty(); //If has tags - it is not a root

//    }
//    qDebug()<<"rootCheck";
    return root;
//    return false;
}

QString TermInfo::getName() const
{
    return name;
}

QString TermInfo::getSmallName() const
{
    return smallName;
}

qreal TermInfo::getNameWidth() const
{
    return nameSize.width();
}

qreal TermInfo::getNameHeight() const
{
    return nameSize.height();
}

int TermInfo::getGroupType() const
{
    return groupType;
}

void TermInfo::nameCompressing( )
{
    smallName = name;

    QFontMetricsF mtr = QFontMetricsF( qApp->font() );

    if( name.contains(" ") ) {
        //Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

        if( mtr.boundingRect(smallName).width() + 15 > baseBlockWidth ) { //Пытаемся ужать в 2 строки

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
        nameSize.setWidth( qMax( nameSize.width(), mtr.boundingRect(s).width() ) );
        nameSize.setHeight( nameSize.height() + mtr.boundingRect(s).height() );
    }
}

void TermInfo::testFunc()
{
    return;
    struct test{
        QString     desc;
        QStringList tags;
        bool        err;
    };

    QList<test> tests;
    test t;
    t.desc = "a{bc}";    t.tags = QStringList()<<"bc";       t.err = false;  tests<<t;
    t.desc = "a{bc\\}}"; t.tags = QStringList()<<"bc}";      t.err = false;  tests<<t;
    t.desc = "a{bc}}";   t.tags = QStringList()<<"bc";       t.err = false;  tests<<t;
    t.desc = "}}";       t.tags = QStringList();             t.err = false;  tests<<t;
    t.desc = "{{";       t.tags = QStringList();             t.err = true;   tests<<t;
    t.desc = "a{bc\\}";  t.tags = QStringList()<<"bc\\";     t.err = false;  tests<<t;
    t.desc = "a{b}{c}";  t.tags = QStringList()<<"b"<<"c";   t.err = false;  tests<<t;
    t.desc = "a{b";      t.tags = QStringList();             t.err = true;   tests<<t;
    t.desc = "a{a\\";    t.tags = QStringList();             t.err = true;   tests<<t;
    t.desc = "\\}\\}";   t.tags = QStringList();             t.err = false;  tests<<t;

    qDebug()<<"-----------------------------------------";

    for( test t : tests ) {
        definition = t.desc;
//        QString err;
        qDebug()<<"tag:"<<t.desc<<t.tags<<getTags();
//        if( (err.isEmpty() && t.err) || (!err.isEmpty() && !t.err) )
//            qDebug()<<"some err"<< err.isEmpty();
    }
    qDebug()<<"-----------------------------------------";
}

QJsonObject TermInfo::toJson()
{
    QJsonObject ret;
//    ret.insert("uid",         QJsonValue(uid));
    ret.insert("longUid",     QJsonValue(longUid));
    ret.insert("name",        QJsonValue(name));
    ret.insert("nameForms",   QJsonValue(nameForms));
    ret.insert("definition",  QJsonValue(definition));
    ret.insert("description", QJsonValue(description));
    ret.insert("examples",    QJsonValue(examples));
    ret.insert("wikiRef",     QJsonValue());
    ret.insert("wikiImg",     QJsonValue());
    return ret;
}

bool TermInfo::fromJson(QJsonObject obj) {
    QStringList checkKeys;
    checkKeys<<"uid";
    checkKeys<<"longUid";
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

    this->uid         = obj["uid"].toInt();
    this->longUid     = obj["longUid"].toString();
    this->name        = obj["name"].toString();
    this->nameForms   = obj["nameForms"].toString();
    this->definition  = obj["definition"].toString();
    this->description = obj["description"].toString();
    this->examples    = obj["examples"].toString();
    this->wikiRef     = obj["wikiRef"].toString();
    this->wikiImg     = obj["wikiImg"].toString();

    return true;
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
