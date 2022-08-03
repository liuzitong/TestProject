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
    property var content;
    property bool locked: true;
//    onCurrentProgramChanged: {
//        if(currentProgram.type!==2){staticParamsSetting.currentProgram=currentProgram;}else{dynamicParamsSetting.currentProgram=currentProgram;}}


    Component.onDestruction: {
        if (currentProgram!=null)
        {
            if(currentProgram.type!==2)
            {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);}
            else{IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::DynamicProgramVM", currentProgram);}
        }

    }
//    property int type;
//    property var newProgram;
    signal changePage(var pageName,var params);
//    function rePaintCanvas(){display.requestPaint();}
    Rectangle{id:content;width: parent.width;height: parent.height*14/15
        NewProgram{
            id:newProgram;
            anchors.fill: parent;
//            onOk: {
//                currentProgram.type=type;currentProgram.strategy=strategy;
//                if(currentProgram.type!==2){currentProgram.params.commonParams.Range[1]=range;}
//                else{currentProgram.params.Range[1]=range;}
//            }
        }

        SaveToAnotherProgram{
            id:saveToAnotherProgram;
            anchors.fill: parent;

        }

        DynamicParamsSetting
        {
            id:dynamicParamsSetting;
            anchors.fill: parent;

        }

        StaticParamsSetting
        {
            id:staticParamsSetting;
            isCustomProg:true;
            anchors.fill: parent;
        }

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
                                    id:programLists;
                                    function refreshData()
                                    {
                                        programListModelVmThreshold.refreshData();
                                        programListModelVmScreening.refreshData();
                                        programListModelVmSpecial.refreshData();
                                        programListModelVmDynamic.refreshData();
                                        programListModelVmCustom.refreshData();
                                    }

                                    property var programListModelVmThreshold: null;
                                    property var programListModelVmScreening: null;
                                    property var programListModelVmSpecial: null;
                                    property var programListModelVmDynamic: null;
                                    property var programListModelVmCustom: null;
                                    Component.onCompleted: {
                                        programListModelVmThreshold=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[0]);
                                        programListModelVmScreening=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[1]);
                                        programListModelVmSpecial=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[2])
                                        programListModelVmDynamic=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[3]);
                                        programListModelVmCustom=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::ProgramListModelVm", false,[4]);
                                    }
                                    Component.onDestruction: {
                                        IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmThreshold);
                                        IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmScreening);
                                        IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmSpecial);
                                        IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmDynamic);
                                        IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::ProgramListModelVm",programListModelVmCustom);
                                    }
                                    model:[programListModelVmThreshold,programListModelVmScreening,programListModelVmSpecial,programListModelVmDynamic,programListModelVmCustom]
                                    Item {id: homeTab;anchors.fill: parent;anchors.margins: height*0.03;
                                        ListView {
                                            id:listView;spacing: -1;anchors.fill: parent;
                                            model: modelData;delegate: delegateProg;clip:true;snapMode: ListView.SnapPosition;
                                        }
                                        Component{id:delegateProg;
                                            Rectangle{height: (homeTab.height-1)*1/10+1;width: listView.width;color:"white";border.color: backGroundBorderColor;
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
                                                            if(currentProgram.type!==2)
                                                            {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);}
                                                            else{IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::DynamicProgramVM", currentProgram);}
                                                        }

                                                        var type=model.type;
                                                        console.log(type);
                                                        if(type!==2)
                                                        {
                                                            currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false,[model.program_id]);
                                                            staticParamsSetting.currentProgram=currentProgram;
                                                        }
                                                        else{
                                                            currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::DynamicProgramVM", false,[model.program_id]);
                                                            dynamicParamsSetting.currentProgram=currentProgram;
                                                        }
                                                        var params=currentProgram.params;
                                                        strategyStack.currentProgramChanged();
                                                        display.currentProgramChanged();
                                                        paramsSetting.enabled=true;
//                                                        staticParamsSetting.currentProgramChanged();
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
                                function currentProgramChanged()
                                {
                                    strategyNames=[];
//                                    console.log("currentProgram.type"+currentProgram.type);
                                    switch(currentProgram.type)
                                    {
                                        case 0:strategyNames=[{name:qsTr("fullThreshold"),strategy:0},{name:qsTr("smart interactive"),strategy:1},{name:qsTr("fast interactive"),strategy:2}];break;
                                        case 1:strategyNames=[{name:qsTr("one stage"),strategy:3},{name:qsTr("two stages"),strategy:4},{name:qsTr("quantify defects"),strategy:5},{name:qsTr("single stimulation"),strategy:6}];break;
//                                        case 2:strategyNames=[];break;
                                        default:break;
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
                                                    height: parent.height;checked:{if(currentProgram.type!==2){return currentProgram.strategies.indexOf(strategy)>-1;}else return false ;} width: parent.height;
                                                    enabled: if(currentProgram.type!==2){return !(currentProgram.params.commonParams.strategy===strategy);} else return false;
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
                        ProgramCustomizeDisplay{
                            id:display;
                            locked: root.locked;
                            function currentProgramChanged()
                            {

                                if(currentProgram.type!==2){display.range=currentProgram.params.commonParams.Range[1];}
                                else{display.range=currentProgram.params.Range[1]}
                                console.log(display.range);
//                                console.log(display.range);
                                display.type=currentProgram.type;
                                display.category=currentProgram.category;
//                                console.log("haha");
                                dotList=currentProgram.dots;
                                displayCanvas.requestPaint();

                            }
                            onDotListChanged:{if(currentProgram!==null) {currentProgram.dots=dotList;currentProgram.type===2?dynamicParamsSetting.currentProgramChanged():staticParamsSetting.currentProgramChanged();}}
                        }
                    }
                }
                Item{ width:parent.width*0.115-2;height: parent.height;
                    Rectangle{anchors.fill: parent;color: "#DCDEE0";
                        Item{ anchors.fill: parent;anchors.margins: 0.15*width;
                            Column{anchors.fill: parent;spacing: width*0.25;
                                CusButton{text:"新建";height: parent.width*0.3;width: parent.width;
                                    onClicked: {newProgram.open();}
                                    Component.onCompleted: {
                                        newProgram.ok.connect(createProgram);
                                    }
                                    function createProgram(){
                                        if (currentProgram!=null)
                                        {
                                            if(currentProgram.type!==2)
                                            {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);}
                                            else{IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::DynamicProgramVM", currentProgram);}
                                        }

                                        var type=newProgram.type;
                                        console.log("type:"+type,"strat:"+newProgram.strategy);
                                        if(type!==2)
                                        {
                                            currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false);
                                            staticParamsSetting.currentProgram=currentProgram;
                                        }
                                        else{
                                            currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::DynamicProgramVM", false);
                                            dynamicParamsSetting.currentProgram=currentProgram;
                                        }

                                        currentProgram.type=type;
                                        if(currentProgram.type!==2)
                                        {
                                            currentProgram.params.commonParams.strategy=newProgram.strategy;
                                            currentProgram.params.commonParams.Range[1]=newProgram.range;
                                            if(currentProgram.type===0) currentProgram.strategies=[0,1,2]; else currentProgram.strategies=[3,4,5,6];

                                        }
                                        else{
                                            currentProgram.params.strategy=newProgram.strategy;
                                            currentProgram.params.Range[1]=newProgram.range;
                                        }
                                        strategyStack.currentProgramChanged();
                                        display.currentProgramChanged();
                                        console.log(newProgram.programName);
                                        currentProgram.name=newProgram.programName;
                                        currentProgram.category=4;
                                        currentProgram.insertProgram();
                                        programLists.refreshData();
                                        paramsSetting.enabled=true;
                                        bar.currentIndex=4;

                                    }
                                }
                                CusButton{text:"取消";height: parent.width*0.3;width: parent.width;}
                                CusButton{
                                    text:"保存";height: parent.width*0.3;width: parent.width;enabled: currentProgram.category===4||!locked;
                                    onClicked: {/*currentProgram.dots.forEach(function(item){console.log("x:"+item.x+" y:"+item.y);});*/currentProgram.updateProgram();}
                                }

                                CusButton{text:"删除";height: parent.width*0.3;width: parent.width;enabled:currentProgram.category===4||!locked; onClicked:{ currentProgram.deleteProgram();paramsSetting.enabled=false;programLists.refreshData();}}
                                CusButton{
                                    text:"复制";height: parent.width*0.3;width: parent.width;
                                    onClicked: saveToAnotherProgram.open();
                                    Component.onCompleted: {
                                        saveToAnotherProgram.ok.connect(saveToAnother);
                                    }
                                    function saveToAnother()
                                    {
                                        currentProgram.category=4;
                                        currentProgram.name=saveToAnotherProgram.name;
                                        console.log(currentProgram.name);
                                        currentProgram.insertProgram();
                                        programLists.refreshData();
                                        bar.currentIndex=4;
                                    }
                                }
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
        Item{ id: item1;anchors.fill: parent;
            Item{height: parent.height; anchors.left: parent.left; anchors.leftMargin: 0;width:parent.width*0.235;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";onClicked:root.changePage("patientManagement",null);}
                    }
                }
            Item{height: parent.height; anchors.horizontalCenter: parent.horizontalCenter;width:parent.width*0.5;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height;spacing: height*0.8;anchors.horizontalCenter: parent.horizontalCenter
                        CusButton{id:paramsSetting;text:"参数设置";enabled:false;onClicked:if(currentProgram.type!==2){ staticParamsSetting.open()} else  { dynamicParamsSetting.open();}}
                        CusButton{text:"圆形选点";}
                        CusButton{text:"矩形选点";}
                        }
                    }
                }
            Item{height: parent.height; anchors.right: parent.right; width:parent.width*0.117;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{
                        id:unlock;text:"解锁"; anchors.horizontalCenter: parent.horizontalCenter;
                        onClicked: {unlock.visible=false;unlockPwd.visible=true;}
                    }
                    CusText{
                        anchors.fill: parent;
                        text:"已解锁";color: "white";visible: !locked}
                }
            }
            Item{height: parent.height; anchors.right: parent.right; width:parent.width*0.25;visible:false;
                id:unlockPwd
                Row{
                    layoutDirection: Qt.RightToLeft;anchors.fill: parent;anchors.margins:parent.height*0.15;spacing:0.5*height
                    CusButton{text:"确定";onClicked:{if(pwd.text===IcUiQmlApi.appCtrl.settings.programUnlockPwd) {locked=false;unlockPwd.visible=false;}else{pwdText.text="输入密码错误"}}}
                    LineEdit{id:pwd;width: parent.height*3;textInput.echoMode: TextInput.Password;}
                    CusText{id:pwdText;text:"输入解锁密码:";color: "white";}
                }
            }

        }
    }
}
