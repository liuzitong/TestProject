import QtQuick 2.6
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
    property var currentPatient: null;
    property string backGroundColor: CommonSettings.backGroundColor;
    property var currentCheckResult:null;
    property var progressAnalysisListVm: null;
    property var currentProgram: null;
    property int pageSize: 20;
    property int os_od;
    signal changePage(var pageName,var params);
    signal refresh()


    onRefresh: {
        console.log("refresh");
        if(progressAnalysisListVm!==null) IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgressAnalysisListVm",progressAnalysisListVm);
        progressAnalysisListVm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgressAnalysisListVm", false,[currentPatient.id,os_od]);}

    Item{
        width: parent.width;height: parent.height*14/15
        Column{
            id:patientInfoCol;
            anchors.fill:parent;spacing: -1;
            Row{
                id:header
                width: parent.width; height:(patientInfoCol.height)/(pageSize+1);spacing: -1;z:1;
                property bool isAllSelected: false;
                Rectangle{width: parent.width*1/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"日期"}}
                Rectangle{width: parent.width*2/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"选择程序"}}
                Rectangle{width: parent.width*2/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"策略"}}
                Rectangle{width: parent.width*1/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"平均缺损"}}
                Rectangle{width: parent.width*1/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"模式标准差"}}
                Rectangle{width: parent.width*1/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"固视丢失率"}}
                Rectangle{width: parent.width*1/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"假阳性率"}}
                Rectangle{width: parent.width*1/10;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"假阴性率"}}
            }
            ListView{
                id:patientInfoListView
                signal deSelectAll();
                property int seletedPatientLength:0;
                property var seletedPatient:[];
                width: parent.width;height:(patientInfoCol.height)/(pageSize+1)*pageSize ; interactive: false; spacing: -1;clip:true;snapMode: ListView.SnapPosition;/*interactive: false;*/
                delegate: patientInfoDelegate
//                model:patientListModelVm;

                model:root.progressAnalysisListVm;
//                model:[
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:true},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:true},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:false},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:false},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:false}]
                Component.onCompleted: {
//                    progressAnalysisVm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::GPACheckResultListVm", [currentCheckResult.]);

                }
//                Component.onDestruction: {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::PatientListModelVm",progressAnalysisVm);}
                Component{
                    id:patientInfoDelegate
                    Item{
                        id:rootItem
                        property bool isSelected: false;
                        property string rowBackGroundColor:isSelected?"steelblue":baseLine?"grey":"white";
                        property string rowForeGroundColor:(isSelected||baseLine)?"white":"black";
                        width: patientInfoListView.width;height: (patientInfoCol.height)/(pageSize+1);
                        MouseArea{
                            anchors.fill: parent;
                            onClicked:
                            {
                                patientInfoListView.deSelectAll();
                                parent.isSelected=true;
                            }

                        }
                        Row{
                            id:patientInfoRow;anchors.fill: parent;spacing: -1;



//                            Repeater
//                            {
//                                property bool isSelected: true;
//                                anchors.fill: parent;
//                                model:[
//                                    {width:parent.width*1/10+1,text:modelData.time},
//                                    {width:parent.width*2/10+1,text:modelData.programName},
//                                    {width:parent.width*2/10+1,text:modelData.strategy},
//                                    {width:parent.width*1/10+1,text:modelData.md},
//                                    {width:parent.width*1/10+1,text:modelData.psd},
//                                    {width:parent.width*1/10+1,text:modelData.fixationLossRate},
//                                    {width:parent.width*1/10+1,text:modelData.falsePosRate},
//                                    {width:parent.width*1/10+1,text:modelData.falseNegRate}
//                                ]
//                                Rectangle{width: modelData.width;height: parent.height;color:parent.isSelected?"blue":"white"; border.color: backGroundBorderColor;CusText{anchors.fill: parent;text:modelData.text}}
//                            }

//                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.time}}
//                            Rectangle{width: parent.width*2/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.program}}
//                            Rectangle{width: parent.width*2/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.strategy}}
//                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.md}}
//                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.psd}}
//                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.fixationLossRate}}
//                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.falseNegativeRate}}
//                            Rectangle{width: parent.width*1/10;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:modelData.falsePositiveRate}}

                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:dateTime}}
                            Rectangle{width: parent.width*2/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:program}}
                            Rectangle{width: parent.width*2/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:strategy}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:md.toFixed(2)}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:psd.toFixed(2)}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:fixationLossRate.toFixed(2)}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:falseNegativeRate.toFixed(2)}}
                            Rectangle{width: parent.width*1/10;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:falsePositiveRate.toFixed(2)}}

                        }
                        Component.onCompleted: {
                            patientInfoListView.deSelectAll.connect(function(){isSelected=false;});
                        }
                    }
                }
            }
        }
    }
    Rectangle{id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
        Item{anchors.fill: parent;
            Item{height: parent.height;width:parent.width*0.20;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";
                        onClicked:{
                            root.changePage("analysisLobby",null);
                        }}

                    }
                }

            Item{height: parent.height; anchors.horizontalCenter: parent.horizontalCenter;width:parent.width*0.4;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height;spacing: height*0.4;anchors.horizontalCenter: parent.horizontalCenter;
                        CusButton{text:"进展基线";onClicked:{console.log("gogog");/*content.positionViewAtBeginning();*/}}
                        CusButton{text:"最后三次进展";onClicked:{console.log("gogog");/*content.positionViewAtBeginning();*/}}
                        CusButton{text:"单次进展分析";onClicked:{console.log((content.height-10)/4*0.9-4);/*content.setIndex()*/}}
                    }
                }
            }

            Item{height: parent.height; anchors.right: parent.right;width:parent.width*0.33;
                Item{ id: item1;anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height; layoutDirection: Qt.RightToLeft; anchors.right: parent.right; spacing: height*0.4;anchors.horizontalCenter: parent.horizontalCenter;
                        CusButton{text:"去除";onClicked:{console.log("gogog");/*content.positionViewAtBeginning();*/}}
                        CusButton{text:"复位";onClicked:{console.log("gogog");/*content.positionViewAtBeginning();*/}}
                        CusButton{text:"切换眼别";onClicked:progressAnalysisListVm.switchEye();}
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
