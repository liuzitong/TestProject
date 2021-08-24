import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import QtQuick.Layouts 1.3

Column {id:root; width: 1366;height: 691;anchors.fill:parent;
    signal changePage(var pageName);
    function rePaintCanvas(){dbDisplay.displayCanvas.requestPaint();}
    Rectangle{id:content;width: parent.width;height: parent.height*14/15
        Item{anchors.fill: parent;anchors.margins: 2;
            Row{anchors.fill: parent;spacing: 2;
                Column{width:parent.width*0.235;height: parent.height;spacing:2;
                    Column{id: column;width: parent.width; height: parent.height*0.6;
                        Rectangle{height: parent.height*0.1;width: parent.width;color:"#D2D2D3"
                            TabBar {id: bar;height: parent.height*0.8;anchors.bottom: parent.bottom;anchors.right: parent.right; anchors.rightMargin:0.03*parent.width;
                                anchors.left: parent.left; anchors.leftMargin: 0.03*parent.width;spacing: 0;currentIndex: 0;
//                                Repeater { model:["阈值","筛选","特殊","动态"]
                                    TabButton {width: bar.width*0.25;height: parent.height;
                                        Rectangle{anchors.fill: parent;color:parent.checked? "#E3E5E8":"#D2D2D3";
                                            Rectangle{width: parent.width;height: 3;color: "#0064B6";visible: parent.parent.checked? true:false; }
                                            CusText{text:modelData; anchors.fill: parent;color:parent.parent.checked?"#0064B6":"black";font.pointSize: height*0.3}
                                        }
                                    }
//                                }
                            }
                        }
                        Rectangle{width: parent.width;height: parent.height*0.9;color: "#DCDEE0";
                            StackLayout {anchors.fill: parent;currentIndex: bar.currentIndex
                                Item {id: homeTab;anchors.fill: parent;anchors.margins: height*0.03;
                                    ListView {id:listView;spacing: -1;anchors.fill: parent;model: ggg;delegate: delegatebb;clip:true;snapMode: ListView.SnapPosition;}
                                    Component{id:delegatebb;
                                        Rectangle{height: (homeTab.height-1)*1/10+1;width: listView.width;color: white;border.color: "black";
                                            CusText{width: parent.width;text:model.name}
                                        }
                                    }
//                                    ListModel {id:ggg
//                                        ListElement {name: "Bill Smith";number: "555 3264" }
//                                        ListElement { name: "John Brown"; number: "555 8426";}
//                                        }
//                                    }
                                }
                            }
                        }
                    }
                    Column{width: parent.width;height: parent.height*0.4-2
                        Rectangle{width: parent.width;height: parent.height*0.11;color:"#D2D2D3";
                            CusText{text:"策略"; horizontalAlignment: Text.AlignHCenter; anchors.fill: parent}
                        }
                        Rectangle{width: parent.width;height: parent.height*0.89;color:"#DCDEE0";
                            StackLayout {anchors.fill: parent;currentIndex: switch(bar.currentIndex) {case 0:return 0;case 1:case 2:return 1;case 3: return 2;}
                                Item{anchors.fill: parent;anchors.margins: 0.1*height;
                                    Column{anchors.fill: parent;spacing: height/7;
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("阈值"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("智能交互式"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("快速智能交互式"); horizontalAlignment: Text.AlignLeft}
                                        }
                                    }
                                }

                                Item{anchors.fill: parent;anchors.margins: 0.1*height;
                                    Column{anchors.fill: parent;spacing: height/7;
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("二区法"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("三区法"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("量化缺损"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("单刺激"); horizontalAlignment: Text.AlignLeft}
                                        }
                                    }
                                }

                                Item{anchors.fill: parent;anchors.margins: 0.1*height;
                                    Column{anchors.fill: parent;spacing: height/7;
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("标准动态"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("盲区动态"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("暗区动态"); horizontalAlignment: Text.AlignLeft}
                                        }
                                        Row{width: parent.width;height: parent.height/7;spacing: 0.5*height;
                                            CusCheckBox{height: parent.height; checked: true;width: parent.height;}
                                            CusText{text:qsTr("直线动态"); horizontalAlignment: Text.AlignLeft}
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
                                CusButton{text:"新建";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"取消";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"保存";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"删除";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"清除";height: parent.width*0.3;width: parent.width;}
                                CusButton{text:"复制";height: parent.width*0.3;width: parent.width;}
                            }
                        }
                    }
                }
            }
        }
    }
    Rectangle{id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
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
