﻿import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0     // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0  // [HINT] ModalPopupDialog is in it
import perimeter.main.view.Utils 1.0

ModalPopupDialog /*Rectangle*/{   // this is the wrapped Popup element in ui_qml_contro
    id:idPopup
    property alias color: idContent.color;
    reqEnterEventLoop:false;
//    width:1366; height: 720;
//    property alias contentWidth : idContent.implicitWidth;
//    property alias contentHeight: idContent.implicitHeight;
    property var currentProgram:null;
    property int program_id;
    property int program_type;
    signal ok();
    signal cancel();

   contentItem:
   Rectangle{
        id: idContent; color: "#60606060";implicitWidth: idPopup.width; implicitHeight: idPopup.height;
        Rectangle
        {
        // [HINT] Popup element need implicitWidth & implicitHeight to calc. the right position
            id: menu; width:idPopup.width*0.35; height: idPopup.height*0.9;color: "#dcdee0";radius: 5;/*width:480; height:480;*/
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            border.color: "#7C7C7C";
            Column{
                id: column1
                anchors.fill: parent;
                Canvas{
                    id:header;height: idPopup.height*0.05;width: parent.width;property alias radius: menu.radius;
                    onPaint: {var ctx = getContext("2d");ctx.beginPath(); // Start the path
                        ctx.moveTo(1,height);
                        ctx.lineTo(1, radius-1); // Set the path origin
                        ctx.arc(radius,radius,radius-1,Math.PI,Math.PI*1.5);
                        ctx.lineTo(width-radius, 1);// Set the path destination
                        ctx.arc(width-radius,radius,radius-1,Math.PI*1.5,0);
                        ctx.lineTo(width-1, height);
                        ctx.closePath();
                        context.fillStyle = "#D2D2D3"
                        ctx.fill();
                    }
                    CusText{text:"选择程序";}
                }
                Column{id: column;width: parent.width-2; height: parent.height*0.8;
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle{height: parent.height*0.1;width: parent.width;color:"#D2D2D3"
                        TabBar {id: bar;height: parent.height*0.8;anchors.bottom: parent.bottom;anchors.right: parent.right; anchors.rightMargin:0.3*parent.width;
                            anchors.left: parent.left; anchors.leftMargin: 0.03*parent.width;spacing: 0;currentIndex: 0;
                            Repeater { model:["阈值","筛选","特殊","动态","自定义"]
                                TabButton {width: bar.width*0.20;height: parent.height;
                                    Rectangle{anchors.fill: parent;color:parent.checked? "#E3E5E8":"#D2D2D3";
                                        Rectangle{width: parent.width;height: 3;color: "#0064B6";visible: parent.parent.checked? true:false; }
                                        CusText{text:modelData; anchors.fill: parent;color:parent.parent.checked?"#0064B6":"black";font.pointSize: height*0.3}
                                    }
                                }
                            }
                        }
                    }
                    Rectangle{id:programSelection;width: parent.width;height: parent.height*0.9;color: "#DCDEE0";
                        property var selectedProgram: null;
                        StackLayout {
                            anchors.fill: parent;currentIndex: bar.currentIndex;
                            Repeater {
                                id:programLists;
                                function refreshData()
                                {
                                    programListModelVmThreshold.refreshData();
                                    programListModelVmScreening.refreshData();
                                    programListModelVmSpecial.refreshData();
                                    programListModelVmMove.refreshData();
                                    programListModelVmCustom.refreshData();
                                }

                                property var programListModelVmThreshold: null;
                                property var programListModelVmScreening: null;
                                property var programListModelVmSpecial: null;
                                property var programListModelVmMove: null;
                                property var programListModelVmCustom: null;
                                Component.onCompleted: {
                                    programListModelVmThreshold=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[0]);
                                    programListModelVmScreening=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[1]);
                                    programListModelVmSpecial=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[2])
                                    programListModelVmMove=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[3]);
                                    programListModelVmCustom=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[4]);
                                }
                                Component.onDestruction: {
                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmThreshold);
                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmScreening);
                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmSpecial);
                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmMove);
                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmCustom);
                                }
                                model:[programListModelVmThreshold,programListModelVmScreening,programListModelVmSpecial,programListModelVmMove,programListModelVmCustom]
                                Item {id: homeTab;anchors.fill: parent;anchors.margins: height*0.07;
                                    ListView {
                                        id:listView;spacing: -1;anchors.fill: parent;
                                        model: modelData;delegate: delegateProg;clip:true;snapMode: ListView.SnapPosition;
                                    }
                                    Component{id:delegateProg;
                                        Rectangle{height: (homeTab.height-1)*1/12+1;width: listView.width;color:"white";border.color: backGroundBorderColor;
                                            CusText{width: parent.width;text:model.name}
                                            MouseArea{ anchors.fill: parent;
                                                onClicked:
                                                {
                                                    idPopup.program_id=model.program_id;
                                                    idPopup.program_type=model.type;
                                                    if(programSelection.selectedProgram!=null){programSelection.selectedProgram.color="white";}
                                                    programSelection.selectedProgram=parent;
                                                    programSelection.selectedProgram.color=CusUtils.rgb(220,222,224);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Item {
                    width: parent.width;
                    height: parent.height*0.15
                    Flow{
                        anchors.top: parent.top
                        anchors.topMargin: parent.height*0.34;
                        anchors.right: parent.right
                        anchors.margins: parent.height*0.40;
                        spacing: height;
                        layoutDirection: Qt.RightToLeft
                        anchors.verticalCenter: parent.verticalCenter
                        CusButton{text:"取消";onClicked: idPopup.close();}
                        CusButton
                        {
                            text:"确定";onClicked:
                            {
                                if (currentProgram!==null)
                                {
                                    if(currentProgram.type!==2)
                                    {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);}
                                    else{IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::MoveProgramVM", currentProgram);}
                                }
                                if(program_type!==2)
                                {
                                    currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false,[program_id]);
                                }
                                else{
                                    currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::MoveProgramVM", false,[program_id]);
                                }
                                idPopup.close();
                                ok();
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