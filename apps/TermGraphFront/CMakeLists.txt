# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

#cmake_minimum_required (VERSION 3.16.0)

#project (TermGraph VERSION 2.0.0 LANGUAGES CXX)

#option (BUILD_TESTING "Build tests with project" ON)

#if (ANDROID)
#    set (BUILD_TESTING OFF)
#endif ()

##set (CMAKE_INCLUDE_CURRENT_DIR ON)
#set (QML_PATH ${PROJECT_SOURCE_DIR}/qml)
#set (QML_IMPORT_PATH "${QML_PATH}" CACHE STRING "" FORCE)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if (ANDROID)
#    set (ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    #    if (ANDROID_ABI STREQUAL "armeabi-v7a")
    #        set(ANDROID_EXTRA_LIBS
    #            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
    #            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
    #    endif()
#endif ()

#add_subdirectory (${CMAKE_SOURCE_DIR}/libs/commontools)
#add_subdirectory (${CMAKE_SOURCE_DIR}/libs/datainterface)
#add_subdirectory (${CMAKE_SOURCE_DIR}/libs/termstorage)

# If you want, android build correctly,
# This line must be before targets definition
#find_package (Qt6 COMPONENTS Core Gui Qml Quick QuickControls2 Network Svg REQUIRED)

#if (ANDROID)
#    add_library (${PROJECT_NAME} SHARED)
#else ()
#    add_executable (${PROJECT_NAME})
#endif ()

#set_target_properties (${PROJECT_NAME}
#    PROPERTIES
#        AUTOUIC ON
#        AUTOMOC ON
#        AUTORCC ON
#        CXX_STANDARD 20
#        CXX_STANDARD_REQUIRED ON)
##        QT_QML_MODULE_VERSION 1.0
##        QT_QML_MODULE_URI     Api)

## qt6_qml_type_registration(${PROJECT_NAME})

## Resources folder
#target_sources (${PROJECT_NAME} PRIVATE resources/helpImages/helpImages.qrc resources/icons/icons.qrc)

## Qml folder
#target_sources (${PROJECT_NAME} PRIVATE qml/qml.qrc)

## Some non compilable files
#target_sources (${PROJECT_NAME}
#    PRIVATE
#        android/AndroidManifest.xml
#        android/res/drawable-hdpi/icon.png
#        android/res/drawable-mdpi/icon.png
#        android/res/drawable-ldpi/icon.png)


## Source folder
#file (GLOB_RECURSE ALL_SOURCES source/*.h source/*.hpp source/*.cpp)
#target_sources (${PROJECT_NAME} PRIVATE ${ALL_SOURCES})

#target_include_directories (${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

#source_group (Sql REGULAR_EXPRESSION ".*\.sql")
#source_group (Cpp REGULAR_EXPRESSION "source\/.*\.(h|cpp)")
#source_group (DevTools REGULAR_EXPRESSION ".devTools\/.*")
#source_group (Qml REGULAR_EXPRESSION "qml\/")

##message (FATAL_ERROR ${PROJECT_SOURCE_DIR})

#target_compile_definitions (${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

#target_link_libraries (${PROJECT_NAME} PRIVATE CommonTools TermDataInterface TermStorage)

#target_link_libraries (${PROJECT_NAME}
#    PRIVATE
#        Qt6::Core
#        Qt6::Gui
#        Qt6::Quick
#        Qt6::QuickControls2
#        Qt6::Qml
#        Qt6::Network
#        Qt6::Svg)



#if (BUILD_TESTING)
#    add_subdirectory (tests)
#endif ()
