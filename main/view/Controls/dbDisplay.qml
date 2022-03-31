import QtQuick 2.0

Item{
    id:root;
    anchors.fill: parent;anchors.margins: parent.height*0.03;
    property alias range: displayCanvas.degreeRange;
    property alias displayCanvas: displayCanvas;
    property var dBList:[];
    property var dotList:[];
    property int type;

//    function hello(){
//        console.log("hello.");
//        dotList.forEach(function(item){console.log(item.x+" "+item.y);})
//        displayCanvas.requestPaint();
//    }

//    function addDB(db,x,y)
//    {
//        dBList.push({db:db,x:x,y:y});
//    }

    signal painted();

    Component.onCompleted:
    {
//        addDB(11,22,33);
//        addDB(15,-20,-44);
    }

    Canvas{
        id:displayCanvas;
        property int degreeRange: 0;
        anchors.fill: parent;
        property double diameter: 0.97*height;
        property double widthMargin: (width-height)/2+heightMargin;
        property double heightMargin:height*0.015;
        property int fontSize: diameter*0.022;
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

        function drawText(content,x_coord,y_coord)
        {
            var ctx = getContext("2d");
            ctx.textAlign = "center";
            ctx.font = fontSize.toString()+"px sans-serif";
            ctx.fillStyle="white";
            ctx.fillRect(x_coord-fontSize*0.7,y_coord-fontSize*0.6,fontSize*1.4,fontSize*1.2)
            ctx.fillStyle="black";
            ctx.fillText(content, x_coord, y_coord+fontSize*1/3);
        }

        function drawDB(dot)
        {
            var x_coord=(dot.x/degreeRange)*(diameter*0.5)+width/2;
            var y_coord=(-dot.y/degreeRange)*(diameter*0.5)+height/2;
            var db=dot.db;
            var size=dot.size;
            drawText(db,x_coord,y_coord,size);
        }

        function drawDot(dot)
        {
            var x;
            var y;
            if(type==2)
            {
                var radius=dot.x;
                var angle=dot.y;
                x=radius*Math.cos(angle/180*Math.PI);
                y=radius*Math.sin(angle/180*Math.PI);
            }
            else{
                x=dot.x;
                y=dot.y;
            }

            var x_coord=(x/degreeRange)*(diameter*0.5)+width/2;
            var y_coord=(-y/degreeRange)*(diameter*0.5)+height/2;

            var dotRadius=diameter/180*1.5;
            var ctx = getContext("2d");
            ctx.lineWidth = 0;
            ctx.strokeStyle = "red";
            ctx.beginPath();
            ctx.arc(x_coord, y_coord, dotRadius, 0, Math.PI*2);
            ctx.stroke();
            ctx.closePath();
            context.fillStyle = "green";
            context.fill();
        }


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


        onPaint:
        {
            if(degreeRange==0) return;
            var ctx = getContext("2d")
            ctx.fillStyle = "#cbced0";
            ctx.fillRect(0, 0, width, height);
            var degreeStart=degreeRange%Math.ceil(degreeRange/3);
            var degreeStep;
            if(degreeStart==0){degreeStart=degreeStep=degreeRange/3;}
            else{degreeStep=(degreeRange-degreeStart)/2;}

            console.log("start:"+degreeStart+"  step:"+degreeStep);
            var i;
            for(i=3;i>=1;i--)
            {
                if(i!==3)
                     drawDashRound(width/2,height/2,(degreeStart+degreeStep*(i-1))/degreeRange*diameter/2, 3)
                else{
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
//                        drawText(degreeRange/3*Math.abs(i),widthMargin+(i+3)*diameter/6,height/2,fontSize);
//                        drawText(degreeRange/3*Math.abs(i),width/2,heightMargin+(i+3)*diameter/6,fontSize);
                        var degree=degreeStart+degreeStep*(Math.abs(i)-1);
                        var coord=degree/degreeRange*diameter/2;
                        if(i<-0) coord=-coord;
                        drawText(degree,width/2+coord,height/2,fontSize);
                        drawText(degree,width/2,height/2+coord,fontSize);
                    }
                }
            }
            dBList.forEach(function(item){drawDB(item);})
            dotList.forEach(function(item){drawDot(item);})
            root.painted();
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
