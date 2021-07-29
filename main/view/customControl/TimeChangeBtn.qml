//中间的时间按钮
import QtQuick 2.0
Rectangle{
    property int curTime: 0;//当前时间
    property int maxTime: 0;//最大时间
    property int minTime: 0;//最小时间
    property alias name: name.text

    width: 60
    height: name.height+add_btn.height+time_text.height+sub_btn.height;
       Column{
           anchors.fill: parent;
           Text {
               id: name
               width: parent.width
               height: 30
               text: qsTr("text")
               font.pointSize: 12
               verticalAlignment: Text.AlignVCenter
               horizontalAlignment: Text.AlignHCenter
               z:parent.z
           }

           Rectangle{
               id: add_btn;
               width: parent.width;
               height: 30;
               color:"#b9f0fb";
               border.width: 1;
               border.color: "#8be3f5";
               z:parent.z
               Text{
                   anchors.centerIn: parent;
                   text: "+";
                   font.bold: true;
                   color:"#3ccfed";
                   font.pixelSize: 20;
                   z:parent.z
               }
               //+按钮
               MouseArea{
                   id:add_mouse;
                   anchors.fill: parent;
                   onClicked: {
                       if(curTime < maxTime)
                            ++curTime;
                   }
               }

           }

           Rectangle{
               id:time_text;
               width: parent.width;
               height: 30;
               z:parent.z
               Text{
                   anchors.centerIn: parent;
                   text: curTime;
                   fontSizeMode: Text.Fit
                   color:"#999797";
                   font.pixelSize: 18;
                   z:parent.z
               }

               MouseArea{
                   anchors.fill: parent
                   onWheel: {
                       if(wheel.angleDelta.y>0&&curTime<maxTime)
                           curTime++
                       else if(wheel.angleDelta.y<0&&curTime>minTime)
                           curTime--
                   }
               }


           }
           //-按钮
           Rectangle{
               id:sub_btn;
               width: parent.width;
               height: 30;
               color:"#b9f0fb";
               border.width: 1;
               border.color: "#8be3f5";
               z:parent.z
               Text{
                   anchors.centerIn: parent;
                   text: "-";
                   font.bold: true;
                   color:"#3ccfed";
                   font.pixelSize: 20;
                   z:parent.z
               }
               MouseArea{
                   id:sub_mouse;
                   anchors.fill: parent;
                   onClicked: {
                       if(curTime > minTime)
                            --curTime;
                   }
               }
           }
       }

       //btn的点击效果
       states: [
           State {
               name:"btn_add_pressed"
               when: add_mouse.pressed;
               PropertyChanges {
                   target:add_btn;
                   color : Qt.darker("#09c3e9",1.2);
               }
           },
           State {
               name:"btn_sub_pressed"
               when: sub_mouse.pressed;
               PropertyChanges {
                   target:sub_btn;
                   color : Qt.darker("#09c3e9",1.2);
               }
           }

       ]



}
