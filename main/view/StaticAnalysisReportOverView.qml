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
    property var analysisResult: null;
    property var analysisVm: null;

    anchors.fill: parent;
    Column{anchors.fill: parent;spacing:height*0.2
        Row{ id: row;width: parent.width;height:parent.height*0.8/2;
            AnalysisDiagram{
                width: parent.height;height:parent.height;
                textTop:"阈值(dB)"
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/dbDiagram.bmp";
                range:root.range;
                onClicked:root.clearDot();
                analysisVm: root.analysisVm;
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }
            AnalysisDiagram{
                width: parent.height;height:parent.height;
                textTop:"灰度图"
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/gray.bmp";
                range:root.range;
                onClicked:root.clearDot();
                analysisVm: root.analysisVm;
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }

            Column {
                height:parent.height*0.8;width:parent.width-parent.height*2
                Item{width:parent.width;height:parent.height*0.2;}
                CusText{width:parent.width;height:parent.height*0.12;text:"VFI:"+Math.round(analysisResult.VFI*100)+"%" ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
                Item{width:parent.width;height:parent.height*0.1;}
                CusText{
                property var ght: if(analysisResult.GHT===0){return "超出界限";} else if(analysisResult.GHT===1){return "普遍敏感度降低";}else if(analysisResult.GHT===2){return "边界";}else if(analysisResult.GHT===3){return "正常范围";}
                width:parent.width;height:parent.height*0.12;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40;text:"青光眼半视野检查: "+ ght;}
                Item{width:parent.width;height:parent.height*0.1;}
                CusText{width:parent.width;height:parent.height*0.12;text:"平均缺损: "+analysisResult.md.toFixed(2)+(analysisResult.p_md>0?" (<"+analysisResult.p_md+"%)":"") ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
                CusText{width:parent.width;height:parent.height*0.12;text:"模式标准偏差: "+analysisResult.psd.toFixed(2)+(analysisResult.p_psd>0?" (<"+analysisResult.p_psd+"%)" :"");horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
            }

        }



        Row{
            width: parent.width;height:parent.height*0.8/2;
            AnalysisDiagram{
                width: parent.height;height:parent.height;
                textTop:"总体偏差"
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/TotalPE.bmp";
                range:root.range;
                onClicked:root.clearDot();
                analysisVm: root.analysisVm;
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }
            AnalysisDiagram{
                width: parent.height;height:parent.height;
                textTop:"模式偏差"
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/PatternPE.bmp";
                range:root.range;
                onClicked:root.clearDot();
                analysisVm: root.analysisVm;
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }

        }
    }

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
