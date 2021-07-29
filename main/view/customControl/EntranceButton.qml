import QtQuick 2.0

Item{
    id: root
    width: parent.height*4/5
    height: parent.height
    property alias title: entryName.text
    property alias imageSource: img.source
    property alias color: rec.color
    function onClicked(){}


    Rectangle{
        id:rec
        radius: 10
        anchors.fill: parent
        border.width: 3
        border.color: "white"
    }
    Item{
        id: column
        anchors.fill: parent
        Image {
            id: img
            width: parent.width*0.9
            height: parent.width*0.9
            anchors.top: parent.top
            anchors.topMargin: parent.width*0.05
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: entryName
            width: parent.width*0.9
            height: parent.height-parent.width*0.1-img.height
            text: qsTr("text")
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: parent.width*0.05
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: parent.height/16

        }
    }

    MouseArea{
        id:mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.onClicked();
        onEntered: rec.border.color="#58bef8"
        onExited: rec.border.color="white"
    }


}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
