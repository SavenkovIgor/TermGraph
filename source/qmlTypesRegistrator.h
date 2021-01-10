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

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>

#include "source/Helpers/tagprocessor.h"
#include "source/Managers/linkshardeningmanager.h"
#include "source/Managers/notificationmanager.h"
#include "source/Model/Termin/nodegadgetwrapper.h"
#include "source/Model/TerminGroup/termgroup.h"

template<typename T>
QObject* registerSingleton([[maybe_unused]] QQmlEngine* qmlEngine, [[maybe_unused]] QJSEngine* jsEngine)
{
    auto& singleton = T::instance();
    QQmlEngine::setObjectOwnership(&singleton, QQmlEngine::CppOwnership);
    return &singleton;
}

void registerUserQmlTypes()
{
    qmlRegisterSingletonType<NotificationManager>("Helpers",
                                                  1,
                                                  0,
                                                  "Notification",
                                                  &registerSingleton<NotificationManager>);

    qmlRegisterSingletonType<TagProcessor>("Helpers", 1, 0, "TagProcessor", &registerSingleton<TagProcessor>);

    qmlRegisterUncreatableType<NodeGadgetWrapper>("NodeGadgetWrapper",
                                                  1,
                                                  0,
                                                  "NodeGadgetWrapper",
                                                  "Please use NodeGadgetWrapper from backend");

    qmlRegisterType<LinksHardeningManager>("Helpers", 1, 0, "LinksHardeningManager");

    qmlRegisterAnonymousType<TermGroup>("Helpers", 1);
}
