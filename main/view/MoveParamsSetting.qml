import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0     // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0  // [HINT] ModalPopupDialog is in it

ModalPopupDialog /*Rectangle*/{   // this is the wrapped Popup element in ui_qml_contro
    id:idPopup
    property alias color: idContent.color;
    reqEnterEventLoop:false;
    width:1366; height: 640;
//    property alias contentWidth : idContent.implicitWidth;
//    property alias contentHeight: idContent.implicitHeight;
    property string backGroundColor:"#dcdee0"
    property string backGroundBorderColor:"#bdc0c6"
    property int type;
    signal ok();
    signal cancel();

    property int range;
    property int dotCount;
    property int strategy;
    property int cursorColor;
    property int cursorSize;
    property int backgroundLightColor;

    property int fixationViewSelection;
    property int moveSpeed;
    property int moveLight;
    property int moveMethod;
    property int moveDistance;
    property int alarmMode;


   contentItem:
   Rectangle{
        id: idContent; color: "#60606060";implicitWidth: idPopup.width; implicitHeight: idPopup.height;
        Rectangle
        {
        // [HINT] Popup element need implicitWidth & implicitHeight to calc. the right position
            id: menu; width:idPopup.width*0.5; height: idPopup.height*0.98;color: "#dcdee0";radius: 5;/*width:480; height:480;*/
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            border.color: "#7C7C7C";
            Column{
                id: column
                anchors.fill: parent;
                Canvas{
                    id:header;height: idPopup.height*0.05;width: parent.width;property alias radius: menu.radius;
                    onPaint: {var ctx = getContext("2d");ctx.beginPath(); // Start the path
                        ctx.moveTo(1,height);
                        ctx.lineTo(1, radius-1); // Set the path origin
                        ctx.arc(radius,radius,radius-1,Math.PI,Math.PI*1.5);
                        ctx.lineTo(width-radius, 1);// Set the path destination
                        ctx.arc(width-radius,radius,radius-1,Math.PI*1.5,0);
                        ctx.lineTo(width-1, height);
                        ctx.closePath();
                        context.fillStyle = "#D2D2D3"
                        ctx.fill();
                    }
                    CusText{text:"参数设置";}
                }
                Column{
                    width: parent.width;height: parent.height-header.height;
                    Rectangle{
                        id: rectangle
                        height: parent.height*0.065;width:parent.width-2;color: "#D2D2D3";anchors.horizontalCenter: parent.horizontalCenter;
                        TabBar {id: bar;height: parent.height*0.8; anchors.bottom: parent.bottom; anchors.bottomMargin: 0;anchors.left: parent.left; anchors.leftMargin: 0.01*parent.width;spacing: 0;currentIndex: 0;width:parent.width*0.15;
                            Repeater { model:["常用参数"]
                                TabButton {width: bar.width;height: parent.height;
                                    Rectangle{anchors.fill: parent;color:parent.checked? "#E3E5E8":"#D2D2D3";
                                        Rectangle{width: parent.width;height: 3;color: "#0064B6";visible: parent.parent.checked? true:false; }
                                        CusText{text:modelData; anchors.fill: parent;color:parent.parent.checked?"#0064B6":"black";font.pointSize: height*0.25}
                                    }
                                }
                            }
                        }
                    }

                    StackLayout {
                        width: parent.width;height: parent.height*0.755;currentIndex: bar.currentIndex;
                        Rectangle{
                            anchors.fill: parent;
                            anchors.margins: parent.width*0.05;
                            radius: width*0.02;
                            id:eyeOptionsGroup;
                            width: parent.width*0.83;
                            height: parent.height*0.35;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            border.color: backGroundBorderColor;
                            color: backGroundColor;
                            Row{
                                anchors.fill: parent;
                                anchors.margins: parent.width*0.04;
                                spacing:width*0.1;
                                property int rowHeight:height*0.08
                                Column{
                                    height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.8;
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"测试范围"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                        LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;}
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"测试点数"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                        LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;}
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"策略"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                        LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;}
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"光标颜色"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                        CusComboBox{width: parent.width*0.6; anchors.right: parent.right;model:["白色","红色","蓝色"];}
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"光标大小"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                        CusComboBox{width: parent.width*0.6; anchors.right: parent.right;model:["I","II","III","IV","V"];}
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"背景光颜色"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                        CusComboBox{width: parent.width*0.6; anchors.right: parent.right;model:["白色","黄色"];}
                                    }
                                }
                                Column{
                                   height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.8;
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"策略"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                       LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;text:"中心点"}
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"移动速度"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                       CusComboBox{width: parent.width*0.6; anchors.right: parent.right;model:["1°/秒","2°/秒","3°/秒","4°/秒","5°/秒","6°/秒","7°/秒","8°/秒","9°/秒"];}
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"移动亮度"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                       SpinBox{height:parent.height; wheelEnabled: true;width: parent.width*0.6; editable: true; value: 30; to: 51;  anchors.right: parent.right;font.family: "Microsoft YaHei";font.pointSize: height*0.3}
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"移动方式"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                       CusComboBox{width: parent.width*0.6; anchors.right: parent.right;model:["4","6","8"];}
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"移动距离"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                       CusComboBox{width: parent.width*0.6; anchors.right: parent.right;model:["5","10","15"];}
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"眼动模式报警"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;}
                                       CusComboBox{width: parent.width*0.6; anchors.right: parent.right;model:["不报警","只报警","报警并暂停"];}
                                   }
                                }

                            }

                        }
                        Rectangle{
                            anchors.fill: parent;
                            anchors.margins: parent.width*0.04;
                            radius: width*0.02;
                            width: parent.width*0.83;
                            height: parent.height*0.35;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            border.color: "red";
                            color: "red";
                        }
                    }

                    Item {
                        width: parent.width;
                        height: parent.height*0.18
//                        anchors.margins: height*0.3;
                        Flow{
                            anchors.fill: parent;
                            anchors.margins: parent.height*0.33;
                            spacing: height;
                            layoutDirection: Qt.RightToLeft
                            anchors.verticalCenter: parent.verticalCenter
                            CusButton{text:"取消";onClicked: idPopup.close();}
                            CusButton{text:"确定";onClicked: idPopup.close();}
                        }
                    }
                }

            }
        }
    }
}
