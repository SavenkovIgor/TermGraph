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
#include "source/Helpers/link/linkstext.h"

// TODO: Move some functions of linkutils to LinksText
// TODO: LinksText -> LinksString

class LinkTest : public QObject
{
    Q_OBJECT

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

    void linksText()
    {
        auto text = LinksText(multipleLinks);

        QCOMPARE(text.links().size(), 3);
        QCOMPARE(text.links()[1].fullLink(), QString("{bc}"));
        QCOMPARE(text.links()[1].text(), QString("bc"));

        QCOMPARE(LinksText(invalidLinksText1).links().size(), 0);
        QCOMPARE(LinksText(invalidLinksText2).links().size(), 0);
        QCOMPARE(LinksText(invalidLinksText3).links().size(), 0);
    }

    void linksReplacement()
    {
        auto lText = LinksText(multipleLinks);

        auto res = lText.replaceLink(1, "{aaa}");

        QCOMPARE(res, " {abc} {aaa} {c}");
    }

    void validLinksText_data()
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

    void validLinksText()
    {
        QFETCH(QString, text);
        QFETCH(bool, result);

        QVERIFY(LinksText::isValidLinksString(text) == result);
    }

private:
    const QString linkedTextWithUuid = " abc {Ab c|94810de3-51b8-469e-b316-00248ffa2a45} abc ";
    const QString linkedText         = " abc {Ab c} abc ";
    const QString multipleLinks      = " {abc} {bc} {c}";
    const QString invalidLinksText1  = "}{";
    const QString invalidLinksText2  = "}}{{";
    const QString invalidLinksText3  = "";
};

QTEST_APPLESS_MAIN(LinkTest)

#include "linktest.moc"
