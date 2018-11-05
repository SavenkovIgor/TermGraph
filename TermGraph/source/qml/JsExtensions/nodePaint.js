function prepareContext(ctx) {
    ctx.textBaseline = "middle"
    ctx.font = "10.5pt sans-serif"
}

function paintNode(ctx, text, rect, rectColor) {
    paintCustomizedNode(ctx, text, "#FFFFFF", rect, 5, rectColor, 2, "#000000")
}

// TODO: Optimize!
function paintCustomizedNode(ctx, text, textColor, rect, rectRadius, rectColor, borderWidth, borderColor) {

    ctx.save()

    // Draw border
    ctx.strokeStyle = borderColor
    ctx.lineWidth = borderWidth*2
    ctx.beginPath()
    ctx.roundedRect(rect.x, rect.y, rect.width, rect.height, rectRadius, rectRadius)
    ctx.stroke()

    // Draw inner rect with the same path
    ctx.fillStyle = rectColor
    ctx.fill()

    // Draw text

    ctx.fillStyle = textColor

    var txtW = ctx.measureText(text).width
    var rectCenterX = rect.x + rect.width / 2
    var rectCenterY = rect.y + rect.height / 2

    ctx.fillText(text, rectCenterX - txtW / 2, rectCenterY + borderWidth)

    ctx.restore()
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
