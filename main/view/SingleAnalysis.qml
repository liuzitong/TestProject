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
    property string lastPage: null;
    property var currentPatient: null;
    property var currentProgram: null;
    property var currentCheckResult: null;
    property int textheight: height*0.05;
    signal refresh();
    width: 1366;height:686
    signal changePage(var pageName,var params);
    Item{
        width: parent.width;height: parent.height*14/15;id:content;
        Rectangle{width:parent.width*0.75;height: parent.height;color:"white";
            Row{anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.rightMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;
                Column{ id: column;width: parent.width*0.25;height: parent.height;spacing:parent.height*0.05
                    CusText{text:"30-2"; font.bold: true; horizontalAlignment: Text.AlignLeft;height:parent.height*0.10;}
                    Column{id:ttt;width:parent.width;height: parent.height*0.30;spacing: root.textheight*-0.36;
                        Repeater{model: [{name:"固视监测",param:"关闭"},{name:"固视目标",param:"中心"},{name:"固视丢失率",param:"0/0"},
                                {name:"假阳性率",param:"0%"},{name:"假阴性率",param:"0%"},{name:"测试用时",param:"02:31"},{name:"中心点",param:"关闭"}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:root.textheight;width: parent.width;}
                        }
//                        CusText{text:"固视监测";  horizontalAlignment: Text.AlignLeft;height:root.textheight;width: parent.width;}
                    }
                    Column{width:parent.width;height: parent.height*0.15;spacing: root.textheight*-0.36;
                        Repeater{model: [{name:"刺激光标",param:"III,白色"},{name:"背景光",param:"31.5 ASB"},{name:"策略",param:"快速智能交互式"}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:root.textheight;width: parent.width;}
                        }
                    }
                    Column{width:parent.width;height: parent.height*0.15;spacing: root.textheight*-0.36;
                        Repeater{model: [{name:"固视监测",param:"关闭"},{name:"固视目标",param:"中心"},{name:"固视丢失率",param:"0/0"}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:root.textheight;width: parent.width;}
                        }
                    }
                    Item{ id: item2;width:parent.width;height: parent.height*0.10;anchors.left: parent.left;anchors.leftMargin: (width-322)/2
                        EyeDeviationCurve{ anchors.verticalCenter: parent.verticalCenter ;  }
                    }
                }
                Column{width: parent.width*0.75;height:parent.height;
                    Row{width: parent.width;height:parent.height*1/3;
//                        VisionDiagram{height:parent.height;width:parent.height;
//                            degLocs: [{x:3,y:27},{x:9,y:27},{x:3,y:21},{x:9,y:21},{x:15,y:21},{x:3,y:15},{x:9,y:15},{x:15,y:15},{x:21,y:15},{x:3,y:9},{x:9,y:9},{x:15,y:9},{x:21,y:9},{x:27,y:9},{x:3,y:3},{x:9,y:3},{x:15,y:3},{x:21,y:3},{x:27,y:3}];range:30;type:0;os_od:0;
//                            values: [10,22,11,33,33,22,23,24,25,11,12,13,14,15,11,12,13,14,15]}
//                        VisionDiagram{height:parent.height;width:parent.height;
//                            degLocs: [{x:3,y:27},{x:9,y:27},{x:3,y:21},{x:9,y:21},{x:15,y:21},{x:3,y:15},{x:9,y:15},{x:15,y:15},{x:21,y:15},{x:3,y:9},{x:9,y:9},{x:15,y:9},{x:21,y:9},{x:27,y:9},{x:3,y:3},{x:9,y:3},{x:15,y:3},{x:21,y:3},{x:27,y:3}];range:30;type:1;os_od:0;
//                            values: [0,1,1,1,1,1,1,1,1,1,1,1,2,1,0,1,0,1,1]}}
//                    VisionDiagram{height:parent.height;width:parent.height;/*canvas.smooth:false;canvas.antialiasing: false;*/
//                        degLocs: [{x:3,y:27},{x:9,y:27},{x:3,y:21},{x:9,y:21},{x:15,y:21},{x:3,y:15},{x:9,y:15},{x:15,y:15},{x:21,y:15},{x:3,y:9},{x:9,y:9},{x:15,y:9},{x:21,y:9},{x:27,y:9},{x:3,y:3},{x:9,y:3},{x:15,y:3},{x:21,y:3},{x:27,y:3}];range:30;type:1;os_od:0;
//                        values: [7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,1,7,7]}
                    Image {
                        height:parent.height;width:parent.height;cache: false;        //to refresh image
                        Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/temp/30-2.bmp";})}
                    }
                    }

                    Row{width: parent.width;height:parent.height*1/3;}
                    Row{width: parent.width;height:parent.height*1/3;}
                }
            }
        }
    }
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
                        CusButton{text:"打印";onClicked: console.log(currentPatient.name); }
                        CusComboBox{
                            id:queryStrategy;height: parent.height;width: parent.height*3.5;
                            borderColor: backGroundBorderColor;font.family:"Microsoft YaHei";
                            imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                            model: ListModel {ListElement { text: "常规分析" } ListElement { text: "三合一图" } ListElement { text: "总览图" }}
                        }
                    }
                }
            }
        }
    }
}
