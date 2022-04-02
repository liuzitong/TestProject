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
    width:1366; height: 720;
//    property alias contentWidth : idContent.implicitWidth;
//    property alias contentHeight: idContent.implicitHeight;
    property int type;
    property int strategy;
    property int range;
    signal ok();
    signal cancel();

   contentItem:
   Rectangle{
        id: idContent; color: "#60606060";implicitWidth: idPopup.width; implicitHeight: idPopup.height;
        Rectangle
        {
        // [HINT] Popup element need implicitWidth & implicitHeight to calc. the right position
            id: menu; width:idPopup.width*0.2; height: idPopup.height*0.4;color: "#dcdee0";radius: 5;/*width:480; height:480;*/
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            border.color: "#7C7C7C";
            Column{
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
                    CusText{text:"创建程序";}
                }
                Item{
                    width: parent.width;height: parent.height-header.height;
                    Item{ anchors.topMargin: parent.height*0.08;anchors.bottomMargin:parent.height*0.08; anchors.leftMargin:parent.height*0.12; anchors.rightMargin:parent.height*0.12;anchors.fill: parent;
                        Column{anchors.fill: parent;spacing:parent.height*(1-0.16*4)/3;
                            Row{width:parent.width;height: parent.height*0.16;spacing: width*0.05;
                                CusText{text:"范围:"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                NumberLineEdit{
                                    id:rangeEdit;
                                    width: parent.width*0.7;
                                    height:parent.height;
                                    step:1;
                                    min:5;
                                    max:90;
//                                    readOnly: true;
                                    onValueChanged: {range=value;}
                                }
                            }
                            Row{width:parent.width;height: parent.height*0.16;spacing: width*0.05;
                                CusText{text:"类别:"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                CusComboBox{
                                    id:typeComboBox;
                                    width: parent.width*0.7;
                                    height:parent.height;
                                    imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                                    model: ["阈值" ,"筛选","动态" ];
                                    onCurrentIndexChanged: type=currentIndex;
                                }
                            }
                            Row{width:parent.width;height: parent.height*0.16;spacing: width*0.05;
                                CusText{text:"策略:"; horizontalAlignment: Text.AlignLeft;width: parent.width*0.25}
                                CusComboBox{
                                    property var strategies: [["全阈值","智能交互式","快速智能交互式"],["二区法","三区法","量化缺损","单刺激"],["标准动态","盲区动态","暗区动态","直线动态"]];
                                    width: parent.width*0.7;
                                    height:parent.height;
                                    imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
//                                    model: ListModel {ListElement { text: "阈值" } ListElement { text: "筛选" } ListElement { text: "移动" }}
                                    model:strategies[typeComboBox.currentIndex];
                                    onCurrentIndexChanged: if(type!=1){strategy=currentIndex;}else{strategy=currentIndex+3;}
                                }
                            }
                            Item{
                                id:okCancel
                                width:parent.width;height: parent.height*0.16 ;
                                CusButton{ text:"确定"; anchors.verticalCenter: parent.verticalCenter;anchors.left: parent.left;onClicked: {idPopup.close();ok()}}
                                CusButton{ text:"取消"; anchors.right: parent.right; anchors.verticalCenter: parent.verticalCenter;onClicked: {idPopup.close();}}
                            }
                        }
                    }
                 }

            }
        }
    }
}
