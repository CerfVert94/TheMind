function drawShuriken(ctx, shrkn, x, y, resize_percentage, num) {
    const shrknWidth       =  50 * resize_percentage,
          shrknHeight      =  50 * resize_percentage,
             font_px       =  32 * resize_percentage;
    
    ctx.drawImage(shrk, x, y, shrknWidth, shrknHeight);
    ctx.textAlign = "Center";
    ctx.textBaseline = "center";
    ctx.font = font_px.toString() + "px Arial";
    ctx.fillStyle   = "rgb(0, 0, 0)";
    ctx.fillText(  "Shuriken : " + num.toString(), x + shrknWidth + shrknWidth/ 32 , y + shrknHeight - shrknHeight / 4);//x + shrknWidth / 2, y);
}

function drawLife(ctx, life, x, y, resize_percentage, num) {
    const lifeWidth        =  41 * resize_percentage,
          lifeHeight       =  50 * resize_percentage,
             font_px       =  32 * resize_percentage;
    
    ctx.drawImage(life, x, y, lifeWidth, lifeHeight);
    ctx.textAlign = "Center";
    ctx.textBaseline = "center";
    ctx.font = font_px.toString() + "px Arial";
    ctx.fillStyle   = "rgb(0, 0, 0)";
    ctx.fillText(  "Life : " + num.toString(), x + lifeWidth + lifeWidth/ 32 , y + lifeHeight - lifeHeight / 4);//x + lifeWidth / 2, y);
}


function drawText(ctx, text,  x, y, bold, size, r,g,b) {

    ctx.textAlign = "Center";
    ctx.textBaseline = "center";
    if (bold == true)
        ctx.font = "bold " +  size.toString() + "px Arial";
    else
        ctx.font = size.toString() + "px Arial";
    // console.log(ctx.font + ";")

    ctx.fillStyle = "rgb(" + r.toString() + "," + g.toString() + "," + b.toString() + ")";
    ctx.fillText(text, x, y);


}
function drawCard(ctx, card, x, y, resize_percentage, num) {
    const cardWidth        = 128 * resize_percentage,
          cardHeight       = 176 * resize_percentage,
             font_px       =  24 * resize_percentage,
             numb_px       =  54 * resize_percentage,
             small_numb_px =  18 * resize_percentage;
    ctx.drawImage(card, x, y, cardWidth, cardHeight);
    ctx.textAlign = "Center";
    ctx.textBaseline = "center";
    if (num == 0) {
        ctx.font = "bold " + font_px.toString() + "px Arial";
        ctx.fillStyle   = "rgb(255, 100, 0)";
        ctx.strokeStyle = "rgb(255, 255, 255)";
        ctx.fillText(  "The Mind", x + cardWidth / 8 - cardWidth / 32, y + cardHeight / 2 + cardHeight / 16);
        ctx.strokeText("The Mind", x + cardWidth / 8 - cardWidth / 32, y + cardHeight / 2 + cardHeight / 16);
    }
    else{
        if (0 < num  && num  <= 9) {
            ctx.font = "bold " + numb_px.toString() + "px Arial";
            ctx.fillStyle   = "rgb(0, 0, 0)";
            ctx.fillText(  num.toString(), x + cardWidth / 2 - cardWidth / 4 + cardWidth / 8, y + cardHeight / 2 + cardHeight / 8 - cardHeight / 32 + cardHeight / 64);

            ctx.font = "bold " + small_numb_px.toString() + "px Arial";
            ctx.fillStyle   = "rgb(255, 255, 255)";
            ctx.fillText(  num.toString(), x + small_numb_px / 4        , y + small_numb_px);
            ctx.fillText(  num.toString(), x + cardWidth - cardWidth / 8 + cardWidth / 32, y + small_numb_px);
            ctx.fillText(  num.toString(), x + small_numb_px / 4        , y + cardHeight - + cardHeight / 32 );
            ctx.fillText(  num.toString(), x + cardWidth - cardWidth / 8 + cardWidth / 32, y + cardHeight - + cardHeight / 32 );
        }  
        else if (10 <= num  && num <= 99){
            ctx.font = "bold " + numb_px.toString() + "px Arial";
            ctx.fillStyle   = "rgb(0, 0, 0)";
            ctx.fillText(  num.toString(), x + cardWidth / 2 - cardWidth / 4 + cardWidth / 32, y + cardHeight / 2 + cardHeight / 8 - cardHeight / 32 + cardHeight / 64);

            ctx.font = "bold " + small_numb_px.toString() + "px Arial";
            ctx.fillStyle   = "rgb(255, 255, 255)";
            ctx.fillText(  num.toString(), x + small_numb_px / 4        , y + small_numb_px);
            ctx.fillText(  num.toString(), x + cardWidth - cardWidth / 4 + cardWidth / 16, y + small_numb_px);
            ctx.fillText(  num.toString(), x + small_numb_px / 4        , y + cardHeight - + cardHeight / 32 );
            ctx.fillText(  num.toString(), x + cardWidth - cardWidth / 4 + cardWidth / 16, y + cardHeight - + cardHeight / 32 );
        }   
        else if (num <= 100) {
            ctx.font = "bold " + numb_px.toString() + "px Arial";
            ctx.fillStyle   = "rgb(0, 0, 0)";
            ctx.fillText(  num.toString(), x + cardWidth / 2 - cardWidth / 4 - cardWidth / 8 + cardWidth / 32, y + cardHeight / 2 + cardHeight / 8 - cardHeight / 32 + cardHeight / 64);

            ctx.font = "bold " + small_numb_px.toString() + "px Arial";
            ctx.fillStyle   = "rgb(255, 255, 255)";
            ctx.fillText(  num.toString(), x + small_numb_px / 4        , y + small_numb_px);
            ctx.fillText(  num.toString(), x + cardWidth - cardWidth / 4, y + small_numb_px);
            ctx.fillText(  num.toString(), x + small_numb_px / 4        , y + cardHeight - + cardHeight / 32 );
            ctx.fillText(  num.toString(), x + cardWidth - cardWidth / 4, y + cardHeight - + cardHeight / 32 );
        }
        // ctx.strokeText("The Mind", x + cardWidth / 8, y + cardHeight / 2);
    }
}