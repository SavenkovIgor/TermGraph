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

#pragma once

#include <QObject>
#include <QString>
#include <QStringView>

class TagProcessor : public QObject
{
    Q_OBJECT

public:
    static TagProcessor& instance();

    TagProcessor(TagProcessor const&) = delete;
    void operator=(TagProcessor const&) = delete;

    Q_INVOKABLE static bool isValidCursor(const QString& str, int cursor);
    Q_INVOKABLE static bool isCursorOnLink(const QString& str, int cursor);
    Q_INVOKABLE static bool hasSoftLinks(const QString& linkedText);

    Q_INVOKABLE static QString addLink(QString str, int cursor);
    Q_INVOKABLE static QString expandLinkRight(QString str, int cursor);
    Q_INVOKABLE static QString removeLink(QString str, int cursor);
    Q_INVOKABLE static QString decorateTags(const QString& str);

private:
    explicit TagProcessor(QObject* parent = nullptr);
};
