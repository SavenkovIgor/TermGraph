// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

Timer {
    property bool startTrigger: false

    onStartTriggerChanged: {
        if (startTrigger)
            start();
    }
}
