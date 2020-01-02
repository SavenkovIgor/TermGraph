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

#pragma once

#include <QObject>

#include "nodeinfocontainer.h"
#include "source/databaseWorks/ndtbl.h"  // TODO: удаление почему то вызывает проблемы со сборкой

// This class contains all "information" part of term, like name, definition and other...
class InfoTerm : public QObject
{
    Q_OBJECT
public:
    enum KnowLevel{
        dontKnowLvl = 0,
        remindLvl,
        wellRemindLvl
    };

    explicit InfoTerm(const NodeInfoContainer& info, QObject* parent = nullptr);
    virtual ~InfoTerm() = default;
//    bool isNull(); //TODO: Realize!

    QUuid getUuid() const;
    QString getTerm() const;
    const QString getCachedLowerTerm() const;
    QUuid getGroupUuid() const;
    QString getTermAndDefinition(bool decorated = false) const;
    QString getSmallName();  // Lazy

    QString getDefinition() const;
    QStringList getDefinitionTags() const;

    NodeInfoContainer infoContainer() const;

    // Learning
    bool atLearning();
//    void swithcAtLearnVar();

    bool needRemindToday();
    bool isRemindDateMissed();
//    void setRemind(KnowLevel lvl);

    int getRepNum() const;

protected:
    QSizeF getNameSize();  // Lazy

private:
    NodeInfoContainer info;

    QString cachedTermToLower;

    QString smallName = QString();  // То же самое название но ужатое до 2х строчек"
    QSizeF  nameSize = QSizeF();

    QDate lastRepeatDate;
    int repNum = -1;
    bool atLearn;

    // Learning
    int getNextRepeatOffset(int lvl);
    int getLevelDaysFromBase(int lvl);
};
