/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include <QtTest>
#include <QCoreApplication>
#include <QDebug>

// add necessary includes here
#include "source/Helpers/tagutils.h"

class TagUtilsTest : public QObject
{
    Q_OBJECT

public:
    TagUtilsTest()           = default;
    ~TagUtilsTest() override = default;

private slots:
    void inTag_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<bool>("result");

        QTest::newRow("case0") << "" << -1 << false;
        QTest::newRow("case1") << "" << 0 << false;
        QTest::newRow("case2") << "" << 1 << false;
        QTest::newRow("case3") << "{}" << -1 << false;
        QTest::newRow("case4") << "{}" << 0 << false;
        QTest::newRow("case5") << "{}" << 1 << true;
        QTest::newRow("case6") << "{}" << 2 << false;
        QTest::newRow("case7") << "{}" << 3 << false;
        QTest::newRow("case8") << " " << -1 << false;
        QTest::newRow("case9") << " " << 0 << false;
        QTest::newRow("case10") << " " << 1 << false;
        QTest::newRow("case11") << " " << 2 << false;
        QTest::newRow("case12") << "{  }" << 2 << true;
        QTest::newRow("case13") << "a{a}a" << 2 << true;
        QTest::newRow("case14") << "a{a}a" << 3 << true;
        QTest::newRow("case15") << "} a {" << 1 << false;
        QTest::newRow("case16") << "}{" << 0 << false;
        QTest::newRow("case17") << "}{" << 1 << false;
        QTest::newRow("case18") << "}{" << 2 << false;
        QTest::newRow("case19") << "}" << 0 << false;
        QTest::newRow("case20") << "}" << 1 << false;
        QTest::newRow("case21") << "{" << 0 << false;
        QTest::newRow("case22") << "{" << 1 << false;
    }

    void inTag()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(bool, result);

        QVERIFY(TagUtils::isInsideTag(src, cursorPosition) == result);
    }

    void addTags_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << "" << -1 << "";
        QTest::newRow("case0") << "" << 0 << "{}";
        QTest::newRow("case0") << "" << 1 << "";
        QTest::newRow("case0") << " " << 0 << "{} ";
        QTest::newRow("case0") << " " << 1 << " {}";
        QTest::newRow("case1") << "a" << -1 << "a";
        QTest::newRow("case1") << "a" << 0 << "{a}";
        QTest::newRow("case2") << "a" << 1 << "{a}";
        QTest::newRow("case1") << "a" << 2 << "a";
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

        QVERIFY(TagUtils::addTag(src, cursorPosition) == result);
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
        QTest::newRow("case41") << " {a} aaaaa, " << 2 << " {a aaaaa}, ";
    }

    void extendRight()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(QString, result);

        QVERIFY(TagUtils::expandTagRight(src, cursorPosition) == result);
    }

    void removeTags_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << "{}" << 0 << "{}";
        QTest::newRow("case1") << "{}" << 1 << "";
        QTest::newRow("case2") << "{}" << 2 << "{}";
        QTest::newRow("case3") << "a{}" << 2 << "a";
        QTest::newRow("case4") << "a{}" << 1 << "a{}";
        QTest::newRow("case5") << "{}a" << 0 << "{}a";
        QTest::newRow("case6") << "{}a" << 1 << "a";
        QTest::newRow("case7") << " {} " << 2 << "  ";
        QTest::newRow("case8") << " {a } " << 2 << " a  ";
        QTest::newRow("case9") << " {a } " << 3 << " a  ";
        QTest::newRow("case10") << " {a } " << 4 << " a  ";
        QTest::newRow("case11") << " a " << 0 << " a ";
        QTest::newRow("case12") << " a " << 1 << " a ";
        QTest::newRow("case13") << " a " << 2 << " a ";
        QTest::newRow("case14") << " a " << 3 << " a ";
        QTest::newRow("case15") << " {abcd} " << 2 << " abcd ";
        QTest::newRow("case16") << " {abcd abcd} " << 2 << " abcd abcd ";
    }

    void removeTags()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(QString, result);

        QVERIFY(TagUtils::removeTag(src, cursorPosition) == result);
    }

    void tagExtraction_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<QStringList>("tags");

        QTest::newRow("ok0") << "a{bc}" << (QStringList() << "bc");
        QTest::newRow("ok1") << "a{a}{b}"
                             << (QStringList() << "a"
                                               << "b");
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

        QVERIFY(TagUtils::extractTags(src) == tags);

        QBENCHMARK(TagUtils::extractTags(src));
    }

    void decorateTags_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << ""
                               << "";
        QTest::newRow("case1") << " asdf "
                               << " asdf ";
        QTest::newRow("case2") << " {asdf} "
                               << " <font color=\"#6d9a28\">asdf</font> ";
    }

    void decorateTags()
    {
        QFETCH(QString, src);
        QFETCH(QString, result);

        QVERIFY(TagUtils::replaceTags(src, "<font color=\"#6d9a28\">", "</font>") == result);
    }

    void tagLengthSuitTerm_data() {}
    void tagLengthSuitTerm() {}

    void getLevDistance_data() {}
    void getLevDistance() {}

    void getDistanceBetweenTagAndTerm_data() {}
    void getDistanceBetweenTagAndTerm() {}

    void wordBorder_data()
    {
        QTest::addColumn<int>("dir");
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startFrom");
        QTest::addColumn<int>("targetPosition");

        // Предполагается что -1 - влево, 1 - вправо
        // В пустой строке поиск и в лево и вправо должен вовращать 0
        QTest::newRow("case0") << -1 << "" << 0 << 0;
        QTest::newRow("case1") << 1 << "" << 0 << 0;

        QTest::newRow("case2") << -1 << " " << 0 << 0;
        QTest::newRow("case3") << 1 << " " << 0 << 0;

        QTest::newRow("case4") << -1 << " " << 1 << 1;
        QTest::newRow("case5") << 1 << " " << 1 << 1;

        QTest::newRow("case6") << -1 << " a" << 1 << 1;
        QTest::newRow("case7") << 1 << " a" << 1 << 2;

        QTest::newRow("case8") << -1 << "ab" << 0 << 0;
        QTest::newRow("case9") << 1 << "ab" << 0 << 2;

        QTest::newRow("case10") << -1 << "ab" << 1 << 0;
        QTest::newRow("case11") << 1 << "ab" << 1 << 2;

        QTest::newRow("case12") << -1 << "ab" << 2 << 0;
        QTest::newRow("case13") << 1 << "ab" << 2 << 2;

        QTest::newRow("case14") << -1 << " ab " << 0 << 0;
        QTest::newRow("case15") << 1 << " ab " << 0 << 0;

        QTest::newRow("case16") << -1 << " ab " << 1 << 1;
        QTest::newRow("case17") << 1 << " ab " << 1 << 3;

        QTest::newRow("case18") << -1 << " ab, " << 1 << 1;
        QTest::newRow("case19") << 1 << " ab, " << 1 << 3;

        QTest::newRow("case20") << -1 << " a a " << 3 << 3;
        QTest::newRow("case21") << 1 << " a a " << 3 << 4;

        QTest::newRow("case22") << -1 << " a a " << 4 << 3;
        QTest::newRow("case23") << 1 << " a a " << 4 << 4;
    }

    void wordBorder()
    {
        QFETCH(int, dir);
        QFETCH(QString, src);
        QFETCH(int, startFrom);
        QFETCH(int, targetPosition);

        auto left  = TagUtils::SearchDirection::left;
        auto right = TagUtils::SearchDirection::right;

        if (dir == 1)
            QVERIFY(TagUtils::findWordBorder(src, startFrom, right) == targetPosition);

        if (dir == -1)
            QVERIFY(TagUtils::findWordBorder(src, startFrom, left) == targetPosition);
    }

    void wordsCount_data() {}
    void wordsCount() {}

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
        QVERIFY(TagUtils::isPairedBrackets(text) == result);
    }

    void nearestBracket_data()
    {
        QTest::addColumn<int>("dir");
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startFrom");
        QTest::addColumn<QChar>("bracket");

        QChar leftBracket  = '{';
        QChar rightBracket = '}';

        // Предполагается что -1 - влево, 1 - вправо
        // В пустой строке поиск и в лево и вправо должен вовращать пустой символ
        QTest::newRow("case0") << -1 << "" << 0 << QChar();
        QTest::newRow("case1") << 1 << "" << 0 << QChar();

        QTest::newRow("case2") << -1 << "{" << 0 << QChar();
        QTest::newRow("case3") << 1 << "{" << 0 << leftBracket;

        QTest::newRow("case4") << -1 << "{" << 1 << leftBracket;
        QTest::newRow("case5") << 1 << "{" << 1 << QChar();

        QTest::newRow("case6") << -1 << "}" << 0 << QChar();
        QTest::newRow("case7") << 1 << "}" << 0 << rightBracket;

        QTest::newRow("case8") << -1 << "}" << 1 << rightBracket;
        QTest::newRow("case9") << 1 << "}" << 1 << QChar();

        QTest::newRow("case10") << -1 << " { asdf } " << 2 << leftBracket;
        QTest::newRow("case11") << 1 << " { asdf } " << 2 << rightBracket;

        QTest::newRow("case12") << -1 << " } asdf { " << 2 << rightBracket;
        QTest::newRow("case13") << 1 << " } asdf { " << 2 << leftBracket;
    }

    void nearestBracket()
    {
        QFETCH(int, dir);
        QFETCH(QString, src);
        QFETCH(int, startFrom);
        QFETCH(QChar, bracket);

        auto left  = TagUtils::SearchDirection::left;
        auto right = TagUtils::SearchDirection::right;

        if (dir == 1) {
            QVERIFY(TagUtils::getBracket(src, startFrom, right) == bracket);
        }

        if (dir == -1) {
            QVERIFY(TagUtils::getBracket(src, startFrom, left) == bracket);
        }
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
        QVERIFY(TagUtils::getBracketsDepth(text) == result);
    }

    void validCursor_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<bool>("result");

        QTest::newRow("case0") << "" << -1 << false;
        QTest::newRow("case1") << "" << 0 << true;
        QTest::newRow("case2") << "" << 1 << false;
        QTest::newRow("case3") << "a" << -1 << false;
        QTest::newRow("case4") << "a" << 0 << true;
        QTest::newRow("case5") << "a" << 1 << true;
        QTest::newRow("case6") << "a" << 2 << false;
    }

    void validCursor()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(bool, result);

        QVERIFY(TagUtils::isValidCursor(src, cursorPosition) == result);
    }

    void findCursor_data() {}
    void findCursor() {}

    void leftSearch_data() {}
    void leftSearch() {}

    void rightSearch_data() {}
    void rightSearch() {}
};

QTEST_APPLESS_MAIN(TagUtilsTest)

#include "tagutilstest.moc"
