import QtQuick 2.0

Rectangle{
    id:root
    width: parent.height*3
    height:parent.height
    property string buttonColor: "#dcdee0"
    color: buttonColor
    radius: height/10
    border.color: "#bdc0c6"
    border.width: 1
    signal clicked()
    property string text
    property int fontSize:height/3
    property bool hoverEnabled: true
    property string hoverColor: "white"
    property string imageSrc



    MouseArea{
        anchors.fill: parent
        hoverEnabled: root.hoverEnabled
        onClicked:
        {
            anime.start();
        }
        onEntered: root.color=root.hoverColor
        onExited: root.color=root.buttonColor
        //onClicked: patientManagement.parent.source="main.qml"
    }

    Item{
        id: row
        anchors.fill: parent
        Text {
            text: root.text
            font.pointSize: root.fontSize
            fontSizeMode: Text.Fit
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent
            font.family: "Microsoft YaHei"
        }

        Image {
            id: name
            source: imageSrc
            anchors.horizontalCenter: parent.horizontalCenter
            height: root.height*0.6
            width: height
            anchors.verticalCenter: parent.verticalCenter
        }
    }


    PropertyAnimation
    {
        id:anime
        target: root
        properties: "opacity";
        to: 0.5
        duration: 200
        onStopped: {
            root.opacity=1.0;
            root.clicked();
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}
}
##^##*/
