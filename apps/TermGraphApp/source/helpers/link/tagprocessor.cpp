// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/link/tagprocessor.h"

#include "source/helpers/link/link.h"
#include "source/helpers/link/linksdecorator.h"
#include "source/helpers/link/linksstring.h"
#include "source/helpers/text/textcursor.h"

TagProcessor& TagProcessor::instance()
{
    static TagProcessor mgr;
    return mgr;
}

bool TagProcessor::isValidCursor(const QString& str, int cursor)
{
    return TextCursor::isValidCursor(QStringView(str), cursor);
}

bool TagProcessor::isCursorOnLink(const QString& str, int cursor) { return Link::isCursorOnLink(str, cursor); }

bool TagProcessor::hasSoftLinks(const QString& linkedText)
{
    LinksString txt(linkedText);

    for (const auto& link : txt.links()) {
        if (!link.hasUuid())
            return true;
    }

    return false;
}

QString TagProcessor::addLink(QString str, int cursor) { return LinksString::addLink(str, cursor); }

QString TagProcessor::expandLinkRight(QString str, int cursor) { return LinksString::expandLinkRight(str, cursor); }

QString TagProcessor::removeLink(QString str, int cursor) { return LinksString::removeLink(str, cursor); }

QString TagProcessor::decorateTags(const QString& str)
{
    LinksString    linkText(str);
    LinksDecorator decorator(linkText, LinksDecorator::blueDecorator);
    return decorator.apply(LinksDecoratorMode::Replace);
}

TagProcessor::TagProcessor(QObject* parent)
    : QObject(parent)
{}
