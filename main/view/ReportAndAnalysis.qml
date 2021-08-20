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
    signal changePage(var pageName);
    Item
    {
        width: parent.width;
        height: parent.height*14/15
        id:content;

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
                    CusButton{text:"返回";onClicked:root.changePage("patientManagement");}
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
                            id:queryStrategy;height: parent.height;width: parent.height*4;
                            borderColor: backGroundBorderColor;font.family:"Microsoft YaHei";
                            imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                            model: ListModel {ListElement { text: "常规分析" } ListElement { text: "三合一图" } ListElement { text: "总览图" }}}
                        CusButton{text:"进展分析";}
                        CusButton{text:"视岛图";}
                        }
                    }
                }

            Item{
                height: parent.height;width:parent.width*0.25;
                Item{
                    id: item2
                    anchors.fill: parent
                    anchors.margins:parent.height*0.15;
                    CusButton{text:"删除"; anchors.horizontalCenter: parent.horizontalCenter;}
                }
                }


            Item{
                height: parent.height;width:parent.width*0.15;
                Item{
                    id: item1
                    anchors.fill: parent
                    anchors.margins:parent.height*0.15;
                    CusButton{text:"打印预览"; anchors.right: parent.right;}
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
