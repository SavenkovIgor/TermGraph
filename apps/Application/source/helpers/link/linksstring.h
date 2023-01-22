// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QStringView>

#include <CommonTools/HandyTypes.h>

#include "source/helpers/link/link.h"

class LinksString
{
public:
    LinksString(QStringView str);
    LinksString(const LinksString& other) = default;

    QString           text() const;
    const Link::List& links() const;

    QString replaceLink(int index, QString text) const;

    QString toRichText() const;
    QString toPlainString() const;

    // Static stuff
    static bool isValidLinksString(QStringView str);

    static QString addLink(QString str, int cursor);
    static QString expandLinkRight(QString str, int cursor);
    static QString removeLink(QString str, int cursor);

private: // Methods
    static int        getCount(QStringView strView);
    static Link::List extractLinks(QStringView strView);

private: // Members
    const QStringView mString;
    const Link::List  mLinks;
};
