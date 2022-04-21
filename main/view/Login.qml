import QtQuick 2.0

Rectangle {
    signal changePage(var name);
    id:root;
    anchors.fill: parent;
    color: "blue";
    MouseArea{
        anchors.fill: parent;
        onClicked: {changePage("main")}
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
