/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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
