import qbs


CppApplication {

    name : "TermGraph"
    destinationDirectory: "./app" /*[ Project.sourceDirectory + "/app" ]*/

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
        "source/edge.h",
        "source/glb.h",
        "source/mainscene.h",
        "source/mainwindow.h",
        "source/mysceneview.h",
        "source/reminder.cpp",
        "source/reminder.h",
        "source/reminder.ui",
        "source/termgroup.h",
        "source/terminfo.cpp",
        "source/terminfo.h",
        "source/termnode.h",
        "source/edge.cpp",
        "source/glb.cpp",
        "source/main.cpp",
        "source/mainscene.cpp",
        "source/mainwindow.cpp",
        "source/mysceneview.cpp",
        "source/termgroup.cpp",
        "source/termnode.cpp",
        "source/mainwindow.ui",
        "source/icons.qrc",
        "source/model.qmodel",
        "source/wordfreqanalyze.cpp",
        "source/wordfreqanalyze.h",
        "source/wordfreqanalyze.ui",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    Depends {
        name: "Qt";
        submodules: [
            "core",
            "widgets",
//            "gui",
            "sql",
            "charts",
        ]
    }
}

/*

DESTDIR = ./app

*/
