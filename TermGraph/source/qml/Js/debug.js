.pragma library

function properties(obj) {
    let propValue;
    for(let propName in obj) {
        propValue = obj[propName]

        print(propName, ":", propValue);
    }
}

function geometry(obj) {
    print("(%1 %2) [%3 %4]".arg(obj.x).arg(obj.y).arg(obj.width).arg(obj.height))
}
