import QtQuick 2.0
import QtQuick.Controls 2.0

Item{
    id:root
    property int radius:height/6;
    property string text:"";
    width: parent.width;
    height:parent.height;
    TextField{
        anchors.fill: parent;
        renderType: Text.NativeRendering
        text:root.text;
        horizontalAlignment: Text.right
        verticalAlignment: Text.AlignVCenter
        font.pointSize: parent.height/3
        font.family:"Microsoft YaHei"
        selectByMouse: true
        selectionColor: "#999999"
        background: Rectangle {border.width: 1;radius: root.radius;color: "white" ;border.color: "#bdc0c6"}
    }

}



