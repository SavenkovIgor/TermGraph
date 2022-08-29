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
class LinkUtils
{
public:
    // Tags
    static bool tagLengthSuitTerm(const QString& tag, const QString& termName);

    static int      getLevDistance(QStringView src, QStringView dst, int limit = 100000);
    static Opt<int> getDistanceBetweenTagAndTerm(const QString& tag, const QString& termName, int maxLimit);
};
