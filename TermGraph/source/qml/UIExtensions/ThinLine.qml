import QtQuick 2.0

Rectangle {
    id: root

    enum LineSide {
        Top = 0,
        Right = 1,
        Bottom = 2,
        Left = 3
    }

    property Item target: null
    property int side: LineSide.Bottom

    property QtObject __private: QtObject {
        property bool horizontal: root.side === ThinLine.LineSide.Top || root.side === ThinLine.LineSide.Bottom
    }

    x: side === ThinLine.LineSide.Right ? target.width - width : 0
    y: side === ThinLine.LineSide.Bottom ? target.height - height : 0

    height: __private.horizontal ? Math.floor(Math.max(target.height / 100.0, 1)) : target.height
    width:  __private.horizontal ? target.width  : Math.floor(Math.max(target.width / 100.0, 1))
}
