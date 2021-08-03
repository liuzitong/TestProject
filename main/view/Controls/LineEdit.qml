import QtQuick 2.0


Rectangle {
    width: 300
    height: parent.height
    color: "white"
    border.color: "#bdc0c6"
    property alias fontSize: input.font.pointSize
    property alias enabled: input.enabled
    property alias text: input.text

    TextInput {
        id:input
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        anchors.margins: 2
        font.pointSize: parent.height/3
        focus: true
        font.family:"Microsoft YaHei"
    }
}
