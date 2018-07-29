#ifndef APPSTYLE_H
#define APPSTYLE_H

#include <QColor>

class AppStyle
{
public:
    class Colors {
    public:
        static QColor transparent;
        static QColor testColor;

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

            static QColor atLearn;
        };

        class Groups {
        public:
            static QColor border;
            static QColor text;
            static QColor backgroundTerms;
            static QColor backgroundFreeConnections;
        };

        class Edges {
        public:
            static QColor standard;
            static QColor termin;
            static QColor description;
            static QColor selected;
        };

        class Scene {
        public:
            static QColor background;
        };
    };

    class Sizes {
    public:
        static qreal groupVerticalSpacer;
        static qreal groupHorizontalSpacer;

        static qreal orphansVerticalSpacer;
        static qreal orphansHorizontalSpacer;

        static qreal treeLayerHorizontalSpacer;

        static qreal baseBlockWidth;
    };
};

#endif // APPSTYLE_H
