# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


add_library (PagesModule STATIC)

set_target_properties (PagesModule PROPERTIES AUTOMOC ON)

qt_add_qml_module (PagesModule
    URI Pages
    IMPORTS
        Theme Tools Atoms Molecules
    QML_FILES
        AppIconsSaving.qml
        EditNode.qml
        GroupsList.qml
        MainScene.qml
        NewNode.qml
        OnBoard.qml
        Settings.qml
        TermView.qml

        Settings/Database.qml
        Settings/Help.qml
        Settings/License.qml
        Settings/NetworkSettings.qml
)
