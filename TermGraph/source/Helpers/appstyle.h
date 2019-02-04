#ifndef APPSTYLE_H
#define APPSTYLE_H

#include <QColor>

class AppStyle
{
public:
    class Colors {
    public:
        inline static auto transparent = QColor(0,0,0,0);
        inline static auto testColor = QColor(0, 255, 0);

        class Nodes {
        public:
            inline static auto border = QColor(0, 0, 0);
            inline static auto text = QColor(255, 255, 255);

            inline static auto leaf = QColor(109, 154, 40);
            inline static auto root = QColor(128, 41, 21);
            inline static auto orphan = QColor(18, 75, 106);

            inline static auto leafSelected = QColor(131, 195, 27);
            inline static auto rootSelected = QColor(220, 76, 29);
            inline static auto orphanSelected = QColor(11, 119, 177);

            inline static auto atLearn = QColor(245, 222, 179);
        };

        class Groups {
        public:
            inline static auto border = QColor(255, 255, 255);
            inline static auto text = QColor(255, 255, 255);
            inline static auto backgroundTerms = QColor(0, 0, 0, 0);
            inline static auto backgroundFreeConnections = QColor(0, 0, 0, 0);
        };

        class Edges {
        public:
            inline static auto standard = QColor(Qt::black);
            inline static auto termin = QColor(34, 91, 122);
            inline static auto description = QColor(Qt::green);
            inline static auto selected = QColor(0, 166, 147);
            inline static auto broken = Qt::red;
        };

        class Scene {
        public:
            inline static auto background = QColor(51, 47, 48);
        };
    };

    class Sizes {
    public:
        static constexpr qreal groupVerticalSpacer = 10.0;
        static constexpr qreal groupHorizontalSpacer = 10.0;

        static constexpr qreal orphansVerticalSpacer = 10.0;
        static constexpr qreal orphansHorizontalSpacer = 10.0;

        static constexpr qreal treeLayerHorizontalSpacer = 15.0;

        static constexpr qreal baseBlockWidth = 40.0;
    };
};

#endif // APPSTYLE_H
