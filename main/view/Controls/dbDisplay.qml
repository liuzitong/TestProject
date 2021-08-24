import QtQuick 2.0

Item{
    id:root;
    anchors.fill: parent;anchors.margins: parent.height*0.03;
    property alias displayCanvas: displayCanvas;
    Canvas{
        id:displayCanvas;
        property int degreeRange: 30;
        anchors.fill: parent;
        property double diameter: 0.98*height;
        property double widthMargin: (width-height)/2+height*0.01;
        property double heightMargin:height*0.01;

        function drawDashRound(x, y, radius, length)
        {
            var step=length/radius
            for (var b = 0, e = step ; e <=Math.PI*2; b += step*2, e += step*2)
            {
              var ctx = getContext("2d")
              ctx.beginPath()
              ctx.arc(x, y, radius, b, e);
              ctx.stroke();
              ctx.closePath();
            }
        }

        function drawText(string,x_degree,y_degree)
        {
            var ctx = getContext("2d")
            ctx.textAlign = "center";
            ctx.font = "12px sans-serif";
            ctx.fillStyle="white";
            ctx.fillRect(x_degree-8,y_degree-6,16,12)
            ctx.fillStyle="black";
            ctx.fillText(string, x_degree, y_degree+4);
        }

        function drawAxisEndText(string,x_degree,y_degree)
        {
            var ctx = getContext("2d")
            drawText(string,x_degree,y_degree);
            ctx.strokeStyle="black";
            ctx.lineWidth=1;
            ctx.beginPath();
            ctx.moveTo(x_degree-8,y_degree-6);
            ctx.lineTo(x_degree-8,y_degree+6);
            ctx.closePath();
            ctx.stroke();
            ctx.moveTo(x_degree+8,y_degree-6);
            ctx.lineTo(x_degree+8,y_degree+6);
            ctx.closePath();
            ctx.stroke();
        }


        onPaint: {
            var ctx = getContext("2d")
            ctx.lineWidth = 0;
            ctx.strokeStyle = "black";
            ctx.fillStyle="white";
            var i;
            for(i=3;i>=1;i--)
            {
                if(i!==3)
                     drawDashRound(width/2,height/2, height*0.98/6*i, 3)
                else{
                    ctx.beginPath();
                    ctx.arc(width/2,height/2,height*0.98/6*i,0,Math.PI*2);
                    ctx.closePath();
                    ctx.stroke();
                    ctx.fill();
                }
            }
            ctx.beginPath();
            ctx.moveTo(widthMargin,height/2);
            ctx.lineTo(widthMargin+diameter,height/2);
            ctx.closePath();
            ctx.stroke();
            ctx.beginPath();
            ctx.moveTo(width/2,heightMargin);
            ctx.lineTo(width/2,heightMargin+diameter);
            ctx.closePath();
            ctx.stroke();
            for(i=-3;i<=3;i++)
            {
                if(i!==0)
                {
                    if(Math.abs(i)===3)
                    {
                        drawAxisEndText(degreeRange/3*Math.abs(i),widthMargin+(i+3)*diameter/6,height/2);
                        drawAxisEndText(degreeRange/3*Math.abs(i),width/2,heightMargin+(i+3)*diameter/6);
                    }
                    else
                    {
                        drawText(degreeRange/3*Math.abs(i),widthMargin+(i+3)*diameter/6,height/2);
                        drawText(degreeRange/3*Math.abs(i),width/2,heightMargin+(i+3)*diameter/6);
                    }
                }
            }
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
