import qbs

Project {
    qbsSearchPaths: "qbs"
    CppApplication {

        name: "TermGraph"
        destinationDirectory: "app" // Project.buildDirectory"./app"

        Group {
            name: "Helpers"
            prefix: "source/Helpers/"
            files: [
                "tagprocessor.cpp",
                "tagprocessor.h",
            ]
        }

        Group {
            name: "Database"
            prefix: "source/databaseWorks/"
            files: [
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
            name: "qrc"
            prefix: "source/"
            files: [
                "icons.qrc",
                "qml.qrc",
            ]
        }

        Group {
            name: "Ui"
            prefix: "source/ui/"
            files: [
                "reminder.ui",
                "wordfreqanalyze.ui",
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
}
