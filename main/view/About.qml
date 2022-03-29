import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0     // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0  // [HINT] ModalPopupDialog is in it

ModalPopupDialog {   // this is the wrapped Popup element in ui_qml_contro
    id:idPopup
    property alias color: idContent.color;
    reqEnterEventLoop:false;
//    width:1280; height: 960;
//    property alias contentWidth : idContent.implicitWidth;
//    property alias contentHeight: idContent.implicitHeight;

   contentItem:
   Rectangle{
        id: idContent; color: "#60606060";implicitWidth: idPopup.width; implicitHeight: idPopup.height;
        Rectangle
        {
        // [HINT] Popup element need implicitWidth & implicitHeight to calc. the right position
            id: menu; width:idPopup.width*0.35; height: idPopup.height*0.6;color: "#dcdee0";radius: 5;/*width:480; height:480;*/
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            border.color: "#7C7C7C";
            Column{
                anchors.fill: parent;
                Canvas{
                    id:header;height: parent.height/12;width: parent.width;
                    property alias radius: menu.radius;
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.beginPath(); // Start the path
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
                    CusText{text:"关于";width: height*1.4}
                }
                Item{
                    width: parent.width;height: parent.height-header.height;
                    Item{
                        anchors.top: parent.top;
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left;
                        anchors.right:parent.right;
                        anchors.rightMargin: parent.height/5;     //possible anchor loop
                        anchors.leftMargin: parent.height/5;
                        anchors.bottomMargin: parent.height/10;
                        anchors.topMargin: parent.height/10;
                        Column{
                            id: column
                            property int rowHeight: height/12;
                            anchors.fill: parent;
                            Item{
                                height:parent.height*0.11;width: parent.width;CusText{width:parent.width;text:"电脑自动视野仪系统"; fontSizeMode: Text.VerticalFit;
                                    anchors.horizontalCenter: parent.horizontalCenter;font.pointSize:height*0.5;
                                }
                            }
                            Item{height:parent.height*0.06;width: parent.width; }

                            Item{ height:parent.height*0.065;width: parent.width;
                                Image {
                                    id: name
                                    anchors.horizontalCenter: parent.horizontalCenter;
                                    source: "qrc:/Pics/base-svg/2logo_version.svg"
                                    height: parent.height;width:height/sourceSize.height*sourceSize.width;
                                }
                            }
                            Item{height:parent.height*0.06;width: parent.width; }

                            CusText{
                                height:parent.height*0.05;width: parent.width;
                                text: "型    号: Perimeter" ;horizontalAlignment: Text.AlignLeft;
                                font.pointSize:height*0.6;
                            }
                            Item{height:parent.height*0.06;width: parent.width; }

                            CusText{
                                height:parent.height*0.05;width: parent.width;
                                text: "软件版本: 1.0"
                                horizontalAlignment: Text.AlignLeft;
                                font.pointSize:height*0.6;
                            }

                            Item{
                                height:parent.height*0.32;width: parent.width;
                            }

                            CusText{
                                height:parent.height*0.05;width: parent.width;
                                text: "版权所有 ©2019 重庆贝奥新视野医疗设备有限公司"
                                horizontalAlignment: Text.AlignLeft;
                                font.pointSize:height*0.6;
                            }
                            Item{height:parent.height*0.12;width: parent.width; }



                        }
                        Item{
                            height: parent.height*0.08; anchors.bottom: parent.bottom; width: parent.width;
                            CusButton{
                                text:"确定"
                                anchors.horizontalCenter: parent.horizontalCenter
                                onClicked: {
                                    idPopup.close();
                                }
                            }
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
