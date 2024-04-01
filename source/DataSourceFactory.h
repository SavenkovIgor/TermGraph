// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>

#include "source/TermDataInterface/DataStorageInterface.h"

struct DataSourceFactory
{
    static std::unique_ptr<DataStorageInterface> defaultSource();

    static std::unique_ptr<DataStorageInterface> localDb();
    static std::unique_ptr<DataStorageInterface> server();
};
