// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <QChar>
#include <QString>
#include <QStringList>
#include <QStringView>

#include <CommonTools/HandyTypes.h>


// Static class
class LinkTools
{
public:
    static bool linkAndTermSimilarWordDistance(const QString& link, const QString& term);
    static int      levDistance(QStringView src, QStringView dst, int limit = 100000);
    static Opt<int> linkAndTermDistance(const QString& link, const QString& term, int maxLimit);
};
