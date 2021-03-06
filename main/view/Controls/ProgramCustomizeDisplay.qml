import QtQuick 2.0

Item{
    id:root;
    anchors.fill: parent;anchors.margins: parent.height*0.03;
    property alias range: displayCanvas.degreeRange;
    property alias displayCanvas: displayCanvas;
    property var dotList:[];
    property int type;


    signal painted();
//    CusText{text:qsTr("点坐标");z:1; anchors.top: parent.top; anchors.topMargin: 0.05*parent.height; anchors.left: parent.left; anchors.leftMargin: 0.05*parent.width;width: parent.width*0.06;height: parent.height*0.05;}
//    CusText{text:"(-90,71)"; z:1;anchors.top: parent.top; anchors.topMargin: 0.08*parent.height; anchors.left: parent.left; anchors.leftMargin: 0.05*parent.width;width: parent.width*0.06;height: parent.height*0.05;}
//    Component.onCompleted:
//    {
//        addDB(11,22,33);
//        addDB(15,-20,-44);
//        console.log("display create");
//        console.log("display create");
//        console.log("display create");
//        console.log("display create");
//    }

    Canvas{
        id:displayCanvas;
        property int degreeRange: 0;
        anchors.fill: parent;
        property double diameter: height-heightMargin*2;
        property double widthMargin: /*(width-height)/2+heightMargin;*/(width-diameter)/2;
        property double heightMargin:height*0.015;
        property int fontSize: diameter*0.022;
        property int pi: 0;
//        property point mouseCoord;

        MouseArea{
            anchors.fill: parent;
            onClicked:
            {
//                console.log(mouseX);
//                console.log(mouseY);
//                console.log("aa");
//                displayCanvas.drawText("QQ",mouseX,mouseY);
//                displayCanvas.mouseCoord.x=mouseX;
//                displayCanvas.mouseCoord.y=mouseY;
//                console.log(displayCanvas.mouseCoord.x);
//                console.log(displayCanvas.mouseCoord.y);
//                var dot = displayCanvas.pixCoordToDot(mouseX,mouseY)

                var dot = displayCanvas.pixCoordToDot(mouseX,mouseY)
                console.log(dot.x+" "+dot.y);
                if(type==2){dot=displayCanvas.orthToPolar(dot)}
                console.log(dot.x+" "+dot.y);
                dotList.push(dot);
                root.dotListChanged();
                displayCanvas.requestPaint();
            }

        }

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

        function drawText(content,x_pix,y_pix)
        {
            var ctx = getContext("2d");
            ctx.textAlign = "center";
            ctx.font = fontSize.toString()+"px sans-serif";
            ctx.fillStyle="white";
            ctx.fillRect(x_pix-fontSize*0.7,y_pix-fontSize*0.6,fontSize*1.4,fontSize*1.2)
            ctx.fillStyle="black";
            ctx.fillText(content, x_pix, y_pix+fontSize*1/3);
        }

//        function drawDB(dot)
//        {
//            var x_coord=(dot.x/degreeRange)*(diameter*0.5)+width/2;
//            var y_coord=(-dot.y/degreeRange)*(diameter*0.5)+height/2;
//            var db=dot.db;
//            var size=dot.size;
//            drawText(db,x_coord,y_coord,size);
//        }


        function pixCoordToDot(pix_x,pix_y)
        {
            var pix_coordX=pix_x-width/2;
            var pix_coordY=-(pix_y-height/2);
            var dot_x=Math.round(pix_coordX/(diameter/2)*degreeRange);
            var dot_y=Math.round(pix_coordY/(diameter/2)*degreeRange);
            return {x:dot_x,y:dot_y};
        }

        function polarToOrth(dot)
        {
            var radius=dot.x;
            var angle=dot.y;
            return {x:radius*Math.cos(angle/180*Math.PI),y:radius*Math.sin(angle/180*Math.PI)}
        }

        function orthToPolar(dot)
        {
            console.log(dot.x+"  "+dot.y);
            var radius=Math.sqrt(Math.pow(dot.x,2)+Math.pow(dot.y,2));
            console.log(radius);
            var rad=Math.asin(dot.y/radius);
            console.log(rad);
            var angle=rad*(180/Math.PI);
            console.log(angle);
            if(dot.x<0)
            {
                if(dot.y>0){angle=90+(90-angle)}
                if(dot.y<0){angle=-90-(90+angle)}
            }

            console.log(angle);
//            if(dot.x>0&&dot.y<0) angle=angle+90;
//            if(dot.x<0&&dot.y<0
            if(angle<0) angle+=360;
            return {x:radius,y:angle}
        }

        function drawDot(dot)
        {
            var orthCoord;
            if(type==2) {orthCoord=polarToOrth(dot)}else{orthCoord=dot}

            var x_pix=(orthCoord.x/degreeRange)*(diameter*0.5)+width/2;
            var y_pix=(-orthCoord.y/degreeRange)*(diameter*0.5)+height/2;

            var dotRadius=diameter/180*1.5;
            var ctx = getContext("2d");
            ctx.lineWidth = 0;
            ctx.strokeStyle = "red";
            ctx.beginPath();
            ctx.arc(x_pix, y_pix, dotRadius, 0, Math.PI*2);
            ctx.stroke();
            ctx.closePath();
            ctx.fillStyle = "green";
            ctx.fill();
        }


        function drawAxisEndText(string,x_pix,y_pix,size)
        {
            var ctx = getContext("2d")
            drawText(string,x_pix,y_pix,size);
            ctx.strokeStyle="black";
            ctx.lineWidth=1;
            ctx.beginPath();
            ctx.moveTo(x_pix-size*0.7,y_pix-size*0.6);
            ctx.lineTo(x_pix-size*0.7,y_pix+size*0.6);
            ctx.closePath();
            ctx.stroke();
            ctx.moveTo(x_pix+size*0.7,y_pix-size*0.6);
            ctx.lineTo(x_pix+size*0.7,y_pix+size*0.6);
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

//            console.log("start:"+degreeStart+"  step:"+degreeStep);
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
                        drawAxisEndText(degreeRange,widthMargin+(i+3)*diameter/6,height/2,fontSize);
                        drawAxisEndText(degreeRange,width/2,heightMargin+(i+3)*diameter/6,fontSize);
                    }
                    else
                    {
                        var degree=degreeStart+degreeStep*(Math.abs(i)-1);
                        var pix=degree/degreeRange*diameter/2;
                        if(i<-0) pix=-pix;
                        drawText(degree,width/2+pix,height/2,fontSize);
                        drawText(degree,width/2,height/2+pix,fontSize);
                    }
                }
            }
//            drawText("o",mouseCoord.x,mouseCoord.y,fontSize);
            dotList.forEach(function(item){drawDot(item);})
            root.painted();
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
