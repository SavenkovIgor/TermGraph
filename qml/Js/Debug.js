/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

.pragma library

function properties(obj) {
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
