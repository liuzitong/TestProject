import QtQuick 2.0

Rectangle {
    id:root;
    anchors.fill: parent;
    color: "blue";
    signal changePage(string pageName)
    MouseArea{
        anchors.fill: parent;
        onClicked: {changePage("main")}
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
