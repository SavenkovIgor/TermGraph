function prepareRoundedRects(ctx) {
    ctx.strokeStyle = "#000000"
    ctx.lineWidth = 4
}

function paintRoundedRect(ctx, rect, rectColor, radius) {

    // Draw border
    ctx.beginPath()
    ctx.roundedRect(rect.x, rect.y, rect.width, rect.height, radius, radius)
    ctx.stroke()

    // Draw inner rect with the same path
    ctx.fillStyle = rectColor
    ctx.fill()
}

function prepareEdge(ctx, color) {
    ctx.strokeStyle = color
}

function paintLine(ctx, pt1, pt2) {
    ctx.beginPath()
    ctx.moveTo(pt1.x, pt1.y)
    ctx.lineTo(pt2.x, pt2.y)
    ctx.stroke()
}

function prepareText(ctx) {
    ctx.textBaseline = "middle"
    ctx.font = "10pt sans-serif"
    ctx.fillStyle = "#FFFFFF"
}

function paintGroupName(ctx, text, basePoint) {
    ctx.textBaseline = "middle"
    ctx.font = "bold 10.5pt sans-serif"

    ctx.fillStyle = "#FFFFFF"
    ctx.fillText(text, basePoint.x, basePoint.y)
}

function paintText(ctx, text, center, rect) {

    // Draw text
    var count = Object.keys(text).length

    if (count === 1) {
        var txtW = ctx.measureText(text[0]).width
        var posX = center.x - txtW / 2
        ctx.fillText(text[0], posX, center.y)
    }

    if (count === 2) {
        var heightQuater = rect.height / 4

        txtW = ctx.measureText(text[0]).width
        posX = center.x - txtW / 2
        ctx.fillText(text[0], posX, center.y - heightQuater)

        txtW = ctx.measureText(text[1]).width
        posX = center.x - txtW / 2
        ctx.fillText(text[1], posX, center.y + heightQuater)
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
