import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import perimeter.main.view.Controls 1.0


Window {
    id: window;visible: true;width: 1366;height: 768;title: qsTr("Hello World");
    minimumWidth: 960;minimumHeight: 640;color: backGroudColor;
    property int commonRadius:height*0.03
    property string backGroudColor:"#dcdee0"
    property string backGroudBorderColor:"#bdc0c6"
    Column {id: column;anchors.fill: parent
        Rectangle{
            Component.onCompleted: {
                console.log("aaaa");
            }

            id:topRibbon;width: parent.width;height: parent.height*0.1;color: "#333e44";}
        Item{
            id:content;width:parent.width;height: parent.height*0.84;
            property int layGroupMargin: 0.03*height
            Row{
                anchors.fill: parent;anchors.margins: content.layGroupMargin;spacing:content.layGroupMargin
                Rectangle{
                    width: (parent.width-content.layGroupMargin)*0.6;height: parent.height;
                    border.color: backGroudBorderColor;color: backGroudColor;radius: commonRadius
                    Keys.onRightPressed: {console.log("up");}
                    Item{
                        anchors.fill: parent;anchors.margins: content.layGroupMargin
                        Column{
                            id:patientManagement;anchors.fill: parent;spacing: rowHight
                            property int rowHight: height*0.05
                            Row{
                                id:dateSelection; width: parent.width;
                                height: patientManagement.rowHight;spacing: height*0.4;
                                CusText{text: "检查日期";horizontalAlignment: Text.AlignLeft;width: height*2.5}
                                LineEdit{id:dateFrom;radius: height/6;width: height*4; Keys.onRightPressed: {console.log("xxxxxxxxxxxx");}}
                                CusButton{id:dateFromButton;text:"选择";width:height*2;onClicked: calendar.open()}
                                CusText{text:"到";width: height*0.6}
                                LineEdit{id:dateTo;radius: height/6;width: height*4}
                                CusButton{id:dateToButton;text:"选择";width:height*2}
                                Keys.onRightPressed: {console.log("dow");}
                            }
                            Row{
                                id:query; width: parent.width; height: patientManagement.rowHight;spacing: height*0.5
                                CusComboBox{
                                    id:queryStrategy;height: parent.height;width: parent.height*5;
                                    borderColor: backGroudBorderColor;font.family:"Microsoft YaHei";
                                    imageSrc: "qrc:/perimeter/main/view/Assets/Pics/base-svg/btn_drop_down.svg";
                                    model: ListModel {ListElement { text: "最近诊断" } ListElement { text: "患者ID" } ListElement { text: "姓名" }ListElement { text: "性别" }ListElement { text: "出身日期" }}
                                }
                                Flow{
                                    id:singleNameQuery;height: parent.height
                                    LineEdit{id:queryIDorName;radius:height/6;width: height*4}
                                }
                                CusButton{height: parent.height;width: height;imageSrc:"qrc:/perimeter/main/view/Assets/Pics/base-svg/btn_find.svg"}}

                            Item{
                                width: parent.width;height: parent.height-6*patientManagement.rowHight
                                CusCalendar{id:calendar;inputObj: dateFrom;}
//                                DatePick{}
                            }
                            Row{
                                id:pageControl;width: parent.width;height: patientManagement.rowHight;
                            }
                        }
                    }
                }
                Column{
                    width: (parent.width-content.layGroupMargin)*0.4; height: parent.height;spacing:content.layGroupMargin;
                    Rectangle{
                        width: parent.width;height: (parent.height-content.layGroupMargin)*0.6;
                        border.color: backGroudBorderColor;color: backGroudColor;radius: commonRadius;
                        Column{
                            id:newPatient;
                        }
                    }
                    Rectangle{
                        width: parent.width;height: (parent.height-content.layGroupMargin)*0.4;
                        border.color: backGroudBorderColor;color: backGroudColor;radius: commonRadius;
                    }
                }
            }
        }
        Rectangle{
            id:bottomRibbon;width: parent.width;height: parent.height*0.06;color: "#333e44";
        }
    }
}







/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}
}
##^##*/
