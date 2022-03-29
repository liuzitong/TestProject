import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import qxpack.indcom.ui_qml_base 1.0
import QtQuick.Layouts 1.3
import perimeter.main.view.Controls 1.0
import perimeter.main.view.Utils 1.0


Item {id:root; width: 1366;height: 691; visible: true;anchors.fill:parent;
    property var currentProgram: null;
    property int type;
    signal changePage(var pageName);
    function rePaintCanvas(){dbDisplay.displayCanvas.requestPaint();}
    Rectangle{id:content;width: parent.width;height: parent.height*14/15
        anchors.top: parent.top
        Item{anchors.fill: parent;anchors.margins: 2;
            Row{anchors.fill: parent;spacing: 2;
                Column{width:parent.width*0.235;height: parent.height;spacing:2;
                    Column{id: column;width: parent.width; height: parent.height*0.6;
                        Rectangle{height: parent.height*0.1;width: parent.width;color:"#D2D2D3"
                            TabBar {id: bar;height: parent.height*0.8;anchors.bottom: parent.bottom;anchors.right: parent.right; anchors.rightMargin:0.03*parent.width;
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
                                    Item {id: homeTab;anchors.fill: parent;anchors.margins: height*0.03;
                                        ListView {
                                            id:listView;spacing: -1;anchors.fill: parent;
                                            model: modelData;delegate: delegateProg;clip:true;snapMode: ListView.SnapPosition;
                                        }
                                        Component{id:delegateProg;
                                            Rectangle{height: (homeTab.height-1)*1/10+1;width: listView.width;color:"white";border.color: "black";
                                                CusText{width: parent.width;text:model.name}
                                                MouseArea{ anchors.fill: parent;
                                                    onClicked:
                                                    {
                                                        console.log(model.name+"  "+model.program_id+" "+model.type);
                                                        if(programSelection.selectedProgram!=null){programSelection.selectedProgram.color="white";}
                                                        programSelection.selectedProgram=parent;
                                                        programSelection.selectedProgram.color=CusUtils.rgb(220,222,224);
//                                                        currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false);
//                                                        currentProgram.hello();
                                                        if (currentProgram!=null)
                                                        {
                                                            if(type!=2)
                                                            {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);}
                                                            else{IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::MoveProgramVM", currentProgram);}
                                                        }

                                                        type=model.type;
                                                        if(type!=2)
                                                        {currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false,[model.program_id]);}
                                                        else{currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::MoveProgramVM", false,[model.program_id]);}
                                                        console.log(currentProgram.name);
                                                        var params=currentProgram.params;
                                                        console.log(currentProgram.dots.length);
                                                        strategyStack.changeProgram();
                                                        dbDisplay.dotList=currentProgram.dots;
                                                        if(type!=2){dbDisplay.range=params.commonParams.Range[1];}
                                                        else{dbDisplay.range=params.Range[1]}
                                                        dbDisplay.displayCanvas.requestPaint();

                                                    }
                                                    Component.onDestruction: {
                                                        if (currentProgram!=null)
                                                        {
                                                            if(type!=2)
                                                            {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);}
                                                            else{IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::MoveProgramVM", currentProgram);}
                                                        }
                                                        if (currentProgram!=null)
                                                        {
                                                            IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);
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
                    Column{width: parent.width;height: parent.height*0.4-2
                        Rectangle{width: parent.width;height: parent.height*0.11;color:"#D2D2D3";
                            CusText{text:"策略"; horizontalAlignment: Text.AlignHCenter; anchors.fill: parent}
                        }
                        Rectangle{width: parent.width;height: parent.height*0.89;color:"#DCDEE0";
                            StackLayout {
                                id:strategyStack;anchors.fill: parent;
                                property var strategyNames;
                                function  changeProgram()
                                {
                                    switch(currentProgram.type)
                                    {
                                        case 0:strategyNames=[{name:qsTr("fullThreshold"),strategy:0},{name:qsTr("smart interactive"),strategy:1},{name:qsTr("fast interactive"),strategy:2}];break;
                                        case 1:strategyNames=[{name:qsTr("one stage"),strategy:3},{name:qsTr("two stages"),strategy:4},{name:qsTr("quantify defects"),strategy:5},{name:qsTr("single stimulation"),strategy:6}];break;
                                        case 2:strategyNames=[];break;
                                    }

                                }

                                Item{anchors.fill: parent;anchors.margins: 0.1*height;
                                    Column{anchors.fill: parent;spacing: height*0.10;
                                        Repeater {
                                            model:strategyStack.strategyNames;
                                            Row{
                                                width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                                CusCheckBox{
                                                    property int strategy:modelData.strategy;
                                                    height: parent.height;checked:currentProgram.strategies.indexOf(strategy)>-1;width: parent.height;
                                                    onClicked:
                                                    {
                                                        var list = currentProgram.strategies;
                                                        checked?list.push(strategy):list.pop(strategy);
                                                        currentProgram.strategies=list;
                                                        console.log(currentProgram.strategies.length);
                                                    }
                                                }
                                                CusText{text:modelData.name; horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.35}
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Item{width:parent.width*0.650-2;height: parent.height;
                    Rectangle{anchors.fill: parent;color:"#cbced0";
                        CusText{text:qsTr("点坐标"); anchors.top: parent.top; anchors.topMargin: 0.05*parent.height; anchors.left: parent.left; anchors.leftMargin: 0.05*parent.width;width: parent.width*0.06;height: parent.height*0.05;}
                        CusText{text:"(-90,71)"; anchors.top: parent.top; anchors.topMargin: 0.08*parent.height; anchors.left: parent.left; anchors.leftMargin: 0.05*parent.width;width: parent.width*0.06;height: parent.height*0.05;}
                        DbDisplay{id:dbDisplay;}
                    }
                }
                Item{ width:parent.width*0.115-2;height: parent.height;
                    Rectangle{anchors.fill: parent;color: "#DCDEE0";
                        Item{ anchors.fill: parent;anchors.margins: 0.15*width;
                            Column{anchors.fill: parent;spacing: width*0.25;
                                CusButton{text:"新建";height: parent.width*0.3;width: parent.width;
                                    onClicked: {}
                                }
                                CusButton{text:"取消";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"保存";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"删除";height: parent.width*0.3;width: parent.width;}
//                                CusButton{text:"清除";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"复制";height: parent.width*0.3;width: parent.width;}
                            }
                        }
                    }
                }
            }
        }
    }
    Rectangle{id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
        anchors.bottom: parent.bottom
        visible: true
        Row{anchors.fill: parent;
            Item{height: parent.height;width:parent.width*0.235;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";onClicked:root.changePage("patientManagement");}
                    }
                }
            Item{height: parent.height;width:parent.width*0.648;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height;spacing: height*0.8;anchors.horizontalCenter: parent.horizontalCenter
                        CusButton{text:"参数设置";}
                        CusButton{text:"圆形选点";}
                        CusButton{text:"矩形选点";}
                        }
                    }
                }
            Item{height: parent.height;width:parent.width*0.117;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"解锁"; anchors.horizontalCenter: parent.horizontalCenter}
                }
            }
        }
    }
}
