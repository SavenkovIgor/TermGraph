function prepareRects(ctx) {
    ctx.strokeStyle = "#000000"
    ctx.lineWidth = 2
    ctx.textBaseline = "middle"
    ctx.font = "10pt sans-serif Arial"
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

function prepareEdge(ctx) {
    ctx.lineWidth = 2
}

function paintLine(ctx, pt1, pt2, color) {
    ctx.strokeStyle = color
    ctx.beginPath()
    ctx.moveTo(pt1.x, pt1.y)
    ctx.lineTo(pt2.x, pt2.y)
    ctx.stroke()
}

function paintGroupName(ctx, text, basePoint) {
    ctx.textBaseline = "middle"
    ctx.font = "bold 10.5pt sans-serif Arial"

    ctx.fillStyle = "#FFFFFF"
    ctx.fillText(text, basePoint.x, basePoint.y)
}

function paintTextWithSplit(ctx, text, center, rect) {

    ctx.fillStyle = "#FFFFFF"
    // Draw text
    var txtList = text.split('\n');

    var count = Object.keys(txtList).length

    if (count === 1) {
        var txtW = ctx.measureText(txtList[0]).width
        var posX = center.x - txtW / 2
        ctx.fillText(txtList[0], posX, center.y)
    }

    if (count === 2) {
        var heightQuater = rect.height / 4

        txtW = ctx.measureText(txtList[0]).width
        posX = center.x - txtW / 2
        ctx.fillText(txtList[0], posX, center.y - heightQuater)

        txtW = ctx.measureText(txtList[1]).width
        posX = center.x - txtW / 2
        ctx.fillText(txtList[1], posX, center.y + heightQuater)
    }
}

function paintRect(ctx, rect, borderColor) {
    ctx.strokeStyle = borderColor
    ctx.lineWidth = 1
    ctx.beginPath()
    ctx.rect(rect.x, rect.y, rect.width, rect.height)
    ctx.stroke()
}

function clearRect(ctx, rect, offset) {
    ctx.clearRect(rect.x - offset, rect.y - offset, rect.width + 2*offset, rect.height + 2*offset)
}

function paintFilledRect(ctx, rect, color) {
    ctx.fillStyle = color
    ctx.fillRect(rect.x, rect.y, rect.width, rect.height)
}
