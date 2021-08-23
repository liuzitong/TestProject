import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import QtMultimedia 5.8


Column {
    id:root
    width: 1366;height: 691
//    anchors.fill:parent;
    signal changePage(var pageName);
    property string backGroundColor:"#dcdee0"
    property string backGroundColorCheckPanel:"#cbced0"
    property string backGroundBorderColor:"#bdc0c6"
    Rectangle{
        width: parent.width;
        height: parent.height*14/15;
        id:content;
        Item{
            anchors.fill: parent;anchors.margins: 2;
            Row{
                anchors.fill: parent;spacing: 2;
                Rectangle{
                    width: parent.width*0.25-2;height: parent.height;color: backGroundColor;
                    Item{
                        anchors.fill: parent;anchors.margins: parent.height*0.02
                        Column{
                            anchors.fill: parent;spacing:height* 0.12
                            Rectangle{
                                id: rectangle
                                width: parent.width;height: parent.height*0.28
                                anchors.horizontalCenter: parent.horizontalCenter
                                border.color: backGroundBorderColor;
                                color: backGroundColor;
                                radius: width*0.03
                                Item{
                                    anchors.fill: parent;anchors.margins: parent.height*0.1;
                                    Column{
                                        anchors.fill: parent;spacing: 0.175*height;
                                        Row{
                                            width:parent.width;height: parent.height*0.65/3;spacing: width*0.05;
                                            CusText{text:"程序名"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit
                                            {
                                                id:programDisplay;text:"驾驶员单眼快速筛查程序"; width: parent.width*0.70;textfeild.readOnly: true;
                                                textfeild.font.pointSize:if(IcUiQmlApi.appCtrl.language!=="Chinese") return programDisplay.height*0.29;
                                            }
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*0.65/3;spacing: width*0.05;
                                            CusText{text:"光标"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"I,白色";width: parent.width*0.70;textfeild.readOnly: true;}
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*0.65/3;spacing: width*0.05;
                                            CusText{text:"策略"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"二区法";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                    }


                                }
                            }
                            Rectangle{
                                width: parent.width;height: parent.height*0.28
                                anchors.horizontalCenter: parent.horizontalCenter
                                border.color: backGroundBorderColor;
                                color: backGroundColor;
                                radius: width*0.03
                                Item{
                                    anchors.fill: parent;anchors.margins: parent.height*0.1;
                                    Column{
                                        anchors.fill: parent;spacing: 0.175*height;
                                        Row{
                                            width:parent.width;height: parent.height*0.65/3;spacing: width*0.05;
                                            CusText{text:"假阳性率"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/0";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*0.65/3;spacing: width*0.05;
                                            CusText{text:"假隐形率"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/0";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*0.65/3;spacing: width*0.05;
                                            CusText{text:"固视丢失率"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/0";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                    }


                                }

                            }
                            Rectangle{
                                width: parent.width;height: parent.height*0.20
                                anchors.horizontalCenter: parent.horizontalCenter
                                border.color: backGroundBorderColor;
                                color: backGroundColor;
                                radius: width*0.03
                                Item{
                                    anchors.fill: parent;anchors.margins: parent.height*0.13;
                                    Column{
                                        anchors.fill: parent;spacing: 1/3*height;
                                        Row{
                                            width:parent.width;height: parent.height*1/3;spacing: width*0.05;
                                            CusText{text:"测试点数"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/4";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*1/3;spacing: width*0.05;
                                            CusText{text:"测试时间"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"00:00";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }

                                    }

                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width*0.25-2;height:parent.height;color: backGroundColor;
                    Item{
                        anchors.fill: parent;anchors.margins: parent.height*0.02
                        Column{
                            id: column
                            anchors.fill: parent;spacing:height* 0.02
                            Item{
                                width: parent.width*0.83;height: width*3/4
                                anchors.horizontalCenter: parent.horizontalCenter
                                Rectangle{
                                    anchors.fill: parent;color:"black";
                                }

                                MediaPlayer{
                                    id:vedio;
                                }
                            }
                            Item{
                                id:controlPanel;
                                width:parent.width*0.6
                                height: width*0.8
                                anchors.horizontalCenter: parent.horizontalCenter

//                                Rectangle{
//                                    anchors.fill: parent;
//                                    border.color: "black";border.width: 1;
//                                }
                                CusButton{
                                    id:autoButton;width: parent.width*0.4;height: parent.height*0.25;buttonColor: backGroundColor;
                                    text:"Auto";borderColor: "black";
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                }


                                Image {
                                    id: upButton
                                    anchors.left: parent.Top;
                                    height: sourceSize.height*root.height/691;width: sourceSize.width*root.width/1366;
                                    anchors.horizontalCenter: parent.horizontalCenter;
                                    source: "qrc:/Pics/capture-svg/arrow_1up.svg";
                                }

                                Image {
                                    id: downButton
                                    anchors.bottom: parent.bottom;
                                    height: sourceSize.height*root.height/691;width: sourceSize.width*root.width/1366;
                                    anchors.horizontalCenter: parent.horizontalCenter;
                                    source: "qrc:/Pics/capture-svg/arrow_2down.svg";
                                }

                                Image {
                                    id: leftButton
                                    anchors.left: parent.left;
                                    anchors.verticalCenter: parent.verticalCenter;
                                   height: sourceSize.height*root.height/691;width: sourceSize.width*root.width/1366;
                                    source: "qrc:/Pics/capture-svg/arrow_3left.svg";
                                }
                                Image {
                                    id: rightButton
                                    anchors.right: parent.right;
                                    height: sourceSize.height*root.height/691;width: sourceSize.width*root.width/1366;
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: "qrc:/Pics/capture-svg/arrow_4right.svg";
                                }
                            }



                            Rectangle{
                                width: parent.width*0.83;height: parent.height*0.40
                                anchors.horizontalCenter: parent.horizontalCenter
                                border.color: backGroundBorderColor;
                                color: backGroundColor;
                                radius: width*0.03
                                Item{
                                    anchors.fill: parent;anchors.margins: parent.height*0.1;
                                    Column{
                                        anchors.fill: parent;spacing: 0.10*height;
                                        Row{
                                            width:parent.width;height: parent.height*0.65/4;spacing: width*0.05;
                                            CusText{text:"假阳性率"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/0";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*0.65/4;spacing: width*0.05;
                                            CusText{text:"假隐形率"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/0";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*0.65/4;spacing: width*0.05;
                                            CusText{text:"固视丢失率"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/0";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                        Row{
                                            width:parent.width;height: parent.height*0.65/4;spacing: width*0.05;
                                            CusText{text:"固视丢失率"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                            LineEdit{text:"0/0";width: parent.width*0.7;textfeild.readOnly: true;}
                                        }
                                    }


                                }

                            }

                        }
                    }



                }
                Rectangle{
                    width: parent.width*0.5;height: parent.height;color:backGroundColorCheckPanel;
                }
            }
        }


    }
    Rectangle{
            id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
            Row{
                anchors.fill: parent;
                Item{
                    height: parent.height;width:parent.width*0.25;
                    Item{
                        anchors.fill: parent
                        anchors.margins:parent.height*0.15;
                        CusButton{text:"返回";onClicked:root.changePage("patientManagement");}
                    }
                }

                Item{
                    height: parent.height;width:parent.width*0.25;
                    Item{
                        anchors.fill: parent
                        anchors.margins:parent.height*0.15;
                        Flow{
                            height: parent.height;spacing: height*0.8
                            anchors.horizontalCenter: parent.horizontalCenter
                            CusButton{text:"选择程序";}
                            CusButton{text:"参数选择";}
                        }
                    }
                }

                Item{
                    height: parent.height;width:parent.width*0.50;
                    Item{
                        id: item1
                        anchors.fill: parent
                        anchors.margins:parent.height*0.15;
                        Flow{
                            height: parent.height;spacing: height*0.8
                            anchors.horizontalCenter: parent.horizontalCenter
                            CusButton{text:"开始测试";}
                            CusButton{text:"停止测试";}
                            CusButton{text:"切换眼别";}
                        }
                        CusButton{text:"打印"; anchors.right: parent.right; }
                    }
                }

            }
        }


//    }
}
