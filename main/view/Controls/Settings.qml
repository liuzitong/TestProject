import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import qxpack.indcom.ui_qml_base 1.0     // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0  // [HINT] ModalPopupDialog is in it

ModalPopupDialog {   // this is the wrapped Popup element in ui_qml_contro
    id:idPopup
    property alias color: idContent.color;
    reqEnterEventLoop:false;
    width:1280; height: 960;
//    property alias contentWidth : idContent.implicitWidth;
//    property alias contentHeight: idContent.implicitHeight;

   contentItem:
   Rectangle{
        id: idContent; color: "#60606060";implicitWidth: idPopup.width; implicitHeight: idPopup.height;
        Rectangle
        {
        // [HINT] Popup element need implicitWidth & implicitHeight to calc. the right position
            id: menu; /*width:idPopup.width/3; height: idPopup.height/2;*/ color: "#dcdee0";radius: 5;width:480; height:480;
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
                    CusText{text:"设置";width: height*1.4}
                }
                Item{
                    width: parent.width;height: parent.height-header.height;
                    Item{
                        anchors.fill: parent;anchors.margins:height/10
                        Column{
                            id: column
                            property int rowHeight: height/12;spacing: rowHeight*1.2;
                            anchors.fill: parent;
                            Item{width: parent.width;height:column.rowHeight*0.1;}
                            Flow{
                                height: column.rowHeight; width: parent.width*0.6;anchors.horizontalCenter: parent.horizontalCenter;
                                CusText{text:"语言选择";width:parent.width*0.35}
                                CusComboBox{
                                    id:languageSelection;height: parent.height;width:parent.width*0.65;
                                    borderColor: backGroundBorderColor;font.family:"Microsoft YaHei";
                                    imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                                    model: ListModel {ListElement { text: "中文" } ListElement { text: "English" } ListElement { text: "其它" } }
                                }
                            }
                            Flow{
                                height: column.rowHeight;width: parent.width*0.6; anchors.horizontalCenter: parent.horizontalCenter;
                                CusText{text:"医院名称";width: parent.width*0.35}
                                LineEdit{height: column.rowHeight;width:parent.width*0.65;}
                            }

                            Flow{
                                height: column.rowHeight;width: parent.width*0.6; anchors.horizontalCenter: parent.horizontalCenter;
                                CusText{text:"双姓名输入";width: parent.width*0.35}
                                CheckBox{
                                    id:doubleName;height: column.rowHeight;width: height;
                                    onClicked:
                                    {
                                        console.log(doubleName.checked);
                                    }
                                }
                            }
                        }
                        Flow{
                            height: column.rowHeight; anchors.right: parent.right;  anchors.bottom: parent.bottom;spacing: 1.2*column.rowHeight;
                            CusButton
                            {
                                text:"确定";
                                onClicked:
                                {
                                    console.log("hello");
                                    if(languageSelection.currentIndex==0) IcUiQmlApi.appCtrl.language="Chinese";
                                    if(languageSelection.currentIndex==1) IcUiQmlApi.appCtrl.language="English";
                                    console.log(doubleName.checked);
                                    IcUiQmlApi.appCtrl.doubleName=doubleName.checked;
                                    console.log(IcUiQmlApi.appCtrl.doubleName);
                                    idPopup.close();
                                }
                            }
                            CusButton{text:"取消";onClicked: {idPopup.close();}}
                        }
                    }

                }
            }
        }
    }
}
