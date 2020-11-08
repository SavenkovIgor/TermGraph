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

#include "source/Helpers/text/linkstext.h"
#include "source/Helpers/text/textlink.h"
#include "source/Helpers/text/textrange.h"

// TODO: Move some functions of linkutils to LinksText
// TODO: TextSelector must return TextLink
// TODO: Remove uuidValidator

class TextLinkTest : public QObject
{
    Q_OBJECT

private slots:
    void textLinkWithUuid()
    {
        auto link = TextLink(linkedTextWithUuid, 5, 48);
        auto res  = link.createLinkWithUuid(QUuid());

        QCOMPARE(res, "{Ab c|00000000-0000-0000-0000-000000000000}");
        QCOMPARE(link.fullLink(), QString("{Ab c|94810de3-51b8-469e-b316-00248ffa2a45}"));
        QCOMPARE(link.text(), QString("Ab c"));
        QCOMPARE(link.textLower(), "ab c");
        QCOMPARE(link.hasUuid(), true);
        QCOMPARE(link.uuid(), QUuid::fromString(QString("{94810de3-51b8-469e-b316-00248ffa2a45}")));
    }

    void textLinkNoUuid()
    {
        auto link = TextLink(linkedText, 5, 11);

        QCOMPARE(link.createLinkWithUuid(QUuid()), QString("{Ab c|00000000-0000-0000-0000-000000000000}"));
        QCOMPARE(link.fullLink(), QString("{Ab c}"));
        QCOMPARE(link.text(), QString("Ab c"));
        QCOMPARE(link.textLower(), "ab c");
        QCOMPARE(link.hasUuid(), false);
    }

    void linksText()
    {
        auto textOpt1 = LinksText::create(multipleLinks);

        QCOMPARE(textOpt1.has_value(), true);

        auto text = textOpt1.value();

        QCOMPARE(text.count(), 3);
        QCOMPARE(text[1].fullLink(), QString("{b}"));
        QCOMPARE(text[1].text(), QString("b"));

        QCOMPARE(LinksText::create(invalidLinksText1).has_value(), false);
        QCOMPARE(LinksText::create(invalidLinksText2).has_value(), false);
        QCOMPARE(LinksText::create(invalidLinksText3).has_value(), false);
    }

    void linksReplacement()
    {
        auto textOpt1 = LinksText::create(multipleLinks);

        QCOMPARE(textOpt1.has_value(), true);

        auto lText = textOpt1.value();

        lText.replaceLink(1, "{aaa}");

        QCOMPARE(lText.text(), " {abc} {aaa} {c}");
    }

private:
    const QString linkedTextWithUuid = " abc {Ab c|94810de3-51b8-469e-b316-00248ffa2a45} abc ";
    const QString linkedText         = " abc {Ab c} abc ";
    const QString multipleLinks      = " {abc} {bc} {c}";
    const QString invalidLinksText1  = "}{";
    const QString invalidLinksText2  = "}}{{";
    const QString invalidLinksText3  = "";
};

QTEST_APPLESS_MAIN(TextLinkTest)

#include "textlinktest.moc"
