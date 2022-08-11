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
    property var analysisLobbyListVm: null;
    property var currentProgram: null;
    property var analysisVm: null;
    signal changePage(var pageName,var params);
    signal refresh()


    onRefresh: {
        if(analysisLobbyListVm!==null) IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::AnalysisLobbyListVm",analysisLobbyListVm);
        analysisLobbyListVm=IcUiQmlApi.appCtrl.objMgr.
        attachObj("Perimeter::AnalysisLobbyListVm", false,[currentPatient.id,((content.height-10)/4*0.9-4)]);}

    ListView{
        id:content;width: parent.width;height: parent.height*14/15;/*snapMode: ListView.SnapOneItem;*/spacing: -2;clip:true;model:analysisLobbyListVm;
        delegate: checkRowDelegate
        signal cancelSelected();
//        signal setIndex();
        Component{id:checkRowDelegate
            Column{width: content.width;height: (content.height+6)/4;
                Rectangle{width: parent.width;height: 2;color: "white"}
                Rectangle{width: parent.width;height:(content.height-10)/4;color: backGroundColor;
                    Row{id: row;anchors.fill: parent;
                        Column{height: parent.height*0.8;anchors.verticalCenter: parent.verticalCenter;width: parent.width*0.07;
                            Item {width:parent.width;height: parent.height*0.18;}
                            CusText{width:parent.width;height: parent.height*0.27;text: year ;horizontalAlignment: Text.AlignHCenter}
                            CusText{width:parent.width;height:parent.height*0.35;text:monthDay ;horizontalAlignment: Text.AlignHCenter}
                        }
                        ListView{
                            height: parent.height*0.9;anchors.verticalCenter: parent.verticalCenter;clip: true;orientation: ListView.Horizontal;
                            spacing: height*0.15;width:parent.width*0.93;model:simpleCheckResult;
                            delegate: checkImgDelegate
                            Component{id:checkImgDelegate
                                Item{property bool selected: false;height: Math.floor(parent.height);width: Math.floor((parent.height-4)*0.8)+4;
                                    Image {
                                        id:image;height:parent.height-4;anchors.verticalCenter: parent.verticalCenter;anchors.horizontalCenter: parent.horizontalCenter;fillMode: Image.PreserveAspectFit
                                        source:"file:///" + applicationDirPath +picName;smooth: false;cache: false;
                                    }
                                    MouseArea
                                    {
                                        anchors.fill: parent;z:1;
                                        onClicked:{
                                            content.cancelSelected();parent.selected=true;
                                            console.log(checkResultId);
                                            if(currentCheckResult!==null)
                                            {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::CheckResultVm", currentCheckResult);}
                                            currentCheckResult=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::CheckResultVm", false,[checkResultId]);
                                            if(currentProgram!==null)
                                            {
                                                if(currentProgram.type!==2)
                                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);
                                                else
                                                    IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::DynamicProgramVM", currentProgram);
                                            }
                                            if(currentCheckResult.type!==2)
                                                currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false,[currentCheckResult.program_id]);
                                            else
                                                currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::DynamicProgramVM", false,[currentCheckResult.program_id]);
                                        }
                                    }
                                    Rectangle{anchors.fill: parent;color: "steelblue";opacity: parent.selected?1:0;z:-1;}
                                    Component.onCompleted:
                                    {
                                        content.cancelSelected.connect(function(){selected=false;})
                                    }
                                }
                            }
                        }
                    }
                }
                Rectangle{width: parent.width;height: 2;color: "white"}
            }
        }
    }
    Rectangle{id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
        Row{anchors.fill: parent;
            Item{height: parent.height;width:parent.width*0.20;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";
                        onClicked:{
                            root.changePage("patientManagement",null);
                            if(currentCheckResult!==null)
                            {
                                IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::CheckResultVm", currentCheckResult);
                                currentCheckResult=null;
                            }
                        }}

                    }
                }

            Item{height: parent.height;width:parent.width*0.3;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height;spacing: height*0.8;anchors.horizontalCenter: parent.horizontalCenter;
//                        CusButton{text:"进展分析";onClicked:{changePage("progressAnalysis",null)}}
                        CusComboBoxButton{
                            height: parent.height;width: height*3.5;
                            property var listModel:["左眼分析","右眼分析"]
                            comboBox.model: listModel;popDirectionDown: false;complexType: false;
                            button.text: "进展分析";
                            button.onClicked:
                            {
                                changePage("progressAnalysis",0)
                            }
                            comboBox.onActivated:
                            {
                                changePage("progressAnalysis",index)
                            }
                        }
                        CusButton{text:"视岛图";onClicked:{console.log((content.height-10)/4*0.9-4);/*content.setIndex()*/}}
                        }
                    }
                }

            Item{height: parent.height;width:parent.width*0.20;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"删除"; enabled: currentCheckResult!==null;anchors.horizontalCenter: parent.horizontalCenter;
                        onClicked:analysisLobbyListVm.deleteCheckResult(currentCheckResult.id);
                    }
                }
            }

            Item{height: parent.height;width:parent.width*0.30;
                Flow{ layoutDirection: Qt.RightToLeft;anchors.fill: parent;anchors.margins:parent.height*0.15;spacing: height*0.8;
                    CusComboBoxButton{
                        id:queryStrategy;
                        height: parent.height;width: height*3.5;
                        enabled: currentCheckResult!==null;
                        property var listModel:ListModel {}
                        property var reportNames: ["常规分析","三合一图","总览图","筛选","标准动态","盲区","暗区","直线"]
                        comboBox.model: listModel;popDirectionDown: false;complexType: true;
                        button.text: "分析";
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
                            case 0:diagramWidth=root.height*14/15*0.92*0.97/3*1.25*0.8;break;
                            case 1:diagramWidth=root.height*14/15*0.92*0.47*0.8;break;
                            case 2:diagramWidth=root.height*14/15*0.92*0.40*0.8;break;
                            case 3:case 4:case 5:case 6:case7:diagramWidth=root.height*14/15*0.92*0.8;break;
                            }
                            var analysisResult=null;
                            if(analysisVm!=null)
                            {
                                if(analysisVm.type!==2){ IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticAnalysisVm",analysisVm);}
                                    else{ IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::DynamicAnalysisVm",analysisVm);}
                            }
                            if(currentProgram.type!==2)
                            {
                                analysisVm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticAnalysisVm", false,[currentCheckResult.id,diagramWidth,report]);
                                analysisResult=analysisVm.getResult();
                            }
                            else
                            {
                                analysisVm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::DynamicAnalysisVm", false,[currentCheckResult.id,diagramWidth,report]);
                            }
                            changePage("analysis",{pageFrom:"analysisLobby",report:report,analysisVm:analysisVm,program:currentProgram,checkResult:currentCheckResult,analysisResult:analysisResult});
                        }

                        Component.onCompleted: {
                            root.currentProgramChanged.connect(function()
                            {
                                listModel.clear();
                                var report=currentProgram.report;
                                report.forEach(function(item){
                                    listModel.append({name:reportNames[item],report:item});
                                })
                                comboBox.currentIndex=0;
                            })
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
