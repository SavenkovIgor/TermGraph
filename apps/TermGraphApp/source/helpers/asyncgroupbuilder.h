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

#include <functional>

#include <QThread>

#include "source/model/group/termgroup.h"

class AsyncGroupBuilder : public QThread
{
    Q_OBJECT

public:
    explicit AsyncGroupBuilder(QObject* parent = nullptr);

    void setAction(std::function<TermGroup::OptPtr()> func);

    TermGroup::OptPtr takeResult();

protected:
    void run() final;

private:
    std::function<TermGroup::OptPtr()> mAction;

    TermGroup::OptPtr mResultGroup = std::nullopt;
};
