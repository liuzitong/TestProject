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
    signal changePage(var pageName,var params);
    signal refresh()

//    onRefresh: {content.model=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::AnalysisLobbyListVm", false,currentPatient.id,queryStrategy.currentIndex,image.height);}

//    Component.onCompleted: {}

    ListView{
        property var listModel: ListModel{ListElement{year:"1998";dayMonth:"06/28";} ListElement{year:"2004";dayMonth:"06/28";}ListElement{year:"2004";dayMonth:"06/28";}ListElement{year:"2004";dayMonth:"06/28";}}
        id:content;
        width: parent.width;
        height: parent.height*14/15
        snapMode: ListView.SnapOneItem
        spacing: -2;
        clip:true;
//        model:checkDateListModel;
        model:listModel;
//        model:ListModel{ListElement{year:"1998";dayMonth:"06/28";}}
        delegate: checkRowDelegate
        signal cancelSelected();
        signal setIndex();
        Component.onCompleted:
        {

        }

        Component{
            id:checkRowDelegate

            Column{
                width: content.width;
                height: (content.height+6)/4;
                Rectangle{width: parent.width;height: 2;color: "white"}
                Rectangle{
                    width: parent.width;height:(content.height-10)/4;
                    color: backGroundColor;
                    Row{
                        id: row
                        anchors.fill: parent;
                        Column{
                            height: parent.height*0.8;
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width*0.07
                            Item {width:parent.width;height: parent.height*0.18;}
                            CusText{width:parent.width;height: parent.height*0.27;text: year ;horizontalAlignment: Text.AlignHCenter}
                            CusText{width:parent.width;height:parent.height*0.32;text:monthDay ;horizontalAlignment: Text.AlignHCenter}
                        }

                        ListView{
                            height: parent.height*0.9
                            highlightRangeMode: ListView.ApplyRange
                            //                            snapMode: ListView.SnapOneItem
                            anchors.verticalCenter: parent.verticalCenter
                            clip: true
                            orientation: ListView.Horizontal
                            spacing: height*0.15
                            width:parent.width*0.93;
                            model:["dBDiagram.bmp","dBDiagram.bmp","dBDiagram.bmp","dBDiagram.bmp"];
                            delegate: checkImgDelegate
//                            Component.onCompleted: positionViewAtIndex(0,ListView.Beginning);
                            Component.onCompleted: content.setIndex.connect(function(){console.log("setIndex");positionViewAtBeginning();})


                            Component
                            {
                                id:checkImgDelegate
                                Item{
                                    property bool selected: false
                                    height: parent.height;
                                    width: parent.height;

                                    Image {
                                        id:image
                                        height:parent.height-4;
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        fillMode: Image.PreserveAspectFit
    //                                    width: sourceSize.width;
    //                                    source: "./analysisLobbyImage/"+modelData;
//                                        source:"file:///" + applicationDirPath + "/analysisLobbyImage/"+picName;
                                        source:"file:///" + applicationDirPath + "/analysisLobbyImage/"+modelData;
                                        smooth: false;
                                        cache: false;        //to refresh image
    //                                    Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + source;})}

                                    }
                                    MouseArea
                                    {
                                        anchors.fill: parent;
                                        z:1;
                                        onClicked:{content.cancelSelected();parent.selected=true;}

                                    }

                                    Rectangle
                                    {
                                        anchors.fill: parent;
                                        color: "blue";
                                        opacity: parent.selected?1:0;
                                        z:-1;

                                    }
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
    Rectangle
    {
        id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
        Row{
            anchors.fill: parent;
            Item{
                height: parent.height;width:parent.width*0.20;
                Item{
                    anchors.fill: parent
                    anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";onClicked:root.changePage("patientManagement",null);}
                    }
                }

            Item{
                height: parent.height;width:parent.width*0.4;
                Item{
                    anchors.fill: parent
                    anchors.margins:parent.height*0.15;
                    Flow{
                        height: parent.height;spacing: height*0.8
                        anchors.horizontalCenter: parent.horizontalCenter
                        CusComboBox{
                            id:queryStrategy;height: parent.height;width: parent.height*3.5;
                            borderColor: backGroundBorderColor;font.family:"Microsoft YaHei";
                            imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                            model: ["常规分析","三合一图","总览图","筛选","标准动态","盲区","暗区","直线" ]
                        }
                        CusButton{text:"进展分析";onClicked:{console.log("gogog");/*content.positionViewAtBeginning();*/}}
                        CusButton{text:"视岛图";onClicked:{console.log((content.height-10)/4*0.9-4);/*content.setIndex()*/}}
                        }
                    }
                }

            Item{
                height: parent.height;width:parent.width*0.25;
                Item{
                    id: item2
                    anchors.fill: parent
                    anchors.margins:parent.height*0.15;
                    CusButton{text:"删除"; anchors.horizontalCenter: parent.horizontalCenter;onClicked: console.log(currentPatient.name);}
                }
                }


            Item{
                height: parent.height;width:parent.width*0.15;
                Item{
                    id: item1
                    anchors.fill: parent
                    anchors.margins:parent.height*0.15;
                    CusButton{
                        text:"分析"; anchors.right: parent.right;
                        onClicked:
                        {
                            changePage("analysis",{pageFrom:"check",report:queryStrategy.report,program:currentProgram,checkResult:currentCheckResult,analysisResult:analysisResult});
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
