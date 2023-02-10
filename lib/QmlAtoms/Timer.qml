// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick as Q

Q.Timer {
    property bool startTrigger: false

    onStartTriggerChanged: {
        if (startTrigger)
            start();
    }
}
