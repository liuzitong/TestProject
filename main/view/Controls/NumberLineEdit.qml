import QtQuick 2.0

import QtQuick.Controls 2.0

Item{
    id:root
//    height: 30;width: 100;
    property int radius:height/6;
    property int step;
    property alias text:textfeild.text;
    property alias verticalAlignment: textfeild.verticalAlignment;
    property alias horizontalAlignment: textfeild.horizontalAlignment;
    property alias readOnly: textfeild.readOnly;
    property alias backgroundColor: recbackground.color;
    property alias textfeild: textfeild;
    property int value;
    property int max;
    property int min;

    width: parent.width;
    height:parent.height;
    clip: true
    signal enterPressed();
    TextField{
        id:textfeild;
        anchors.fill: parent;
        text:value;
        horizontalAlignment: Text.AlignLeft
        renderType: Text.NativeRendering
        verticalAlignment: Text.AlignVCenter
        font.pointSize: parent.height*0.30
        font.family:"Microsoft YaHei"
        selectByMouse: false;
        selectionColor: "yellow"
        background: Rectangle {id:recbackground;border.width: 1;radius: root.radius;color:readOnly? "#e4e6e8":"white" ;border.color: "#bdc0c6"}
        Keys.onPressed: { switch (event.key){/*EnterKey*/ case 16777220:root.enterPressed();break;};
        }
        Canvas{
            height: parent.height
            anchors.right: parent.right
            width: height;
            onPaint: {
                var ctx = getContext("2d");
                ctx.strokeStyle=recbackground.border.color;
                ctx.lineWidth=0;
                ctx.beginPath();
                ctx.moveTo(0,0);
                ctx.lineTo(0,height);
                ctx.moveTo(0,height/2);
                ctx.lineTo(width,height/2);
                ctx.closePath();
                ctx.stroke();
                ctx.strokeStyle="black";
                ctx.fillStyle = "black";
                ctx.beginPath();
                ctx.moveTo(width/2-height*0.15,height/4+height*0.075);
                ctx.lineTo(width/2+height*0.15,height/4+height*0.075);
                ctx.lineTo(width/2,height/4-height*0.075);
                ctx.moveTo(width/2-height*0.15,3*height/4-height*0.075);
                ctx.lineTo(width/2+height*0.15,3*height/4-height*0.075);
                ctx.lineTo(width/2,3*height/4+height*0.075);
                ctx.closePath();
                ctx.stroke();
                ctx.fill();
            }
            Timer{
                id:timerUp;
                triggeredOnStart: true
                repeat: true
                interval: 30;
                running:false;
                onTriggered: {value++;if(value>max) value=max; textfeild.text=value;}
            }

            Timer{
                id:timerDown;
                triggeredOnStart: true
                repeat: true
                interval: 30;
                running:false;
                onTriggered: {value--;if(value<min) value=min;textfeild.text=value;}
            }


            MouseArea{
                height: parent.height/2
                width: parent.width;
                anchors.top:parent.top;
                anchors.right: parent.right;
                onClicked:{value++;if(value>max) value=max;textfeild.text=value;}
                onPressAndHold: timerUp.start();
                onReleased: timerUp.stop();

            }
            MouseArea{
                height: parent.height/2
                width: parent.width;
                anchors.bottom: parent.bottom;
                anchors.right: parent.right;
                onClicked:{value--;if(value<min) value=min;textfeild.text=value;}
                onPressAndHold: timerDown.start();
                onReleased: timerDown.stop();
            }
        }

    }


}


