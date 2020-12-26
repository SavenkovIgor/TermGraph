/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#include <memory>

#include <QCoreApplication>
#include <QtTest>

#include "source/Helpers/link/link.h"
#include "source/Helpers/link/linksstring.h"

class LinkTest : public QObject
{
    Q_OBJECT

    using Idxs = std::vector<int>; // Short for Indexes

private slots:
    void linkSearch_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startFrom");
        QTest::addColumn<bool>("result");

        QTest::addColumn<int>("leftPos");
        QTest::addColumn<int>("rightPos");
        QTest::addColumn<bool>("isEmpty");
        QTest::addColumn<int>("size");

        // clang-format off
        QTest::newRow("case01") << ""                                             << 0 << false << -1 << -1 << true  << 0;
        QTest::newRow("case02") << " "                                            << 0 << false << -1 << -1 << true  << 0;
        QTest::newRow("case03") << "  "                                           << 1 << false << -1 << -1 << true  << 0;
        QTest::newRow("case04") << "} {"                                          << 0 << false << -1 << -1 << true  << 0;
        QTest::newRow("case05") << "} {"                                          << 1 << false << -1 << -1 << true  << 0;
        QTest::newRow("case06") << "} {"                                          << 2 << false << -1 << -1 << true  << 0;
        QTest::newRow("case07") << "} {"                                          << 3 << false << -1 << -1 << true  << 0;
        QTest::newRow("case08") << "{} {}"                                        << 2 << false << -1 << -1 << true  << 0;
        QTest::newRow("case09") << " {a}"                                         << 1 << false << -1 << -1 << true  << 3;
        QTest::newRow("case10") << " { abc2 }"                                    << 2 << true  <<  1 <<  9 << false << 8;
        QTest::newRow("case11") << " {a} {b}"                                     << 2 << true  <<  1 <<  4 << false << 3;
        QTest::newRow("case12") << "{}ab"                                         << 1 << true  <<  0 <<  2 << false << 2;
        QTest::newRow("case13") << " {ab|94810de3-51b8-469e-b316-00248ffa2a45}"   << 2 << true  <<  1 << 42 << false << 41;
        QTest::newRow("case14") << " { ab |94810de3-51b8-469e-b316-00248ffa2a45}" << 2 << true  <<  1 << 44 << false << 43;
        // clang-format on
    }

    void linkSearch()
    {
        QFETCH(QString, src);
        QFETCH(int, startFrom);
        QFETCH(bool, result);
        QFETCH(int, leftPos);
        QFETCH(int, rightPos);
        QFETCH(bool, isEmpty);
        QFETCH(int, size);

        auto borders = Link::select(src, startFrom);
        QCOMPARE(borders.has_value(), result);

        if (borders.has_value()) {
            QCOMPARE(borders->left().pos(), leftPos);
            QCOMPARE(borders->right().pos(), rightPos);
            QCOMPARE(borders->isEmpty(), isEmpty);
            QCOMPARE(borders->size(), size);
        }
    }

    void linkWithUuid()
    {
        auto link = Link(linkedTextWithUuid, 5, 48);
        auto res  = link.createLinkWithUuid(QUuid());

        QCOMPARE(res, "{Ab c|00000000-0000-0000-0000-000000000000}");
        QCOMPARE(link.fullLink(), QString("{Ab c|94810de3-51b8-469e-b316-00248ffa2a45}"));
        QCOMPARE(link.text(), QString("Ab c"));
        QCOMPARE(link.textLower(), "ab c");
        QCOMPARE(link.hasUuid(), true);
        QCOMPARE(link.uuid(), QUuid::fromString(QString("{94810de3-51b8-469e-b316-00248ffa2a45}")));
    }

    void linkNoUuid()
    {
        auto link = Link(linkedText, 5, 11);

        QCOMPARE(link.createLinkWithUuid(QUuid()), QString("{Ab c|00000000-0000-0000-0000-000000000000}"));
        QCOMPARE(link.fullLink(), QString("{Ab c}"));
        QCOMPARE(link.text(), QString("Ab c"));
        QCOMPARE(link.textLower(), "ab c");
        QCOMPARE(link.hasUuid(), false);
    }

    void linksString()
    {
        auto text = LinksString(multipleLinks);

        QCOMPARE(text.links().size(), 3);
        QCOMPARE(text.links()[1].fullLink(), QString("{bc}"));
        QCOMPARE(text.links()[1].text(), QString("bc"));

        QCOMPARE(LinksString(invalidLinksString1).links().size(), 0);
        QCOMPARE(LinksString(invalidLinksString2).links().size(), 0);
        QCOMPARE(LinksString(invalidLinksString3).links().size(), 0);
    }

    void linksReplacement()
    {
        auto lText = LinksString(multipleLinks);

        auto res = lText.replaceLink(1, "{aaa}");

        QCOMPARE(res, " {abc} {aaa} {c}");
    }

    void validLinksString_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<bool>("result");

        // clang-format off
        QTest::newRow("ok0") << ""                     << true;
        QTest::newRow("ok1") << "{}"                   << true;
        QTest::newRow("ok2") << "{}{}{}{}{}{}"         << true;
        QTest::newRow("ok3") << "{a}{b}{c}   {},-{}{}" << true;

        QTest::newRow("wrong0") << "{"                    << false;
        QTest::newRow("wrong1") << "}"                    << false;
        QTest::newRow("wrong2") << "}{"                   << false;
        QTest::newRow("wrong3") << "a}b{c"                << false;
        QTest::newRow("wrong4") << "a}b}c"                << false;
        QTest::newRow("wrong5") << "a{b{c"                << false;
        QTest::newRow("wrong6") << "{{}{}}"               << false;
        QTest::newRow("wrong7") << "{{}{}}}"              << false;
        QTest::newRow("wrong8") << "{{{{{{{{{{}}}}}}}}}}" << false;
        // clang-format on
    }

    void validLinksString()
    {
        QFETCH(QString, text);
        QFETCH(bool, result);

        QCOMPARE(LinksString::isValidLinksString(text), result);
    }

    void inLink_data()
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

    void inLink()
    {
        QFETCH(QString, src);
        QFETCH(Idxs, cursorPositions);
        QFETCH(bool, result);

        for (auto cursor : cursorPositions) {
            QCOMPARE(Link::isCursorOnLink(src, cursor), result);
        }
    }

    void linkAt_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("index");
        QTest::addColumn<int>("start");
        QTest::addColumn<int>("end");

        // clang-format off
        QTest::newRow("case0") << "{a}{b}{}" << 0 << 0 << 3;
        QTest::newRow("case1") << "{a}{b}{}" << 1 << 3 << 6;
        QTest::newRow("case2") << "{a}{b}{}" << 2 << 6 << 8;
        QTest::newRow("case3") << " {a} {b} {} " << 0 << 1 << 4;
        QTest::newRow("case4") << " {a} {b} {} " << 1 << 5 << 8;
        QTest::newRow("case5") << " {a} {b} {} " << 2 << 9 << 11;
        QTest::newRow("case6") << "{}"       << 0 << 0 << 2;
        QTest::newRow("case7") << "{asdf}"   << 0 << 0 << 6;
        QTest::newRow("case8") << " {   } "  << 0 << 1 << 6;
        // clang-format on
    }

    void linkAt()
    {
        QFETCH(QString, src);
        QFETCH(int, index);
        QFETCH(int, start);
        QFETCH(int, end);

        auto lString = LinksString(src);
        auto range   = lString.links()[index];

        QCOMPARE(range.left().pos(), start);
        QCOMPARE(range.right().pos(), end);
    }

    void addLink_data()
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

    void addLink()
    {
        QFETCH(QString, src);
        QFETCH(Idxs, cursorPositions);
        QFETCH(QString, result);

        for (auto cursor : cursorPositions) {
            QCOMPARE(LinksString::addLink(src, cursor), result);
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
            QCOMPARE(LinksString::expandLinkRight(src, cursor), result);
        }
    }

    void removeLink_data()
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

    void removeLink()
    {
        QFETCH(QString, src);
        QFETCH(Idxs, cursorPositions);
        QFETCH(QString, result);

        for (auto cursor : cursorPositions) {
            QCOMPARE(LinksString::removeLink(src, cursor), result);
        }
    }

private:
    const QString linkedTextWithUuid  = " abc {Ab c|94810de3-51b8-469e-b316-00248ffa2a45} abc ";
    const QString linkedText          = " abc {Ab c} abc ";
    const QString multipleLinks       = " {abc} {bc} {c}";
    const QString invalidLinksString1 = "}{";
    const QString invalidLinksString2 = "}}{{";
    const QString invalidLinksString3 = "";
};

QTEST_APPLESS_MAIN(LinkTest)

#include "linktest.moc"
