import qbs

CppApplication {

    name: "TermGraph"
    destinationDirectory: "app" // Project.buildDirectory"./app"

    Group {
        name: "Database"
        prefix: "source/databaseWorks/"
        files: [
            "commonqueryfunctions.h",
            "sqlqueryconstructor.cpp",
            "sqlqueryconstructor.h",
            "cloudservices.cpp",
            "cloudservices.h",
            "dbabstract.cpp",
            "dbabstract.h",
            "dropboxconnection.cpp",
            "dropboxconnection.h",
            "edgtbl.cpp",
            "edgtbl.h",
            "tblbase.cpp",
            "tblbase.h",
            "termgrouptbl.cpp",
            "termgrouptbl.h",
            "ndtbl.cpp",
            "ndtbl.h",
        ]
    }

    Group {
        name: "Common"
        prefix: "source/"
        files: [
            "edge.cpp",
            "edge.h",
            "glb.cpp",
            "glb.h",
            "mainscene.cpp",
            "mainscene.h",
            "mainwindow.cpp",
            "mainwindow.h",
            "mysceneview.cpp",
            "mysceneview.h",
            "reminder.cpp",
            "reminder.h",
            "termgroup.cpp",
            "termgroup.h",
            "terminfo.cpp",
            "terminfo.h",
            "termnode.cpp",
            "termnode.h",
            "main.cpp",
            "tgroupname.cpp",
            "tgroupname.h",
            "wordfreqanalyze.cpp",
            "wordfreqanalyze.h",
            "Helpers/tagprocessor.cpp",
            "Helpers/tagprocessor.h",
            "icons.qrc",
            "qml.qrc",
            "ui/reminder.ui",
            "ui/wordfreqanalyze.ui",
        ]
    }

    Depends {
        name: "Qt";
        submodules: [
            "core",
            "gui",
            "sql",
            "quickwidgets",
            "quickcontrols2",
            "xml",
            //            "widgets",
            //            "charts",
        ]
    }
    //        Depends{ name: "QtDropbox" }

    //            cpp.includePaths: ["source/QtDropbox/src"]
    //            cpp.libraryPaths: ["source/QtDropbox/build"]
    //            cpp.dynamicLibraries: "QtDropbox"

}
