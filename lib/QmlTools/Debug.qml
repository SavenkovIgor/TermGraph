// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton

import QtQml

QtObject {

    function properties(obj) {
        print("Type: ", typeof obj)
        let propValue;
        for(let propName in obj) {
            propValue = obj[propName]

            print(propName, ":", propValue);
        }
    }

    function propertiesStr(obj) {
        let propValue;
        let retStr = "";
        for(let propName in obj) {
            propValue = obj[propName]

            retStr += propName + ":" + propValue + "\n";
        }
        return retStr;
    }

    function geometry(obj) {
        print("(%1 %2) [%3 %4]".arg(obj.x).arg(obj.y).arg(obj.width).arg(obj.height))
    }

    function randomColor() {
        let colors = ["tan", "teal", "coral", "tomato", "salmon", "thistle", "steelblue",
                      "rosybrown", "sandybrown", "lightslategrey", "lightsteelblue", "mediumseagreen"];

        if (typeof randomColor.randomIndex == 'undefined') { // static variable
            randomColor.randomIndex = Math.floor(Math.random() * colors.length);
        }

        randomColor.randomIndex++;
        randomColor.randomIndex %= colors.length;
        return colors[randomColor.randomIndex];
    }

    function getRandomInt(min, max) {
        min = Math.ceil(min);
        max = Math.floor(max);
        return Math.floor(Math.random() * (max - min + 1)) + min;
    }

    function easingToString(easing) {
        switch (easing) {
        case Easing.Linear: return "Linear";
        case Easing.InQuad: return "InQuad";
        case Easing.OutQuad: return "OutQuad";
        case Easing.InOutQuad: return "InOutQuad";
        case Easing.OutInQuad: return "OutInQuad";
        case Easing.InCubic: return "InCubic";
        case Easing.OutCubic: return "OutCubic";
        case Easing.InOutCubic: return "InOutCubic";
        case Easing.OutInCubic: return "OutInCubic";
        case Easing.InQuart: return "InQuart";
        case Easing.OutQuart: return "OutQuart";
        case Easing.InOutQuart: return "InOutQuart";
        case Easing.OutInQuart: return "OutInQuart";
        case Easing.InQuint: return "InQuint";
        case Easing.OutQuint: return "OutQuint";
        case Easing.InOutQuint: return "InOutQuint";
        case Easing.OutInQuint: return "OutInQuint";
        case Easing.InSine: return "InSine";
        case Easing.OutSine: return "OutSine";
        case Easing.InOutSine: return "InOutSine";
        case Easing.OutInSine: return "OutInSine";
        case Easing.InExpo: return "InExpo";
        case Easing.OutExpo: return "OutExpo";
        case Easing.InOutExpo: return "InOutExpo";
        case Easing.OutInExpo: return "OutInExpo";
        case Easing.InCirc: return "InCirc";
        case Easing.OutCirc: return "OutCirc";
        case Easing.InOutCirc: return "InOutCirc";
        case Easing.OutInCirc: return "OutInCirc";
        case Easing.InElastic: return "InElastic";
        case Easing.OutElastic: return "OutElastic";
        case Easing.InOutElastic: return "InOutElastic";
        case Easing.OutInElastic: return "OutInElastic";
        case Easing.InBack: return "InBack";
        case Easing.OutBack: return "OutBack";
        case Easing.InOutBack: return "InOutBack";
        case Easing.OutInBack: return "OutInBack";
        case Easing.InBounce: return "InBounce";
        case Easing.OutBounce: return "OutBounce";
        case Easing.InOutBounce: return "InOutBounce";
        case Easing.OutInBounce: return "OutInBounce";
        }
        return "";
    }
}
