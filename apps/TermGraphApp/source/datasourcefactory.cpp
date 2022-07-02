// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

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
