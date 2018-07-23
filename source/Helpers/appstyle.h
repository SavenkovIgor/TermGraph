#ifndef APPSTYLE_H
#define APPSTYLE_H

#include <QColor>

class AppStyle
{
public:
    class Colors {
    public:
        static QColor transparent;

        class Nodes {
        public:
            static QColor border;
            static QColor text;

            static QColor leaf;
            static QColor root;
            static QColor orphan;

            static QColor leafSelected;
            static QColor rootSelected;
            static QColor orphanSelected;
        };

        class Groups {
        public:
            static QColor border;
            static QColor text;
            static QColor backgroundTerms;
            static QColor backgroundFreeConnections;
        };

        class Scene {
        public:
            static QColor background;
        };
    };
};

#endif // APPSTYLE_H
