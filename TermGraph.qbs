import qbs

CppApplication {

    name: "TermGraph"
    destinationDirectory: "../../app" // Project.buildDirectory"./app"


    files: [
        "source/databaseWorks/dbabstract.cpp",
        "source/databaseWorks/dbabstract.h",
        "source/databaseWorks/edgtbl.cpp",
        "source/databaseWorks/edgtbl.h",
        "source/databaseWorks/tblbase.cpp",
        "source/databaseWorks/tblbase.h",
        "source/databaseWorks/termgrouptbl.cpp",
        "source/databaseWorks/termgrouptbl.h",
        "source/databaseWorks/ndtbl.cpp",
        "source/databaseWorks/ndtbl.h",
        "source/qml.qrc",
        "source/qml/GoupsSideBar.qml",
        "source/qml/MComboBox.qml",
        "source/qml/MSquareButton.qml",
        "source/qml/MainScheme.qml",
        "source/qml/MainWindow.qml",
        "source/qml/MyLabelPair.qml",
        "source/qml/MyRoundButton.qml",
        "source/qml/MyTextArea.qml",
        "source/qml/MyTextField.qml",
        "source/qml/NewNodeEdit.qml",
        "source/qml/TermGroupsList.qml",
        "source/edge.cpp",
        "source/edge.h",
        "source/glb.cpp",
        "source/glb.h",
        "source/mainscene.cpp",
        "source/mainscene.h",
        "source/mainwindow.cpp",
        "source/mainwindow.h",
        "source/ui/mainwindow.ui",
        "source/mysceneview.cpp",
        "source/mysceneview.h",
        "source/reminder.cpp",
        "source/reminder.h",
        "source/ui/reminder.ui",
        "source/termgroup.cpp",
        "source/termgroup.h",
        "source/terminfo.cpp",
        "source/terminfo.h",
        "source/termnode.cpp",
        "source/termnode.h",
        "source/main.cpp",
        "source/icons.qrc",
        "source/tgroupname.cpp",
        "source/tgroupname.h",
        "source/wordfreqanalyze.cpp",
        "source/wordfreqanalyze.h",
        "source/ui/wordfreqanalyze.ui",
    ]

//    Group {     // Properties for the produced executable
//        fileTagsFilter: product.type
//        qbs.install: true
//    }

    Depends {
        name: "Qt";
        submodules: [
            "core",
            "gui",
            "sql",
            "quickwidgets",
            "quickcontrols2",
//            "widgets",
//            "charts",
        ]
    }
}
