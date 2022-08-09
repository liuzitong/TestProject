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
    Column{anchors.fill: parent;/*spacing:height*0.035*/
        Row{ id: row;width: parent.width;height:parent.height*0.97/3;
            Item{width: parent.width*0.18;height: parent.height;}
            AnalysisDiagram{
                width: parent.height;height:parent.height*1.25;
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/dbDiagram.bmp";
                range:root.range;
                onClicked:
                {
                    root.clearDot();
//                    dot=IcUiQmlApi.appCtrl.analysisSvc.getClickDot(mouseX,mouseY,width,height);
//                    canvas.requestPaint();
                }
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }
            Item{width: parent.width*0.1;height: parent.height;}
            AnalysisDiagram{
                width: parent.height;height:parent.height*1.25;
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/gray.bmp";
                range:root.range;
                onClicked:root.clearDot();
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }
//            Item{
//                width:parent.width*0.72-parent.height*2;height:parent.height;
//                CusText{ width:parent.width;height:parent.height*0.15;verticalAlignment: Text.AlignVCenter;
//                    text: "    ("+currentProgram.params.commonParams.Range[1]+")" ;
//                    horizontalAlignment: Text.AlignLeft; anchors.left: parent.left; anchors.leftMargin: 0;anchors.verticalCenter: parent.verticalCenter}
//            }
        }

        Row{
            width: parent.width;height:parent.height*0.97/3;spacing:width*0.05
            AnalysisDiagram{
                width: parent.height;height:parent.height*1.25;
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/TotalDeviation.bmp";
                range:root.range;
                onClicked:root.clearDot();
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }

            AnalysisDiagram{
                width: parent.height;height:parent.height*1.25;
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/PatterDeviation.bmp";
                range:root.range;
                onClicked:root.clearDot();
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }

            Column {
                height:parent.height;width:parent.width*0.9-parent.height*2
                Item{width:parent.width;height:parent.height*0.2;}
                CusText{width:parent.width;height:parent.height*0.12;text:"VFI:"+Math.round(analysisResult.VFI*100)+"%" ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
                Item{width:parent.width;height:parent.height*0.1;}
                CusText{
                property var ght: if(analysisResult.GHT===0){return "超出界限";} else if(analysisResult.GHT===1){return "普遍敏感度降低";}else if(analysisResult.GHT===2){return "边界";}else if(analysisResult.GHT===3){return "正常范围";}
                width:parent.width;height:parent.height*0.12;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40;text:"青光眼半视野检查: "+ ght;}
                Item{width:parent.width;height:parent.height*0.1;}
                CusText{width:parent.width;height:parent.height*0.12;text:"平均缺损: "+analysisResult.md.toFixed(2)+" (<"+analysisResult.p_md+"%)" ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
                CusText{width:parent.width;height:parent.height*0.12;text:"模式标准偏差: "+analysisResult.psd.toFixed(2)+" (<"+analysisResult.p_psd+"%)" ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
            }
        }
        Row{width: parent.width;height:parent.height*0.03;spacing:width*0.05;
            CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "总体偏差"; verticalAlignment: Text.AlignVCenter; font.pointSize: height*0.50}
            CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "模式偏差";  verticalAlignment: Text.AlignVCenter;font.pointSize: height*0.50}
        }
        Row{
            width: parent.width;height:parent.height*0.97/3;spacing:width*0.05
            AnalysisDiagram{
                width: parent.height;height:parent.height*1.25;
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/TotalPE.bmp";
                range:root.range;
                onClicked:root.clearDot();
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }
            AnalysisDiagram{
                width: parent.height;height:parent.height*1.25;
                anchors.verticalCenter: parent.verticalCenter
                source:"/previewImage/PatternPE.bmp";
                range:root.range;
                onClicked:root.clearDot();
                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
            }
            Item{
                height: parent.height;width:parent.width*0.9-2*parent.height;
                Column{
                    height: parent.height*0.5; anchors.bottom: parent.bottom; width:parent.width;spacing:height*0.08;
                    Repeater{
                        anchors.fill: parent;
                        model: [{image:"qrc:/grays/PE1.bmp",pe:"<5%"},{image:"qrc:/grays/PE2.bmp",pe:"<2%"},{image:"qrc:/grays/PE3.bmp",pe:"<1%"},{image:"qrc:/grays/PE4.bmp",pe:"<0.5%"}]
                        Row{
                            height: parent.height*0.14;width: parent.width;spacing: height*0.5
                            Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                            CusText{height: parent.height;text:modelData.pe; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: height*0.70}
                        }
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
