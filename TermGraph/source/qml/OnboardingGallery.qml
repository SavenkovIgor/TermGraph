import QtQuick 2.5
import QtQuick.Controls 2.5

import "UIExtensions"

Page {
    id: onboarding

    property StackView mainStack

    function open() {
        mainStack.push(onboarding)
    }

    header: MainHeader {
        id: mainHeader
        titleText: "Справка"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: mainStack.pop()
    }

    SwipeView {
        id: swipeView

        currentIndex: 0
        anchors.fill: parent

        property real minWH: Math.min(width, height)

        OnBoardCard {
            id: tip1
            baseSize: swipeView.minWH
        }

        OnBoardCard {
            id: tip2
            baseSize: swipeView.minWH
        }

        OnBoardCard {
            id: tip3
            baseSize: swipeView.minWH
        }
    }

    PageIndicator {
        id: pageIndicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
