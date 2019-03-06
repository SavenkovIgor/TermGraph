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

        Pane {
            background: Rectangle {
                color: "#332f30"
            }

            Rectangle {
                id: tip1

                property real marginVal: swipeView.minWH * 0.03

                anchors.fill: parent

                anchors.topMargin: marginVal
                anchors.bottomMargin: marginVal
                anchors.leftMargin: marginVal
                anchors.rightMargin: marginVal

                radius: swipeView.minWH * 0.08
                color: "#6f6f6f"
            }
        }

        Pane {
            background: Rectangle {
                color: "#332f30"
            }

            Rectangle {
                id: tip2

                property real marginVal: swipeView.minWH * 0.03

                anchors.fill: parent

                anchors.topMargin: marginVal
                anchors.bottomMargin: marginVal
                anchors.leftMargin: marginVal
                anchors.rightMargin: marginVal

                radius: swipeView.minWH * 0.08
                color: "#6f6f6f"
            }
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
