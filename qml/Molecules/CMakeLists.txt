# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


add_library (MoleculesModule STATIC)

set_target_properties (MoleculesModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (MoleculesModule
    URI Molecules
    IMPORTS
        Theme Tools Atoms
    QML_FILES
        AppIcon.qml
        BurgerButton.qml
        DefaultHeader.qml
        EdgeHardeningDrawer.qml
        EmptyView.qml
        HelpCard.qml
        HelpView.qml
        LabelPair.qml
        LinkEditorArea.qml
        LoadingIcon.qml
        LoadingInProcess.qml
        MainAppDrawer.qml
        NodesScene.qml
        NotificationDrawer.qml
        Page.qml
        SearchDrawer.qml
        SideMenuHeader.qml
        StdDialog.qml
        TermInfoDrawer.qml
        TextArea.qml
        TextField.qml
)
