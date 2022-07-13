import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Utils 1.0

Row{
    id:root
    property string source: "";
    property string textTop: ""
    property string textBottom: ""
    property int range;
    signal refresh;
    signal clearDot;
    signal clicked;
    width: parent.height;height:parent.height;
    property int fontSize:width<=400?width/30:width/60;
//    anchors.horizontalCenter: parent.horizontalCenter;
    Column{
        width: parent.height*0.8;height:parent.height;
        CusText{text:root.textTop; anchors.horizontalCenter: parent.horizontalCenter;height:parent.height*0.1;font.pointSize: height*0.35;width: parent.width;}
        Canvas
        {
            property var dot: null;
            width:height;height:parent.height*0.8;

            Component.onCompleted:
            {
                root.clearDot.connect(function(){
                    console.log("cleardot");
                    dot=null;
                    requestPaint();
                })
            }

            function drawDot()
            {
                if(dot==null) return;
                var pix=IcUiQmlApi.appCtrl.AnalysisSvc.getPixFromPoint(dot,width,height);
                console.log(pix.x+" "+pix.y);
                var dotRadius=width/25;
                var ctx = getContext("2d");
                ctx.lineWidth = 0;
                ctx.strokeStyle = "blue";
                ctx.beginPath();
                ctx.arc(pix.x, pix.y, dotRadius, 0, Math.PI*2);
                ctx.stroke();
                ctx.closePath();
            }
            onPaint:
            {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height);
                drawDot();
            }

            MouseArea
            {
                height:parent.height;width:parent.height;
                onClicked: {
                    root.clicked();
                    parent.dot=IcUiQmlApi.appCtrl.AnalysisSvc.getClickDot(mouseX,mouseY,width,height);
                    parent.requestPaint();
                }

            }

            Image{
                z:-1;
                height:parent.height; smooth: false;width:height;cache: false;        //to refresh image
                Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + root.source;})}
            }
        }
        CusText{text:root.textBottom; anchors.horizontalCenter: parent.horizontalCenter;height:parent.height*0.1;font.pointSize: height*0.35;width: parent.width;}
    }
    Column{
        width: parent.height*0.15;height: parent.height;
        Item {
            width: parent.width;height:parent.height*0.1;
        }
        Item{
            width: parent.width;height:parent.height*0.8;
            CusText{width: parent.width;height:parent.height/10; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: root.fontSize;
            text: " ("+root.range+")"}
        }
        Item {
            width: parent.width;height:parent.height*0.1;}
    }
}

