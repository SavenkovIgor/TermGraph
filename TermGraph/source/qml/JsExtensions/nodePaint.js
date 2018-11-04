
function paintNode(ctx, text, rect, rectColor) {
    paintCustomizedNode(ctx, text, "#FFFFFF", rect, 5, rectColor, 1, "#000000")
}

function paintCustomizedNode(ctx, text, textColor, rect, rectRadius, rectColor, borderWidth, borderColor) {

    ctx.save()
    //var txtW = ctx.measureText(text).width
    var marginHorizontal = 10
    //var marginVertical = 5

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
    ctx.textBaseline = "middle"
    ctx.fillStyle = textColor
    ctx.font = "14px sans-serif"
    ctx.fillText(text, rect.x + marginHorizontal, rect.y + rect.height / 2)

    ctx.restore()
}
