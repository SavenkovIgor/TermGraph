
function paintNode(ctx, x, y, text) {

    ctx.save()
    var txtW = ctx.measureText(text).width
    var txtH = ctx.measureText(text).height

    ctx.fillStyle = 'green'
    ctx.fillRect(10, 10, 100, 100)
    ctx.strokeRect(40, 40, 70, 70)

    ctx.restore()
}
