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
    clip: true
    signal enterPressed();
    TextField{
        id:textfeild;
        anchors.fill: parent;
        text:"";
        horizontalAlignment: Text.AlignLeft
        renderType: Text.NativeRendering
        verticalAlignment: Text.AlignVCenter
        font.pointSize: parent.height*0.30
        font.family:"Microsoft YaHei"
        selectByMouse: true
        selectionColor: "yellow"
        background: Rectangle {id:recbackground;border.width: 1;radius: root.radius;color:readOnly? "#e4e6e8":"white" ;border.color: "#bdc0c6"}
        Keys.onPressed: { switch (event.key){/*EnterKey*/ case 16777220:root.enterPressed();break;};
        }
    }
}



