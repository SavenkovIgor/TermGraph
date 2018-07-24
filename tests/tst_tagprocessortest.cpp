#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "../source/Helpers/tagprocessor.h"

class TagProcessorTest : public QObject
{
    Q_OBJECT

public:
    TagProcessorTest();
    ~TagProcessorTest();

private slots:
    void test_case1();

};

TagProcessorTest::TagProcessorTest()
{

}

TagProcessorTest::~TagProcessorTest()
{

}

void TagProcessorTest::test_case1()
{
//    TagProcessor* processor = new TagProcessor(this);
    QVERIFY(TagProcessor::isPairedBrackets("a}b{c") == false);
    QVERIFY(TagProcessor::isPairedBrackets("a}b}c") == false);
    QVERIFY(TagProcessor::isPairedBrackets("a{b{c") == false);
    QVERIFY(TagProcessor::isPairedBrackets("a{b{c") == false);
    QVERIFY(TagProcessor::isPairedBrackets("{{}{}}}") == false);

    QVERIFY(TagProcessor::isPairedBrackets("{{}{}}") == true);
    QVERIFY(TagProcessor::isPairedBrackets("{{{{{{{{{{}}}}}}}}}}") == true);
    QVERIFY(TagProcessor::isPairedBrackets("{}{}{}{}{}{}") == true);
    /*
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
    */
}

QTEST_APPLESS_MAIN(TagProcessorTest)

#include "tst_tagprocessortest.moc"
