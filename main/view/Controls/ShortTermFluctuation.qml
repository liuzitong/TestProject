import QtQuick 2.0


Canvas{
    id:canvas;
    antialiasing: false;
    smooth: false;
    height: 27;
    width: 322;

    property var dots:null;
    onDotsChanged: requestPaint();

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
        if(dots!==null)
        {
            var ctx = getContext("2d")
            console.log(dots.length);
            for(var i=1;i<dots.length;i++)
            {
                var temp=dots[i];
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
}

