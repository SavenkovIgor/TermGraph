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

#include <limits>
#include <vector>

#include <QtTest>
#include <QCoreApplication>
#include <QDebug>

// add necessary includes here
#include "source/Helpers/tagutils.h"

class TagUtilsTest : public QObject
{
    Q_OBJECT

    using Idxs = std::vector<int>;  // Short for Indexes

public:
    TagUtilsTest()           = default;
    ~TagUtilsTest() override = default;

private slots:
    void inTag_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<Idxs>("cursorPositions");
        QTest::addColumn<bool>("result");

        // clang-format off
        QTest::newRow("case0") << "{}"    << Idxs {1}           << true;
        QTest::newRow("case1") << "{  }"  << Idxs {2}           << true;
        QTest::newRow("case2") << "a{a}a" << Idxs {2, 3}        << true;

        QTest::newRow("case3") << ""      << Idxs {-1, 0, 1}    << false;
        QTest::newRow("case4") << " "     << Idxs {-1, 0, 1, 2} << false;
        QTest::newRow("case5") << "{"     << Idxs {0, 1}        << false;
        QTest::newRow("case6") << "}"     << Idxs {0, 1}        << false;
        QTest::newRow("case7") << "{}"    << Idxs {-1, 0, 2, 3} << false;
        QTest::newRow("case8") << "}{"    << Idxs {0, 1, 2}     << false;
        QTest::newRow("case9") << "} a {" << Idxs {1}           << false;
        // clang-format on
    }

    void inTag()
    {
        QFETCH(QString, src);
        QFETCH(Idxs, cursorPositions);
        QFETCH(bool, result);

        for (auto cursor : cursorPositions) {
            QVERIFY2(TagUtils::isInsideTag(src, cursor) == result, failCursor(cursor));
        }
    }

    void addTags_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<Idxs>("cursorPositions");
        QTest::addColumn<QString>("result");

        // clang-format off
        QTest::newRow("case0")  << ""           << Idxs {-1, 1} << "";
        QTest::newRow("case1")  << ""           << Idxs {0}     << "{}";
        QTest::newRow("case2")  << " "          << Idxs {0}     << "{} ";
        QTest::newRow("case3")  << " "          << Idxs {1}     << " {}";
        QTest::newRow("case4")  << "a"          << Idxs {-1, 2} << "a";
        QTest::newRow("case5")  << "a"          << Idxs {0, 1}  << "{a}";
        QTest::newRow("case6")  << " a "        << Idxs {1, 2}  << " {a} ";
        QTest::newRow("case7")  << " a "        << Idxs {0}     << "{} a ";
        QTest::newRow("case8")  << " a "        << Idxs {3}     << " a {}";
        QTest::newRow("case9")  << " {a} "      << Idxs {2, 3}  << " {a} ";
        QTest::newRow("case10") << " {a} "      << Idxs {1}     << " {}{a} ";
        QTest::newRow("case11") << " {a} "      << Idxs {4}     << " {a}{} ";
        QTest::newRow("case12") << " aa bb "    << Idxs {1}     << " {aa} bb ";
        QTest::newRow("case13") << " aa   bb "  << Idxs {7}     << " aa   {bb} ";
        QTest::newRow("case14") << " aaaaa, "   << Idxs {1}     << " {aaaaa}, ";
        // clang-format on
    }

    void addTags()
    {
        QFETCH(QString, src);
        QFETCH(Idxs, cursorPositions);
        QFETCH(QString, result);

        for (auto cursor : cursorPositions) {
            QVERIFY2(TagUtils::addTag(src, cursor) == result, failCursor(cursor));
        }
    }

    void extendRight_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<Idxs>("cursorPositions");
        QTest::addColumn<QString>("result");

        // clang-format off
        QTest::newRow("case0")  << ""             << Idxs {0}                << "";
        QTest::newRow("case1")  << " "            << Idxs {0, 1}             << " ";
        QTest::newRow("case2")  << "a"            << Idxs {0, 1}             << "a";
        QTest::newRow("case3")  << " a "          << Idxs {0, 1, 2, 3}       << " a ";
        QTest::newRow("case4")  << "{a}"          << Idxs {0, 1, 2, 3}       << "{a}";
        QTest::newRow("case5")  << " {a}"         << Idxs {2, 3}             << " {a}";
        QTest::newRow("case6")  << "{a}a"         << Idxs {1, 2}             << "{aa}";
        QTest::newRow("case7")  << "{a}a"         << Idxs {0, 3, 4}          << "{a}a";
        QTest::newRow("case8")  << " a a "        << Idxs {0, 1, 2, 3, 4, 5} << " a a ";
        QTest::newRow("case9")  << " {a} "        << Idxs {2, 3}             << " {a} ";
        QTest::newRow("case10") << " {a}a "       << Idxs {2, 3}             << " {aa} ";
        QTest::newRow("case11") << "{a}{a}"       << Idxs {1, 2}             << "{a}{a}";
        QTest::newRow("case12") << " {a} a "      << Idxs {2, 3}             << " {a a} ";
        QTest::newRow("case13") << "{a} {a}"      << Idxs {1, 2}             << "{a} {a}";
        QTest::newRow("case14") << " aaaaa, "     << Idxs {1}                << " aaaaa, ";
        QTest::newRow("case15") << " {a } a "     << Idxs {2, 3, 4}          << " {a  a} ";
        QTest::newRow("case16") << " {aa} bb "    << Idxs {2}                << " {aa bb} ";
        QTest::newRow("case17") << " {a} aaaaa, " << Idxs {2}                << " {a aaaaa}, ";
        // clang-format on
    }

    void extendRight()
    {
        QFETCH(QString, src);
        QFETCH(Idxs, cursorPositions);
        QFETCH(QString, result);

        for (auto cursor : cursorPositions) {
            QVERIFY2(TagUtils::expandTagRight(src, cursor) == result, failCursor(cursor));
        }
    }

    void removeTags_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<Idxs>("cursorPositions");
        QTest::addColumn<QString>("result");

        // clang-format off
        QTest::newRow("case0")  << "{}"            << Idxs {0, 2}       << "{}";
        QTest::newRow("case1")  << "{}"            << Idxs {1}          << "";
        QTest::newRow("case2")  << " a "           << Idxs {0, 1, 2, 3} << " a ";
        QTest::newRow("case3")  << "a{}"           << Idxs {2}          << "a";
        QTest::newRow("case4")  << "a{}"           << Idxs {1}          << "a{}";
        QTest::newRow("case5")  << "{}a"           << Idxs {0}          << "{}a";
        QTest::newRow("case6")  << "{}a"           << Idxs {1}          << "a";
        QTest::newRow("case7")  << " {} "          << Idxs {2}          << "  ";
        QTest::newRow("case8")  << " {a } "        << Idxs {2, 3, 4}    << " a  ";
        QTest::newRow("case9")  << " {abcd} "      << Idxs {2}          << " abcd ";
        QTest::newRow("case10") << " {abcd abcd} " << Idxs {2}          << " abcd abcd ";
        // clang-format on
    }

    void removeTags()
    {
        QFETCH(QString, src);
        QFETCH(Idxs, cursorPositions);
        QFETCH(QString, result);

        for (auto cursor : cursorPositions) {
            QVERIFY2(TagUtils::removeTag(src, cursor) == result, failCursor(cursor));
        }
    }

    void tagExtraction_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<QStringList>("tags");

        // clang-format off
        QTest::newRow("ok0")    << "a{bc}"     << (QStringList() << "bc");
        QTest::newRow("ok1")    << "a{a}{b}"   << (QStringList() << "a" << "b");
        QTest::newRow("ok2")    << "a{bc\\}"   << (QStringList() << "bc\\");
        QTest::newRow("ok3")    << "a{bc}{bc}" << (QStringList() << "bc");

        QTest::newRow("error0") << "{{"        << QStringList();
        QTest::newRow("error1") << "}}"        << QStringList();
        QTest::newRow("error2") << "a{b"       << QStringList();
        QTest::newRow("error3") << "a{a\\"     << QStringList();
        QTest::newRow("error4") << "a{bc}}"    << QStringList();
        QTest::newRow("error5") << "\\}\\}"    << QStringList();
        QTest::newRow("error6") << "a{bc\\}}"  << QStringList();
        // clang-format on
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

        // clang-format off
        QTest::newRow("case0") << ""         << "";
        QTest::newRow("case1") << " asdf "   << " asdf ";
        QTest::newRow("case2") << " {asdf} " << " <font color=\"#6d9a28\">asdf</font> ";
        // clang-format on
    }

    void decorateTags()
    {
        QFETCH(QString, src);
        QFETCH(QString, result);

        QVERIFY(TagUtils::replaceTags(src, "<font color=\"#6d9a28\">", "</font>") == result);
    }

    void tagLengthSuitTerm_data()
    {
        QTest::addColumn<QString>("word1");
        QTest::addColumn<QString>("word2");
        QTest::addColumn<bool>("result");

        // clang-format off
        QTest::newRow("case0") << ""                     << ""                        << true;
        QTest::newRow("case1") << "атом"                 << "атомов"                  << true;
        QTest::newRow("case2") << "электрон"             << "электроны"               << true;
        QTest::newRow("case3") << "отрицательный заряд"  << "отрицательный заряд"     << true;
        QTest::newRow("case4") << "элементарная частица" << "элементарными частицами" << true;
        QTest::newRow("case5") << "конформация"          << "конформационный"         << true;
        QTest::newRow("case7") << "w w"                  << "wwwww"                   << true;

        QTest::newRow("case8") << "w"   << "wwwwww"       << false;
        QTest::newRow("case9") << "w w" << "wwwww wwwwww" << false;
        // clang-format on
    }

    void tagLengthSuitTerm()
    {
        QFETCH(QString, word1);
        QFETCH(QString, word2);
        QFETCH(bool, result);

        QVERIFY(TagUtils::tagLengthSuitTerm(word1, word2) == result);
        QVERIFY(TagUtils::tagLengthSuitTerm(word2, word1) == result);
    }

    void getLevDistance_data()
    {
        QTest::addColumn<QString>("word1");
        QTest::addColumn<QString>("word2");
        QTest::addColumn<int>("limit");
        QTest::addColumn<int>("distance");

        const int max_int = std::numeric_limits<int>::max();

        // clang-format off
        QTest::newRow("case0") << ""                     << ""                        << 9 << 0;
        QTest::newRow("case1") << "атом"                 << "атомов"                  << 9 << 2;
        QTest::newRow("case2") << "электрон"             << "электроны"               << 9 << 1;
        QTest::newRow("case3") << "отрицательный заряд"  << "отрицательный заряд"     << 9 << 0;
        QTest::newRow("case4") << "элементарная частица" << "элементарными частицами" << 9 << 5;
//        QTest::newRow("case5") << "элементарная частица" << "элементарными частицами" << 3 << max_int;
        QTest::newRow("case6") << "конформация"          << "конформационный"         << 9 << 5;
        QTest::newRow("case7") << "w w"                  << "wwwww"                   << 9 << 3;
        // clang-format on
    }

    void getLevDistance()
    {
        QFETCH(QString, word1);
        QFETCH(QString, word2);
        QFETCH(int, limit);
        QFETCH(int, distance);

        qDebug() << TagUtils::getLevDistance(word1, word2, limit);
        QVERIFY(TagUtils::getLevDistance(word1, word2, limit) == distance);
    }

    void wordBorder_data()
    {
        QTest::addColumn<int>("dir");
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startFrom");
        QTest::addColumn<int>("targetPosition");

        // Предполагается что -1 - влево, 1 - вправо
        // В пустой строке поиск и в лево и вправо должен вовращать 0
        // clang-format off
        QTest::newRow("case0")  << -1 << ""      << 0 << 0;
        QTest::newRow("case1")  <<  1 << ""      << 0 << 0;
        QTest::newRow("case2")  << -1 << " "     << 0 << 0;
        QTest::newRow("case3")  <<  1 << " "     << 0 << 0;
        QTest::newRow("case4")  << -1 << " "     << 1 << 1;
        QTest::newRow("case5")  <<  1 << " "     << 1 << 1;
        QTest::newRow("case6")  << -1 << " a"    << 1 << 1;
        QTest::newRow("case7")  <<  1 << " a"    << 1 << 2;
        QTest::newRow("case8")  << -1 << "ab"    << 0 << 0;
        QTest::newRow("case9")  <<  1 << "ab"    << 0 << 2;
        QTest::newRow("case10") << -1 << "ab"    << 1 << 0;
        QTest::newRow("case11") <<  1 << "ab"    << 1 << 2;
        QTest::newRow("case12") << -1 << "ab"    << 2 << 0;
        QTest::newRow("case13") <<  1 << "ab"    << 2 << 2;
        QTest::newRow("case14") << -1 << " ab "  << 0 << 0;
        QTest::newRow("case15") <<  1 << " ab "  << 0 << 0;
        QTest::newRow("case16") << -1 << " ab "  << 1 << 1;
        QTest::newRow("case17") <<  1 << " ab "  << 1 << 3;
        QTest::newRow("case18") << -1 << " ab, " << 1 << 1;
        QTest::newRow("case19") <<  1 << " ab, " << 1 << 3;
        QTest::newRow("case20") << -1 << " a a " << 3 << 3;
        QTest::newRow("case21") <<  1 << " a a " << 3 << 4;
        QTest::newRow("case22") << -1 << " a a " << 4 << 3;
        QTest::newRow("case23") <<  1 << " a a " << 4 << 4;
        // clang-format on
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

    void wordsCount_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("count");

        // Предполагается что -1 - влево, 1 - вправо
        // В пустой строке поиск и в лево и вправо должен вовращать 0
        // clang-format off
        QTest::newRow("case0")  << ""                 << 0;
        QTest::newRow("case1")  << "w"                << 1;
        QTest::newRow("case2")  << " w "              << 1;
        QTest::newRow("case3")  << "2"                << 1;
        QTest::newRow("case4")  << " 2 "              << 1;
        QTest::newRow("case5")  << "w w w"            << 3;
        QTest::newRow("case6")  << "abcd"             << 1;
        QTest::newRow("case7")  << " abcd "           << 1;
        QTest::newRow("case8")  << "abcd abcd"        << 2;
        QTest::newRow("case9")  << " abcd abcd "      << 2;
        QTest::newRow("case10") << "   abcd  abcd   " << 2;
        // clang-format on
    }

    void wordsCount()
    {
        QFETCH(QString, src);
        QFETCH(int, count);

        QVERIFY(TagUtils::wordsCount(src) == count);
    }

    void pairBrackets_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<bool>("result");

        // clang-format off
        QTest::newRow("ok0") << ""                     << true;
        QTest::newRow("ok1") << "{{}{}}"               << true;
        QTest::newRow("ok2") << "{}{}{}{}{}{}"         << true;
        QTest::newRow("ok3") << "{{{{{{{{{{}}}}}}}}}}" << true;

        QTest::newRow("wrong0") << "{"       << false;
        QTest::newRow("wrong1") << "}"       << false;
        QTest::newRow("wrong2") << "}{"      << false;
        QTest::newRow("wrong3") << "a}b{c"   << false;
        QTest::newRow("wrong4") << "a}b}c"   << false;
        QTest::newRow("wrong5") << "a{b{c"   << false;
        QTest::newRow("wrong6") << "{{}{}}}" << false;
        // clang-format on
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
        // clang-format off
        QTest::newRow("case0")  << -1 << ""           << 0 << QChar();
        QTest::newRow("case1")  <<  1 << ""           << 0 << QChar();
        QTest::newRow("case2")  << -1 << "{"          << 0 << QChar();
        QTest::newRow("case3")  <<  1 << "{"          << 0 << leftBracket;
        QTest::newRow("case4")  << -1 << "{"          << 1 << leftBracket;
        QTest::newRow("case5")  <<  1 << "{"          << 1 << QChar();
        QTest::newRow("case6")  << -1 << "}"          << 0 << QChar();
        QTest::newRow("case7")  <<  1 << "}"          << 0 << rightBracket;
        QTest::newRow("case8")  << -1 << "}"          << 1 << rightBracket;
        QTest::newRow("case9")  <<  1 << "}"          << 1 << QChar();
        QTest::newRow("case10") << -1 << " { asdf } " << 2 << leftBracket;
        QTest::newRow("case11") <<  1 << " { asdf } " << 2 << rightBracket;
        QTest::newRow("case12") << -1 << " } asdf { " << 2 << rightBracket;
        QTest::newRow("case13") <<  1 << " } asdf { " << 2 << leftBracket;
        // clang-format off
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

        // clang-format off
        QTest::newRow("case0")  << "{"               << -1;
        QTest::newRow("case1")  << "}"               << -1;
        QTest::newRow("case2")  << "{{"              << -1;
        QTest::newRow("case3")  << "}}"              << -1;
        QTest::newRow("case4")  << "  "              << 0;
        QTest::newRow("case5")  << "{}"              << 1;
        QTest::newRow("case6")  << "aa}"             << -1;
        QTest::newRow("case7")  << "}aa"             << -1;
        QTest::newRow("case8")  << "{{{}}}  "        << 3;
        QTest::newRow("case9")  << "a{a}a{a}a{a}a"   << 1;
        QTest::newRow("case10") << "a{}  {{{}}} {}a" << 3;
        // clang-format on
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

        // clang-format off
        QTest::newRow("case0") << ""  <<  0 << true;
        QTest::newRow("case1") << "a" <<  1 << true;
        QTest::newRow("case2") << "a" <<  0 << true;

        QTest::newRow("case0") << ""  << -1 << false;
        QTest::newRow("case1") << ""  <<  1 << false;
        QTest::newRow("case2") << "a" << -1 << false;
        QTest::newRow("case3") << "a" <<  2 << false;
        // clang-format on
    }

    void validCursor()
    {
        QFETCH(QString, src);
        QFETCH(int, cursorPosition);
        QFETCH(bool, result);

        QVERIFY(TagUtils::isValidCursor(src, cursorPosition) == result);
    }

private:
    const char* failCursor(int cursor)
    {
        return QString("Fail on cursor " + QString::number(cursor)).toStdString().c_str();
    }
};

QTEST_APPLESS_MAIN(TagUtilsTest)

#include "tagutilstest.moc"
