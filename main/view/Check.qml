import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0


Column {
    id:root
    anchors.fill:parent;
    Item{
        width: parent.width;
        height: parent.height*14/15
        id:content;
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
                        CusButton{text:"返回";onClicked:IcUiQmlApi.appCtrl.changePage("patientManagement");}
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
                        anchors.fill: parent
                        anchors.margins:parent.height*0.15;
                        Flow{
                            height: parent.height;spacing: height*0.8
                            anchors.horizontalCenter: parent.horizontalCenter
                            CusButton{text:"开始测试";}
                            CusButton{text:"停止测试";}
                            CusButton{text:"切换眼别";}
                            CusButton{text:"打印";}
                        }
                    }
                }

            }
        }


//    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
