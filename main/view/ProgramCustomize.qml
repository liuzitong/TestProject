import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import QtQuick.Layouts 1.3

Column {
    id:root; width: 1366;height: 691;
    anchors.fill:parent;
    signal changePage(var pageName);
    Rectangle{id:content;width: parent.width;height: parent.height*14/15
        Item{anchors.fill: parent;anchors.margins: 2;
            Row{anchors.fill: parent;spacing: 2;
                Column{width:parent.width*0.235;height: parent.height;spacing:2;
                    Column{id: column;width: parent.width; height: parent.height*0.6;
                        Rectangle{height: parent.height*0.1;width: parent.width;color:"#D2D2D3"
                            TabBar {
                                id: bar
                                height: parent.height*0.8;
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right; anchors.rightMargin:0.03*parent.width;
                                anchors.left: parent.left; anchors.leftMargin: 0.03*parent.width;
                                spacing: 0;
                                currentIndex: 0;
//                                Repeater {
//                                    model:["阈值","筛选","特殊","动态"]
                                    TabButton {
                                        width: bar.width*0.25;height: parent.height;
                                        Rectangle{
                                            anchors.fill: parent;color:parent.checked? "#E3E5E8":"#D2D2D3";
                                            Rectangle{
                                                width: parent.width;height: 3;color: "#0064B6";visible: parent.parent.checked? true:false;
                                            }
                                            CusText{text:"modelData"; anchors.fill: parent;color:parent.parent.checked?"black": "#0064B6"}
                                        }

                                    }
//                                }
                            }
                        }


                        StackLayout {
                            width: parent.width;height: parent.height*0.9;
                            currentIndex: bar.currentIndex
                            Rectangle{anchors.fill: parent;color: "#DCDEE0";}
                            Item {
                                id: homeTab;anchors.fill: parent;anchors.margins: height*0.03;
                                ListView{
                                    anchors.fill: parent;
                                }
                            }

                        }
                    }
                    Rectangle{
                        width: parent.width;
                        height: parent.height*0.4-2;
                    }
                }
                Column{
                    width:parent.width*0.648-2;height: parent.height;
                    Rectangle{
                        anchors.fill: parent;color: "red";
                    }
                }
                Column{
                    width:parent.width*0.117-2;height: parent.height;
                    Rectangle{
                        anchors.fill: parent;color: "yellow";
                    }
                }
            }
        }

    }
    Rectangle{id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
        Row{anchors.fill: parent;
            Item{height: parent.height;width:parent.width*0.25;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"返回";onClicked:root.changePage("patientManagement");}
                    }
                }
            Item{height: parent.height;width:parent.width*0.6;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    Flow{height: parent.height;spacing: height*0.8;anchors.horizontalCenter: parent.horizontalCenter
                        CusButton{text:"参数设置";}
                        CusButton{text:"圆形选点";}
                        CusButton{text:"矩形选点";}
                        }
                    }
                }
            Item{height: parent.height;width:parent.width*0.15;
                Item{anchors.fill: parent;anchors.margins:parent.height*0.15;
                    CusButton{text:"解锁"; anchors.horizontalCenter: parent.horizontalCenter}
                }
            }
        }
    }
}
