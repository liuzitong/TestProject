import QtQuick 2.0
import QtQuick.Controls 2.0

Item{
    id:root
    property int radius:height/6;
    property alias text:textfeild.text;
    property alias verticalAlignment: textfeild.verticalAlignment;
    property alias horizontalAlignment: textfeild.horizontalAlignment;
    property alias readOnly: textfeild.readOnly;
    property alias backgroundColor: recbackground.color;
    property alias textfeild: textfeild;
    width: parent.width;
    height:parent.height;
    TextField{
        id:textfeild;
        anchors.fill: parent;
        renderType: Text.NativeRendering
        horizontalAlignment: Text.right
        verticalAlignment: Text.AlignVCenter
        font.pointSize: parent.height/3
        font.family:"Microsoft YaHei"
        selectByMouse: true
        selectionColor: "#999999"
        background: Rectangle {id:recbackground;border.width: 1;radius: root.radius;color: "white" ;border.color: "#bdc0c6"}
    }

}



