import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Utils 1.0

Item
{
    id:root;
    signal refresh();
    property var currentPatient: null;
    property var currentProgram: null;
    property var currentCheckResult: null;
    property var report:null;                           //4:常规,5:三合一,6:总览,7:筛选
    property int range: currentProgram.params.Range[1];
    property string rx:"";
    property int fontPointSize: CommonSettings.fontPointSize;
    property int textHeight:CommonSettings.textHeight;

    onRefresh:{
        if(currentPatient.os_od===0)  //左
            rx=currentPatient.rx.rx1_l.toFixed(2)+"DS "+currentPatient.rx.rx2_l.toFixed(2)+"DC "+currentPatient.rx.rx3_l.toFixed(2)+"X";
        else
            rx=currentPatient.rx.rx1_r.toFixed(2)+"DS "+currentPatient.rx.rx2_r.toFixed(2)+"DC "+currentPatient.rx.rx3_r.toFixed(2)+"X";
    }

    Row{
        anchors.fill: parent;
        Rectangle{width:parent.width;height: parent.height;color:"white";
            Row{ id: row;anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;spacing: width*0.10;
                Column{ id: column;width: parent.width*0.18;height: parent.height;spacing:parent.height*0.01
                    CusText{text:currentProgram.name; font.bold: true; horizontalAlignment: Text.AlignLeft;height:parent.height*0.10;font.pointSize: fontPointSize*2;}
                    Column{width:parent.width;height: parent.height*0.15;spacing: textHeight*0.5;
                        Repeater{
                            property var params: currentCheckResult.params;
                            property int timeSpan:currentCheckResult.resultData.testTimespan;
                            property var fixationMonitor: ["不报警","只报警","报警并暂停"];

                            model: [
                                {name:"眼动报警模式",param:fixationMonitor[params.fixationMonitor]},
                                {name:"测试用时",param:Math.floor(timeSpan/60)+":"+timeSpan%60}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
    //                        CusText{text:"固视监测";  horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                    }
                    Column{width:parent.width;height: parent.height*0.15;spacing: textHeight*0.5;
                        Repeater{
                            property var params: currentCheckResult.params;
                            property var cursorSize: ["I","II","III","IV","V"];
                            property var cursorColor: ["白色","红色","蓝色"];
                            property var backGroundColor: ["31.5 ASB","315 ASB"];
                            property var strategy: ["标准动态","盲区","暗区","直线"]
                            model: [
                                {name:"刺激光标",param:cursorSize[params.cursorSize]+","+cursorColor[params.cursorColor]},
                                {name:"背景光",param:backGroundColor[params.backGroundColor]},
                                {name:"策略",param:strategy[params.strategy]}]
                            CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
                    }
                    Column{width:parent.width;height: parent.height*0.15;spacing: textHeight*0.5;
                        Repeater{
                            model: [
                                {name:"瞳孔直径",param:currentCheckResult.resultData.pupilDiameter.toFixed(2)+"mm"},
                                {name:"视力",param:!currentCheckResult.OS_OD?currentPatient.rx.visual_l.toFixed(2):currentPatient.rx.visual_r.toFixed(2)},
                                {name:"屈光度",param:"Rx:"+rx}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
                    }

                    Column{width:parent.width;height: parent.height*0.25;spacing: textHeight*0.5;
                        CusText{text:"诊断:"; horizontalAlignment: Text.AlignLeft;width:parent.width;height:textHeight;}
                        Rectangle{ id: rectangle;width:parent.width;height: parent.height*0.73;radius: 5;border.color: "black";smooth: false;
                            TextInput
                            {
                                id:diagnosis;anchors.fill: parent;anchors.margins: 3;
                                width:parent.width*1.0;height: parent.height*0.70;
                                text:currentCheckResult==null?"":currentCheckResult.diagnosis;
                                selectionColor: "blue";selectByMouse: true;
                                font.pointSize: textHeight;font.family: "Consolas";
                                wrapMode: Text.WrapAnywhere;renderType: Text.NativeRendering;
                            }

                            CusButton{ id: cusButton;height: parent.height*0.25;width: height*2;
                                text:"保存";anchors.right: parent.right;anchors.bottom: parent.bottom;
                                anchors.rightMargin: 5;anchors.bottomMargin: 5;
                                onClicked:
                                {
                                    currentCheckResult.diagnosis=diagnosis.text;
                                    currentCheckResult.update();
                                }
                            }
                        }
                    }

                    Item{ id: item2;width:parent.width;height: parent.height*0.12; anchors.left: parent.left;
                        FixationDeviation{ anchors.bottom: parent.bottom; dots:if(currentCheckResult!==null) currentCheckResult.resultData.fixationDeviation; }
                    }
                }

                Item
                {
                    id:content;
                    width: parent.width*0.72;height:parent.height
                    AnalysisDiagram{
                        width: height;height:parent.height;
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source:"/previewImage/Dynamic.bmp";
                        range:root.range;
                        Component.onCompleted: {root.refresh.connect(refresh);}
                        visible: report==0;
                    }

                    Column
                    {
                        visible: report==1;
                        anchors.fill: parent;
                        Row{
                            width: scrollView.width-20;height: parent.height/21;
                            Repeater{
                                model:["等视标识线","离心度起点","子午线起点","离心度停止","子午线停止","刺激亮度反应"]
                                CusText{width:parent.width/6;text:modelData;font.pointSize: fontPointSize;}
                            }
                        }
                        ScrollView
                        {
                            width: parent.width;height: parent.height*20/21;
                            horizontalScrollBarPolicy: 1
                            verticalScrollBarPolicy: 0
                            clip: true
                            id:scrollView;
                            Column
                            {
                                width: scrollView.width-20;
                                height: (currentCheckResult.resultData.checkData.length)/20*scrollView.height;
                                Repeater
                                {
                                    model:currentCheckResult.resultData.checkData
                                    Row{
                                        width:parent.width;height:scrollView.height/20;
                                        Repeater
                                        {
                                            model:[modelData.name,modelData.start.x,modelData.start.y,modelData.end.x,modelData.end.y,modelData.isSeen?"Seen":"UnSeen"]
                                            CusText{width:parent.width/6;height: parent.height;font.pointSize:fontPointSize;text:modelData;}
                                        }
                                    }
                                }
                            }
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
