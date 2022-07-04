﻿import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Utils 1.0

Column {
    id:root
    anchors.fill:parent;
    property string lastPage: "";
    property var currentPatient: null;
    property var currentProgram: null;
    property var currentCheckResult: null;
    property var analysisResult: null;
    property int report;
    property int textHeight: height*0.05;

    signal refresh();
    signal changePage(var pageName,var params);

    onRefresh: {
        console.log("report type is "+report);
        var type=currentProgram.type;
        if(type!==2)
        {
            content.source="StaticAnalysis.qml";
        }
        else
        {
            content.source="MoveAnalysis.qml";
        }

        content.item.textHeight=textHeight;
        content.item.currentPatient=currentPatient;
        content.item.currentProgram=currentProgram;
        content.item.currentCheckResult=currentCheckResult;
        content.item.analysisResult=analysisResult;
        content.item.report=report;
        content.item.refresh();
    }


    Loader{
        id:content;
        width: parent.width;height: parent.height*14/15;
    }


//    onRefresh: {
//        sf.textHeight=textHeight;
//        sf.currentPatient=currentPatient;
//        sf.currentProgram=currentProgram;
//        sf.currentCheckResult=currentCheckResult;
//        sf.analysisResult=analysisResult;
//        sf.refresh();
//    }


//    SingleField{
//        id:sf;
//        width: parent.width;height: parent.height*14/15;
//    }

    Rectangle{id:bottomRibbon;width: parent.width;height: parent.height*1/15;color:CommonSettings.ribbonColor;
        Row{anchors.fill: parent;
            Item{height: parent.height;width:parent.width*0.20;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";onClicked:{root.changePage(lastPage,null);}}}
                }
            Item{height: parent.height;width:parent.width*0.52;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height;spacing: height*0.8;width: parent.width;anchors.horizontalCenter: parent.horizontalCenter
                        CusButton{text:"重测";onClicked:{root.changePage("check",null);}}
                        CusButton{text:"新建患者";onClicked:{;root.changePage("patientManagement","createNewPatient");}}
                    }
                }
            }

            Item{height: parent.height;width:parent.width*0.28;
                Item{id: item1;anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Row
                    {
                        height: parent.height; layoutDirection: Qt.RightToLeft;spacing: height*0.8;width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
//                        CusButton{text:"打印";onClicked: {console.log(currentPatient.name);console.log(currentProgram.params.commonParams.Range[1]);console.log(analysisResult.GHT)}}
                        CusButton{
                            text:"打印";onClicked:
                            {
//                                var reportWindow=IcUiQmlApi.appCtrl.objMgr.attachObj("Dialog", false,[0]);
//                                reportWindow.showWindow();
//                                IcUiQmlApi.appCtrl.objMgr.detachObj("Dialog",reportWindow);

//                                var reportWindow=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::Report", false,[0]);
//                                reportWindow.showWindow();
//                                IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::Report",reportWindow);
                                IcUiQmlApi.appCtrl.AnalysisProvider.showReport();
                            }
                        }
                        CusComboBox{
                            id:queryStrategy;height: parent.height;width: parent.height*3.5;
                            borderColor: backGroundBorderColor;font.family:"Microsoft YaHei";
                            imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                            model: ListModel {ListElement { text: "常规分析" } ListElement { text: "三合一图" } ListElement { text: "总览图" }ListElement { text: "筛选" }}

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