function prepareRoundedRects(ctx) {
    ctx.strokeStyle = "#000000"
    ctx.lineWidth = 4
}

function paintRoundedRect(ctx, rect, rectColor, radius) {

    // Draw border
    ctx.beginPath()
    //var cornerRadius = rect.height*0.18
    //ctx.roundedRect(rect.x, rect.y, rect.width, rect.height, cornerRadius, cornerRadius)
    ctx.roundedRect(rect.x, rect.y, rect.width, rect.height, radius, radius)
    ctx.stroke()

    // Draw inner rect with the same path
    ctx.fillStyle = rectColor
    ctx.fill()
}

function prepareText(ctx) {
    ctx.textBaseline = "middle"
    ctx.font = "10.5pt sans-serif"
    ctx.fillStyle = "#FFFFFF"
}

function paintGroupName(ctx, text, basePoint) {
    ctx.textBaseline = "middle"
    ctx.font = "bold 10.5pt sans-serif"

    ctx.fillStyle = "#FFFFFF"
    ctx.fillText(text, basePoint.x, basePoint.y)
}

function paintText(ctx, text, rect) {

    // Draw text
    var txtW = ctx.measureText(text).width
    var rectCenterX = rect.x + rect.width / 2
    var rectCenterY = rect.y + rect.height / 2

    var posX = rectCenterX - txtW / 2
    var posY = rectCenterY + 2

    var count = Object.keys(text).length

    if (count === 1) {
        ctx.fillText(text[0], posX, posY)
    }

    if (count === 2) {
        ctx.fillText(text[0], posX, posY-4)
        ctx.fillText(text[1], posX, posY+4)
    }
}

function paintRect(ctx, rect, borderColor) {
    ctx.save()
    ctx.strokeStyle = borderColor
    ctx.lineWidth = 1
    ctx.beginPath()
    ctx.rect(rect.x, rect.y, rect.width, rect.height)
    ctx.stroke()
    ctx.restore()
}

function paintFilledRect(ctx, rect, color) {
    ctx.save()
    ctx.fillStyle = color
    ctx.fillRect(rect.x, rect.y, rect.width, rect.height)
    ctx.restore()
}
