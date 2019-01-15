#include <QtTest>
#include <QCoreApplication>
#include <QDebug>

// add necessary includes here
#include "../../../TermGraph/source/Helpers/tagprocessor.h"

class TagProcessorTest : public QObject
{
    Q_OBJECT

public:
    TagProcessorTest() { }
    ~TagProcessorTest() { }

private slots:

    void pairBrackets_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<bool>("result");

        QTest::newRow("ok0") << "{{}{}}" << true;
        QTest::newRow("ok1") << "{{{{{{{{{{}}}}}}}}}}" << true;
        QTest::newRow("ok2") << "{}{}{}{}{}{}" << true;
        QTest::newRow("ok3") << "" << true;

        QTest::newRow("wrong0") << "a}b{c" << false;
        QTest::newRow("wrong1") << "a}b}c" << false;
        QTest::newRow("wrong2") << "a{b{c" << false;
        QTest::newRow("wrong3") << "{" << false;
        QTest::newRow("wrong3") << "}" << false;
        QTest::newRow("wrong3") << "}{" << false;
        QTest::newRow("wrong4") << "{{}{}}}" << false;
    }

    void pairBrackets()
    {
        QFETCH(QString, text);
        QFETCH(bool, result);
        QVERIFY(TagProcessor::isPairedBrackets(text) == result);
    }

    void bracketsDepth_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<int>("result");

        QTest::newRow("case0") << "{}" << 1;
        QTest::newRow("case1") << "a{a}a{a}a{a}a" << 1;
        QTest::newRow("case2") << "  " << 0;
        QTest::newRow("case3") << "{{{}}}  " << 3;
        QTest::newRow("case4") << "a{}  {{{}}} {}a" << 3;
        QTest::newRow("case5") << "{" << -1;
        QTest::newRow("case6") << "}" << -1;
        QTest::newRow("case7") << "aa}" << -1;
        QTest::newRow("case8") << "}aa" << -1;
        QTest::newRow("case9") << "}}" << -1;
        QTest::newRow("case10") << "{{" << -1;
    }

    void bracketsDepth()
    {
        QFETCH(QString, text);
        QFETCH(int, result);
        QVERIFY(TagProcessor::getMaxDepthOfNestedBrackets(text) == result);
    }

    void tagExtraction_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<QStringList>("tags");

        QTest::newRow("ok0") << "a{bc}" << (QStringList() << "bc");
        QTest::newRow("ok1") << "a{a}{b}" << (QStringList() << "a" << "b");
        QTest::newRow("ok2") << "a{bc}{bc}" << (QStringList() << "bc");
        QTest::newRow("ok3") << "a{bc\\}" << (QStringList() << "bc\\");

        QTest::newRow("error0") << "a{bc\\}}" << QStringList();
        QTest::newRow("error1") << "a{bc}}" << QStringList();
        QTest::newRow("error2") << "}}" << QStringList();
        QTest::newRow("error3") << "{{" << QStringList();
        QTest::newRow("error4") << "a{b" << QStringList();
        QTest::newRow("error5") << "a{a\\" << QStringList();
        QTest::newRow("error6") << "\\}\\}" << QStringList();
    }

    void tagExtraction()
    {
        QFETCH(QString, src);
        QFETCH(QStringList, tags);

        QVERIFY(TagProcessor::extractTags(src) == tags);
    }

    void wordBorder_data()
    {
        QTest::addColumn<int>("dir");
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startPosition");
        QTest::addColumn<int>("targetPosition");

        // Предполагается что -1 - влево, 1 - вправо
        // В пустой строке поиск и в лево и вправо должен вовращать 0
        QTest::newRow("case0") << -1 << "" << 0 << 0;
        QTest::newRow("case1") << 1  << "" << 0 << 0;

        QTest::newRow("case2") << -1 << " " << 0 << 0;
        QTest::newRow("case3") << 1  << " " << 0 << 0;

        QTest::newRow("case4") << -1 << " " << 1 << 1;
        QTest::newRow("case5") << 1  << " " << 1 << 1;

        QTest::newRow("case6") << -1 << " a" << 1 << 1;
        QTest::newRow("case7") << 1  << " a" << 1 << 2;

        QTest::newRow("case8") << -1 << "ab" << 0 << 0;
        QTest::newRow("case9") << 1  << "ab" << 0 << 2;

        QTest::newRow("case10") << -1 << "ab" << 1 << 0;
        QTest::newRow("case11") << 1  << "ab" << 1 << 2;

        QTest::newRow("case12") << -1 << "ab" << 2 << 0;
        QTest::newRow("case13") << 1  << "ab" << 2 << 2;

        QTest::newRow("case14") << -1 << " ab " << 0 << 0;
        QTest::newRow("case15") << 1  << " ab " << 0 << 0;

        QTest::newRow("case16") << -1 << " ab " << 1 << 1;
        QTest::newRow("case17") << 1  << " ab " << 1 << 3;

        QTest::newRow("case18") << -1 << " ab, " << 1 << 1;
        QTest::newRow("case19") << 1  << " ab, " << 1 << 3;

        QTest::newRow("case20") << -1 << " a a " << 3 << 3;
        QTest::newRow("case21") << 1  << " a a " << 3 << 4;

        QTest::newRow("case22") << -1 << " a a " << 4 << 3;
        QTest::newRow("case23") << 1  << " a a " << 4 << 4;
    }

    void wordBorder()
    {
        QFETCH(int, dir);
        QFETCH(QString, src);
        QFETCH(int, startPosition);
        QFETCH(int, targetPosition);

        auto left = TagProcessor::SearchDirection::left;
        auto right = TagProcessor::SearchDirection::right;

        if (dir == 1) {
            QVERIFY(TagProcessor::searchWordBorder(right, src, startPosition) == targetPosition);
        }

        if (dir == -1) {
            QVERIFY(TagProcessor::searchWordBorder(left, src, startPosition) == targetPosition);
        }
    }

    void nearestBracket_data()
    {
        QTest::addColumn<int>("dir");
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startPosition");
        QTest::addColumn<QChar>("bracket");

        QChar leftBracket = '{';
        QChar rightBracket = '}';

        // Предполагается что -1 - влево, 1 - вправо
        // В пустой строке поиск и в лево и вправо должен вовращать пустой символ
        QTest::newRow("case0") << -1 << "" << 0 << QChar();
        QTest::newRow("case1") << 1  << "" << 0 << QChar();

        QTest::newRow("case2") << -1 << "{" << 0 << QChar();
        QTest::newRow("case3") << 1  << "{" << 0 << leftBracket;

        QTest::newRow("case4") << -1 << "{" << 1 << leftBracket;
        QTest::newRow("case5") << 1  << "{" << 1 << QChar();

        QTest::newRow("case6") << -1 << "}" << 0 << QChar();
        QTest::newRow("case7") << 1  << "}" << 0 << rightBracket;

        QTest::newRow("case8") << -1 << "}" << 1 << rightBracket;
        QTest::newRow("case9") << 1  << "}" << 1 << QChar();

        QTest::newRow("case10") << -1 << " { asdf } " << 2 << leftBracket;
        QTest::newRow("case11") << 1  << " { asdf } " << 2 << rightBracket;

        QTest::newRow("case12") << -1 << " } asdf { " << 2 << rightBracket;
        QTest::newRow("case13") << 1  << " } asdf { " << 2 << leftBracket;
    }

    void nearestBracket()
    {
        QFETCH(int, dir);
        QFETCH(QString, src);
        QFETCH(int, startPosition);
        QFETCH(QChar, bracket);

        auto left = TagProcessor::SearchDirection::left;
        auto right = TagProcessor::SearchDirection::right;

        if (dir == 1) {
            QVERIFY(TagProcessor::getNearesBracket(right, src, startPosition) == bracket);
        }

        if (dir == -1) {
            QVERIFY(TagProcessor::getNearesBracket(left, src, startPosition) == bracket);
        }
    }

    void inTag_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<bool>("result");

        QTest::newRow("case0") << "{}" << 0 << false;
        QTest::newRow("case1") << "{}" << 1 << true;
        QTest::newRow("case2") << "{}" << 2 << false;
        QTest::newRow("case3") << " " << 0 << false;
        QTest::newRow("case4") << " " << 1 << false;
        QTest::newRow("case5") << "{  }" << 2 << true;
        QTest::newRow("case6") << "a{a}a" << 2 << true;
        QTest::newRow("case7") << "a{a}a" << 3 << true;
        QTest::newRow("case8") << "} a {" << 1 << false;
        QTest::newRow("case9") << "}{" << 0 << false;
        QTest::newRow("case10") << "}{" << 1 << false;
        QTest::newRow("case11") << "}{" << 2 << false;
        QTest::newRow("case12") << "}" << 0 << false;
        QTest::newRow("case13") << "}" << 1 << false;
        QTest::newRow("case14") << "{" << 0 << false;
        QTest::newRow("case15") << "{" << 1 << false;
    }

    void inTag()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(bool, result);

        QVERIFY(TagProcessor::isInsideTag(src, cursorPosition) == result);
    }

    void addTags_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << "" << 0 << "{}";
        QTest::newRow("case0") << " " << 0 << "{} ";
        QTest::newRow("case0") << " " << 1 << " {}";
        QTest::newRow("case1") << "a" << 0 << "{a}";
        QTest::newRow("case2") << "a" << 1 << "{a}";
        QTest::newRow("case3") << " a " << 0 << "{} a ";
        QTest::newRow("case4") << " a " << 1 << " {a} ";
        QTest::newRow("case5") << " a " << 2 << " {a} ";
        QTest::newRow("case6") << " a " << 3 << " a {}";
        QTest::newRow("case7") << " {a} " << 1 << " {}{a} ";
        QTest::newRow("case8") << " {a} " << 2 << " {a} ";
        QTest::newRow("case9") << " {a} " << 3 << " {a} ";
        QTest::newRow("case10") << " {a} " << 4 << " {a}{} ";
        QTest::newRow("case11") << " aa bb " << 1 << " {aa} bb ";
        QTest::newRow("case12") << " aaaaa, " << 1 << " {aaaaa}, ";
    }

    void addTags()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(QString, result);

        TagProcessor* proc = new TagProcessor();

        QVERIFY(proc->addTagInPosition(cursorPosition, src) == result);
    }

    void extendRight_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << "" << 0 << "";
        QTest::newRow("case1") << " " << 0 << " ";
        QTest::newRow("case2") << " " << 1 << " ";
        QTest::newRow("case3") << "a" << 0 << "a";
        QTest::newRow("case4") << "a" << 1 << "a";
        QTest::newRow("case5") << " a " << 0 << " a ";
        QTest::newRow("case6") << " a " << 1 << " a ";
        QTest::newRow("case7") << " a " << 2 << " a ";
        QTest::newRow("case8") << " a " << 3 << " a ";
        QTest::newRow("case9") << " a a " << 0 << " a a ";
        QTest::newRow("case10") << " a a " << 1 << " a a ";
        QTest::newRow("case11") << " a a " << 2 << " a a ";
        QTest::newRow("case12") << " a a " << 3 << " a a ";
        QTest::newRow("case13") << " a a " << 4 << " a a ";
        QTest::newRow("case14") << " a a " << 5 << " a a ";
        QTest::newRow("case15") << " {a}" << 2 << " {a}";
        QTest::newRow("case16") << " {a}" << 3 << " {a}";
        QTest::newRow("case17") << " {a} " << 2 << " {a} ";
        QTest::newRow("case18") << " {a} " << 3 << " {a} ";
        QTest::newRow("case19") << " {a} a " << 2 << " {a a} ";
        QTest::newRow("case20") << " {a} a " << 3 << " {a a} ";
        QTest::newRow("case21") << " {a } a " << 2 << " {a  a} ";
        QTest::newRow("case22") << " {a } a " << 3 << " {a  a} ";
        QTest::newRow("case23") << " {a } a " << 4 << " {a  a} ";
        QTest::newRow("case24") << " {a}a " << 2 << " {aa} ";
        QTest::newRow("case25") << " {a}a " << 3 << " {aa} ";
        QTest::newRow("case26") << " {aa} bb " << 2 << " {aa bb} ";
        QTest::newRow("case27") << " aaaaa, " << 1 << " aaaaa, ";
        QTest::newRow("case28") << "{a}" << 0 << "{a}";
        QTest::newRow("case29") << "{a}" << 1 << "{a}";
        QTest::newRow("case30") << "{a}" << 2 << "{a}";
        QTest::newRow("case31") << "{a}" << 3 << "{a}";
        QTest::newRow("case32") << "{a}a" << 0 << "{a}a";
        QTest::newRow("case33") << "{a}a" << 1 << "{aa}";
        QTest::newRow("case34") << "{a}a" << 2 << "{aa}";
        QTest::newRow("case35") << "{a}a" << 3 << "{a}a";
        QTest::newRow("case36") << "{a}a" << 4 << "{a}a";
        QTest::newRow("case37") << "{a} {a}" << 1 << "{a} {a}";
        QTest::newRow("case38") << "{a} {a}" << 2 << "{a} {a}";
        QTest::newRow("case39") << "{a}{a}" << 1 << "{a}{a}";
        QTest::newRow("case40") << "{a}{a}" << 2 << "{a}{a}";
    }

    void extendRight()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(QString, result);

        TagProcessor* proc = new TagProcessor();

        QVERIFY(proc->expandRight(cursorPosition, src) == result);
    }
};

QTEST_APPLESS_MAIN(TagProcessorTest)

#include "tst_tagprocessortest.moc"

