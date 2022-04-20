import QtQuick 2.0


Canvas{
    id:canvas;
    antialiasing: false;
    smooth: false;
    height: 27;
    width: 322;


    function convertY(p){ return 13-p;}

    function drawBackGround(){
        var ctx = getContext("2d")
        ctx.strokeStyle="black";
        ctx.lineWidth=0;
        ctx.beginPath();
        ctx.moveTo(0,0);
        ctx.lineTo(0,27);
        ctx.stroke();
        ctx.moveTo(width-1,0);
        ctx.lineTo(width-1,27)
        ctx.stroke();
        ctx.moveTo(0,13);
        ctx.lineTo(width,13)
        ctx.stroke();
    }

    onPaint:
    {
        drawBackGround()
        var ctx = getContext("2d")
        for(var i=1;i<width-2;i++)
        {
            var temp=Math.random()*20-10;
            temp=convertY(temp)
//            console.log(temp);
            ctx.strokeStyle="black";
            ctx.lineWidth=0;
            ctx.beginPath();
            ctx.moveTo(i,13);
            ctx.lineTo(i,temp);
            ctx.closePath();
            ctx.stroke();
        }
    }
}

