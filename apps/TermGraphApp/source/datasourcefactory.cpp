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

#include "datasourcefactory.h"

#include <QHostAddress>

#include <CommonTools/Platform.h>
#include <TermDataStorage/LocalDataStorage.h>

//#ifndef Q_OS_WASM
//#include <TermDataConnection/DataStorageConnection.h>
//#endif

#include "source/helpers/appconfig.h"

std::unique_ptr<DataStorageInterface> DataSourceFactory::defaultSource()
{
    if (Platform::isWasm()) {
        return wasm();
    }

    if (Platform::isDesktop() || Platform::isMobile()) {
        return localDb();
    }
}

std::unique_ptr<DataStorageInterface> DataSourceFactory::localDb()
{
    using namespace AppSettings;
    return std::make_unique<LocalDatabaseStorage>(StdPaths::defaultDatabaseFilePath(), StdPaths::backupFolder());
}

std::unique_ptr<DataStorageInterface> DataSourceFactory::server()
{
    //    auto address = QHostAddress("127.0.0.1");
    //    return std::make_unique<DataStorageConnection>(address);
    return nullptr;
}

std::unique_ptr<DataStorageInterface> DataSourceFactory::wasm() { return nullptr; }
