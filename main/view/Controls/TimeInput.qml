//整个时间控件
import QtQuick 2.0

Rectangle{
    id:messagebox
    width: 300
    height: 240
    color: "#dff7fc"
    z: 10

    signal sendDate(var date)
    signal cancel()
        //顶部title
        Rectangle{
            id: title_rect;
            width: parent.width;
            height: 40;
            color: "#aeebf8";
            z:parent.z
            Text{
                anchors.left: parent.left;
                anchors.leftMargin: 5;
                anchors.verticalCenter: parent.verticalCenter;
                text: qsTr("选择日期");
                font.pixelSize: 20;
                color: "#09c3e9";
                font.family: "微软雅黑";
                z:parent.z
            }
        }


        Row{

            anchors.top: title_rect.bottom;
            anchors.topMargin: 15;
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 7;
            z:parent.z
//            TimeChangeBtn{
//                id: year;
//                name:qsTr("年")
//                property int currentYear: 0;
//                curTime: currentYear-40;
//                minTime: currentYear-110;
//                maxTime: currentYear;
//                z:parent.z
//            }
//            TimeChangeBtn{
//                id: month;
//                name:qsTr("月")
//                curTime: 6;
//                minTime: 1;
//                maxTime: 12;
//                z:parent.z
//            }
//            TimeChangeBtn{
//                id: day;
//                name:qsTr("日")
//                curTime: 15;
//                minTime: 1;
//                maxTime: 31;
//                z:parent.z
//            }
            //获取当前时间
            Component.onCompleted: {
                var date = new Date();
                year.currentYear    = date.getFullYear();
            }
        }


        //底部btn组
        Rectangle{
            color: "#aeebf8";
            width:  messagebox.width;
            height: 50;
            anchors.bottom: parent.bottom;
            Row{
                width: parent.width;
                spacing: 20;
                height: parent.height;
                anchors.left: parent.left;
                anchors.leftMargin: 20;
                anchors.horizontalCenter: parent.horizontalCenter;
                //取消btn
                Rectangle{
                    id:btn_cancel;
                    color: "white";
                    width: (parent.width-parent.spacing)/2;
                    height: 30;
                    anchors.verticalCenter: parent.verticalCenter;
                    Text{
                        anchors.centerIn: parent;
                        color: "#bfc0c0";
                        font.pixelSize: 16;
                        font.family: "微软雅黑";
                        text: qsTr("取消");
                    }

                    MouseArea{
                        id:mouseArea_cancel;
                        anchors.fill: parent;
                        propagateComposedEvents: true;
                        onClicked:
                        {
                            messagebox.visible = false
                            messagebox.cancel();
                        }
                    }
                }


                //确定btn
                Rectangle{
                    id:btn_ok;
                    color: "#09c3e9";
                    width: (parent.width-parent.spacing)/2;
                    height: 30;
                    anchors.verticalCenter: parent.verticalCenter;
                    Text{
                        anchors.centerIn: parent;
                        color: "white";
                        font.pixelSize: 16;
                        font.family: "微软雅黑";
                        text: qsTr("确定");
                    }

                    MouseArea{
                        id:mouseArea_ok;
                        anchors.fill: parent;
                        propagateComposedEvents: true;
                        onClicked:
                        {

                            var tempTime=year.curTime+"-"+month.curTime+"-"+day.curTime;
                            console.debug(tempTime);
                            messagebox.sendDate(tempTime)
                            messagebox.visible=false;
                        }
                    }
                }

            }


            //btn的点击效果
            states: [
                State {
                    name:"button_pressed_cancel"
                    when: mouseArea_cancel.pressed;
                    PropertyChanges {
                        target:btn_cancel;
                        color : Qt.darker("white",1.2);
                    }
                },
                State {
                    name:"button_pressed_ok"
                    when: mouseArea_ok.pressed;
                    PropertyChanges {
                        target:btn_ok;
                        color : Qt.darker("#09c3e9",1.2);
                    }
                }

            ]

    }

}





