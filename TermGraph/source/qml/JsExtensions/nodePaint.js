function prepareRoundedRects(ctx) {
    ctx.strokeStyle = "#000000"
    ctx.lineWidth = 4
}

function paintRoundedRect(ctx, rect, rectColor) {

    // Draw border
    ctx.beginPath()
    //var cornerRadius = rect.height*0.18
    //ctx.roundedRect(rect.x, rect.y, rect.width, rect.height, cornerRadius, cornerRadius)
    ctx.roundedRect(rect.x, rect.y, rect.width, rect.height, 5, 5)
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

function paintText(ctx, text, rect) {

    // Draw text
    var txtW = ctx.measureText(text).width
    var rectCenterX = rect.x + rect.width / 2
    var rectCenterY = rect.y + rect.height / 2

    ctx.fillText(text, rectCenterX - txtW / 2, rectCenterY + 2)
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
