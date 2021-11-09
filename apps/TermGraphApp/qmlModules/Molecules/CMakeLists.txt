#  TermGraph, build graph of knowledge.
#  Copyright © 2016-2020. Savenkov Igor. All rights reserved
#  Contacts: dev.savenkovigor@protonmail.com
#
#  This file is part of TermGraph.
#
#  TermGraph is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  TermGraph is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (MoleculesModule LANGUAGES CXX)

qt_add_library (MoleculesModule STATIC)

set_target_properties (MoleculesModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (MoleculesModule
    URI Molecules
    VERSION 1.0
    IMPORTS StyleInfo Tools Atoms
    QML_FILES
        AppIcon.qml
        BurgerButton.qml
        DefaultHeader.qml
        EdgeHardeningDrawer.qml
        EmptyView.qml
        HelpCard.qml
        HelpView.qml
        LabelPair.qml
        LoadingIcon.qml
        LoadingInProcess.qml
        MainAppDrawer.qml
        NodesScene.qml
        NotificationDrawer.qml
        Page.qml
        SearchDrawer.qml
        SideMenuHeader.qml
        StdDialog.qml
        TagEditorArea.qml
        TermInfoDrawer.qml
        TextArea.qml
        TextField.qml)
