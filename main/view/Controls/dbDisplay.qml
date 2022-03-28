import QtQuick 2.0

Item{
    id:root;
    anchors.fill: parent;anchors.margins: parent.height*0.03;
    property alias displayCanvas: displayCanvas;
    property var dotList:[];
    function addDot(db,x,y,size)
    {
        dotList.push({db:db,x_Deg:x,y_Deg:y,size:size});
    }

    signal painted();

    Component.onCompleted: {addDot(39,33,33,14);addDot(55,-44,-44,14)}

    Canvas{
        id:displayCanvas;
        property int degreeRange: 30;
        anchors.fill: parent;
        property double diameter: 0.97*height;
        property double widthMargin: (width-height)/2+heightMargin;
        property double heightMargin:height*0.015;
        property double fontSize: diameter*0.020;
        property int pi: 0;

        function drawDashRound(x, y, radius, length)
        {
//            console.log("drawDsh");
            var step=length/radius
            for (var b = 0, e = step ; e <=Math.PI*2; b += step*2, e += step*2)
            {
                 var ctx = getContext("2d")
                ctx.lineWidth = 0;
                ctx.strokeStyle = "black";
                ctx.fillStyle="white";
                ctx.beginPath()
                ctx.arc(x, y, radius, b, e);
                ctx.stroke();
                ctx.closePath();
            }
        }

        function drawText(content,x_coord,y_coord,size)
        {
            var ctx = getContext("2d");
            ctx.textAlign = "center";
            ctx.font = size.toString()+"px sans-serif";
            ctx.fillStyle="white";
            ctx.fillRect(x_coord-size*0.7,y_coord-size*0.6,size*1.4,size*1.2)
            ctx.fillStyle="black";
            ctx.fillText(content, x_coord, y_coord+size*1/3);
        }

        function drawDB(dot)
        {
            var x_coord=(dot.x_Deg/90)*(diameter*0.5)+width/2;
            var y_coord=(-dot.y_Deg/90)*(diameter*0.5)+height/2;
            var db=dot.db;
            var size=dot.size;
            drawText(db,x_coord,y_coord,size);
        }

        //        function drawDot(dot)
        //        {
        //            var x_coord=(dot.x_Deg/90)*(diameter*0.5)+width/2;
        //            var y_coord=(-dot.y_Deg/90)*(diameter*0.5)+height/2;
        //            var radius=diameter/180;
        //            var ctx = getContext("2d");
        //            ctx.lineWidth = 0;
        //            ctx.strokeStyle = "red";
        //            ctx.beginPath();
        //            ctx.arc(x_coord, x_coord, radius, 0, Math.PI*2);
        //            ctx.stroke();
        //            ctx.closePath();
        //            context.fillStyle = "green";
        //            context.fill();
        //        }


        function drawAxisEndText(string,x_coord,y_coord,size)
        {
            var ctx = getContext("2d")
            drawText(string,x_coord,y_coord,size);
            ctx.strokeStyle="black";
            ctx.lineWidth=1;
            ctx.beginPath();
            ctx.moveTo(x_coord-size*0.7,y_coord-size*0.6);
            ctx.lineTo(x_coord-size*0.7,y_coord+size*0.6);
            ctx.closePath();
            ctx.stroke();
            ctx.moveTo(x_coord+size*0.7,y_coord-size*0.6);
            ctx.lineTo(x_coord+size*0.7,y_coord+size*0.6);
            ctx.closePath();
            ctx.stroke();
        }


        onPaint: {
            var i;
            for(i=3;i>=1;i--)
            {
                if(i!==3)
                     drawDashRound(width/2,height/2, diameter/6*i, 3)
                else{
                    var ctx = getContext("2d")
                    ctx.lineWidth = 0;
                    ctx.strokeStyle = "black";
                    ctx.fillStyle="white";
                    ctx.beginPath();
                    ctx.arc(width/2,height/2,diameter/6*i,0,Math.PI*2);
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
                        drawAxisEndText(degreeRange/3*Math.abs(i),widthMargin+(i+3)*diameter/6,height/2,fontSize);
                        drawAxisEndText(degreeRange/3*Math.abs(i),width/2,heightMargin+(i+3)*diameter/6,fontSize);
                    }
                    else
                    {
                        drawText(degreeRange/3*Math.abs(i),widthMargin+(i+3)*diameter/6,height/2,fontSize);
                        drawText(degreeRange/3*Math.abs(i),width/2,heightMargin+(i+3)*diameter/6,fontSize);
                    }
                }
            }
            dotList.forEach(function(item)
            {
//                console.log(item.x_Coord);
                drawDB(item);
            })
            root.painted();
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
