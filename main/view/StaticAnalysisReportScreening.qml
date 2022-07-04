import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Utils 1.0

Item {
    id:root;
    signal refresh;
    signal clearDot;
    property int range: currentProgram.params.commonParams.Range[1];
    anchors.fill: parent;

    Row
    {
        anchors.fill: parent
        AnalysisDiagram{
            width: height;height:parent.height;
            anchors.verticalCenter: parent.verticalCenter
            source:"/temp/Screening.bmp";
            range:root.range;
            onClicked:root.clearDot();
            Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
        }
    }
    Column
    {
        z:1;
        width: parent.width*0.3;height:parent.height*0.2
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Repeater
        {
            anchors.fill: parent;
            model:[
                {img:"qrc:/grays/SE0.bmp",text:":看见:"+(analysisResult.dotSeen+analysisResult.dotWeakSeen)+"/"+currentProgram.dots.length},
                {img:"qrc:/grays/SE2.bmp",text:":未看见:"+analysisResult.dotUnseen+"/"+currentProgram.dots.length},
                {img:"qrc:/grays/SE3.bmp",text:":盲点"}
            ]
            Row{
                height: parent.height*0.15;width: parent.width;spacing: height*0.2
                Image {height: sourceSize.height;width:sourceSize.width;anchors.verticalCenter: parent.verticalCenter;source: modelData.img}
                CusText{height: parent.height;text:modelData.text; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: height*0.50}
            }

        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
