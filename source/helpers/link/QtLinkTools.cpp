// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/link/QtLinkTools.h"

import Link.LinkTools;

QtLinkTools::QtLinkTools(QObject* parent)
    : QObject(parent)
{}

bool QtLinkTools::isValidCursor(const QString& str, int cursor) { return LinkTools::isValidCursor(str, cursor); }

bool QtLinkTools::isCursorOnLink(const QString& str, int cursor) { return LinkTools::isCursorOnLink(str, cursor); }

bool QtLinkTools::hasSoftLinks(const QString& linkedText) { return LinkTools::hasSoftLinks(linkedText); }

QString QtLinkTools::add(QString str, int cursor) { return LinkTools::add(str, cursor); }

QString QtLinkTools::expandRight(QString str, int cursor) { return LinkTools::expandRight(str, cursor); }

QString QtLinkTools::remove(QString str, int cursor) { return LinkTools::remove(str, cursor); }

QString QtLinkTools::decorate(const QString& str) { return LinkTools::decorate(str); }
