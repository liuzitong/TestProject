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
    property var progressAnalysisListVm: null;
    property int resultId;
    property int os_od;
    property int pageSize: 20
    signal changePage(var pageName,var params);
    signal refresh()

    onRefresh: {
        console.log("refresh");
        if(progressAnalysisListVm!==null) IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgressAnalysisListVm",progressAnalysisListVm);
        progressAnalysisListVm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgressAnalysisListVm", false,[currentPatient.id,os_od]);}

    Item{
        width: parent.width;height: parent.height*14/15
        Column{
            id:infoCol;
            anchors.fill:parent;spacing: -1;
            Row{
                id:header
                width: parent.width; height:(infoCol.height)/(pageSize+1);spacing: -1;z:1;
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
                id:progressAnalysisListView
                signal deSelectAll();
                property int seletedPatientLength:0;
                property var seletedPatient:[];
                width: parent.width;height:(infoCol.height)/(pageSize+1)*pageSize ; interactive: false; spacing: -1;clip:true;snapMode: ListView.SnapPosition;/*interactive: false;*/
                delegate: infoDelegate
                model:progressAnalysisListVm;
                currentIndex: progressAnalysisListVm.selectedIndex;
//                currentIndex: count-1;
//                model:[
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:true},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:true},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:false},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:false},
//                    {time:"sss",programName:"30-2",strategy:3,md:1.35,psd:2.0,fixationLossRate:0.3,falsePosRate:0.1,falseNegRate:0.5,baseline:false}]
                Component.onCompleted: {

                }
                Component{
                    id:infoDelegate
                    Item{
                        id:rootItem
//                        property bool isSelected: false;
                        property string rowBackGroundColor:progressAnalysisListView.currentIndex==index?"steelblue":(index==0||index==1)?"grey":"white";
                        property string rowForeGroundColor:(progressAnalysisListView.currentIndex==index||index==0||index==1)?"white":"black";

                        width: progressAnalysisListView.width;height: (infoCol.height)/(pageSize+1);
                        MouseArea{
                            anchors.fill: parent;
                            onClicked:
                            {
                                progressAnalysisListView.deSelectAll();
//                                parent.isSelected=true;
                                progressAnalysisListVm.selectedIndex=index;

                            }

                        }
                        Row{
                            id:infoRow;anchors.fill: parent;spacing: -1;
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:dateTime}}
                            Rectangle{width: parent.width*2/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:program}}
                            Rectangle{width: parent.width*2/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:strategy}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:md.toFixed(2)}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:psd.toFixed(2)}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:fixationLossRate.toFixed(2)}}
                            Rectangle{width: parent.width*1/10+1;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:falseNegativeRate.toFixed(2)}}
                            Rectangle{width: parent.width*1/10;height: parent.height;color:rootItem.rowBackGroundColor; border.color: backGroundBorderColor;CusText{color:rootItem.rowForeGroundColor;anchors.fill: parent;text:falsePositiveRate.toFixed(2)}}
                        }
                    }
                }
            }
        }
    }
    Rectangle{id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
        Item{ id: item2;anchors.fill: parent;
            Item{height: parent.height;width:parent.width*0.20;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";onClicked:root.changePage("analysisLobby",null);}
                    }
                }


            Item{height: parent.height; anchors.horizontalCenter: parent.horizontalCenter; width:parent.width*0.33;
                Item{ id: item1;anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height; layoutDirection: Qt.RightToLeft; anchors.right: parent.right; spacing: height*0.4;anchors.horizontalCenter: parent.horizontalCenter;
                        CusButton{text:"去除";enabled:progressAnalysisListVm==null?false:progressAnalysisListVm.rowCount()>=3;onClicked:
                            {
                                progressAnalysisListVm.removeCheckResult(progressAnalysisListView.currentIndex);
                                if(progressAnalysisListVm.rowCount()-1<progressAnalysisListView.currentIndex)
                                {
                                    progressAnalysisListView.currentIndex=progressAnalysisListVm.rowCount()-1;
                                }
                                progressAnalysisListVmChanged();
                            }
                        }
                        CusButton
                        {
                            text:"复位";onClicked:
                            {
                                progressAnalysisListVm.reset();progressAnalysisListVmChanged();
                            }
                        }
                        CusButton{text:"切换眼别";onClicked:{
                                progressAnalysisListVm.switchEye();
                                console.log(root.progressAnalysisListVm.selectedIndex);
//                                progressAnalysisListView.currentIndex=root.progressAnalysisListVm.selectedIndex;
                            }
                        }
                    }
                }
            }

            Item{height: parent.height; anchors.right: parent.right; width:parent.width*0.4;
                Item{ id: item3;anchors.fill: parent;anchors.margins:parent.height*0.15;
                    //                    Flow{height: parent.height;spacing: height*0.4;anchors.horizontalCenter: parent.horizontalCenter;
//                        CusButton{text:"进展基线";onClicked:{changePage("progressAnalysis",{data:progressAnalysisListVm,report:0})}}
//                        CusButton{text:"最后三次进展";onClicked:{changePage("progressAnalysis",{data:progressAnalysisListVm,report:1})}}
//                        CusButton{text:"单次进展分析";onClicked:{changePage("progressAnalysis",{data:progressAnalysisListVm,report:2})}}
//                    }
                    CusComboBoxButton{
                        id:queryStrategy;
                        property var baseLineAnalysisResult: null;  //对象
                        property var threeFollowUpsAnalysisResult;  //值
                        property var singleAnalysisResult;  //值
                        property var staticAnalysisResult: null;    //对象
                        property var staticAnalysisVm: null;
                        height: parent.height; anchors.right: parent.right; width: height*3.5;
                        property var listModel:ListModel {
                            ListElement{name:"进展基线";report:0}
                            ListElement{name:"最后三次进展";report:1}
                            ListElement{name:"单次进展分析";report:2}}
//                        property var listModel;
                        comboBox.model: listModel;popDirectionDown: false;complexType: true;
                        button.text: "进展分析";
                        button.onClicked:
                        {
                            var report=listModel.get(0).report;
                            analysis(report);
                        }
                        comboBox.onActivated:
                        {
                            var report=listModel.get(index).report;
                            analysis(report);
                        }
                        function analysis(report)
                        {
                            var diagramWidth;
                            switch (report)
                            {
                            case 0:
                                if(baseLineAnalysisResult!==null) analysisResult1.destroy();
                                diagramWidth=root.height*14/15*0.92*0.32;
                                baseLineAnalysisResult=progressAnalysisListVm.getProgressBaseLine(diagramWidth,false);
                                changePage("progressAnalysis",{progressAnalysisListVm:progressAnalysisListVm,report:report,result:baseLineAnalysisResult});
                                return;
                            case 1:
                                diagramWidth=root.height*14/15*0.92*0.96*0.32;
                                threeFollowUpsAnalysisResult=progressAnalysisListVm.getThreeFollowUps(progressAnalysisListVm.selectedIndex,diagramWidth,false);
                                changePage("progressAnalysis",{progressAnalysisListVm:progressAnalysisListVm,report:report,result:threeFollowUpsAnalysisResult});
                                return;
                            case 2:
                                diagramWidth=root.height*14/15*0.92*0.96/3;
                                singleAnalysisResult=progressAnalysisListVm.getSingleProgress(progressAnalysisListVm.selectedIndex,diagramWidth,false);
                                if(staticAnalysisVm!=null)
                                {
                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticAnalysisVm",staticAnalysisVm);
                                }
                                staticAnalysisVm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticAnalysisVm", false,[progressAnalysisListVm.selectedResultId,diagramWidth,0]);
                                changePage("progressAnalysis",{progressAnalysisListVm:progressAnalysisListVm,report:report,result:singleAnalysisResult,staticAnalysisVm:staticAnalysisVm});
                                return;
                            }
//                            changePage("progressAnalysis",{progressAnalysisListVm:progressAnalysisListVm,report:report,result:analysisResult});
//                            changePage("analysis",{pageFrom:"analysisLobby",report:report,analysisVm:analysisVm,program:currentProgram,checkResult:currentCheckResult,analysisResult:analysisResult});
                        }
                        Component.onCompleted:
                        {
                            progressAnalysisListView.currentIndexChanged.connect(function()
                            {
                                if(progressAnalysisListView.currentIndex>=2) enabled=true; else enabled=false;

                                console.log(progressAnalysisListView.currentIndex);
                            });

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
