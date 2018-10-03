
function paintNode(ctx, text, xCenter, yCenter, rectColor) {
    paintCustomizedNode(ctx, text, "#000000", xCenter, yCenter, 5, rectColor, 1, "#000000")
}

function paintCustomizedNode(ctx, text, textColor, xCenter, yCenter, rectRadius, rectColor, borderWidth, borderColor) {

    ctx.save()
    var txtW = ctx.measureText(text).width
    var txtH = 20
    var marginHorizontal = 10
    var marginVertical = 5

    // Full rectWidth includes 2 margins and 2 borders
    var rectWidth  = txtW + 2*marginHorizontal
    var rectHeight = txtH + 2*marginVertical

    var x = xCenter - rectWidth/2
    var y = yCenter - rectHeight/2

//    console.log(text)
//    console.log(txtW + " " + txtH)
//    console.log(xCenter + " " + yCenter)

    // Draw border
    ctx.strokeStyle = borderColor
    ctx.lineWidth = borderWidth*2
    ctx.beginPath()
    ctx.roundedRect(x, y, rectWidth, rectHeight, rectRadius, rectRadius)
    ctx.stroke()

    // Draw inner rect with the same path
    ctx.fillStyle = rectColor
    ctx.fill()

    // Draw text
    ctx.textBaseline = "middle"
    ctx.fillStyle = textColor
    ctx.fillText(text, x + marginHorizontal, yCenter)

    ctx.restore()
}
