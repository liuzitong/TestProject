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
    anchors.fill: parent;
    property int range: currentProgram.params.commonParams.Range[1];
    property var analysisResult: null;
    property var analysisVm: null;
    onRefresh: clearDot();

    AnalysisDiagram{
        width: height;height:parent.height*0.8*1.25;
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source:"/previewImage/dBDiagram.bmp";
        textTop:"阈值(dB)";
        analysisVm: root.analysisVm;
        range:root.range;
        onClicked:root.clearDot();
        Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
