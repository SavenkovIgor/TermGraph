#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "../source/Helpers/tagprocessor.h"

class TagProcessorTest : public QObject
{
    Q_OBJECT

public:
    TagProcessorTest() { }

    ~TagProcessorTest() { }

private slots:

    void test_case1_data() {
        QTest::addColumn<QString>("text");
        QTest::addColumn<bool>("result");

        QTest::newRow("wrong0") << "a}b{c" << false;
        QTest::newRow("wrong1") << "a}b}c" << false;
        QTest::newRow("wrong2") << "a{b{c" << false;
        QTest::newRow("wrong3") << "a{b{c" << false;
        QTest::newRow("wrong4") << "{{}{}}}" << false;

        QTest::newRow("ok0") << "{{}{}}" << true;
        QTest::newRow("ok1") << "{{{{{{{{{{}}}}}}}}}}" << true;
        QTest::newRow("ok2") << "{}{}{}{}{}{}" << true;
    }

    void test_case1()
    {
        //    TagProcessor* processor = new TagProcessor(this);
        QFETCH(QString, text);
        QFETCH(bool, result);

        QVERIFY(TagProcessor::isPairedBrackets(text) == result);
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

};

QTEST_APPLESS_MAIN(TagProcessorTest)

#include "tst_tagprocessortest.moc"
