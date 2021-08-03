import QtQuick 2.0

Rectangle{
    id:root
    width: parent.height*3
    height:parent.height
    property string buttonColor: "#dcdee0"
    property string hoverBorderColor: "black"
    property string pressBorderColor: "#006486"
    property string borderColor:"#bdc0c6"
    color: buttonColor
    radius: height/10
    border.color: borderColor
    border.width: 1
    signal clicked()
    signal entered()
    signal exited()
    property string text
    property int fontSize:height/3
    property bool hoverEnabled: true
    property string hoverColor: "white"
    property string imageSrc:""
    property string hoverImageSrc:""
    property string pressImageSrc: ""
    property double imageHightScale: 0.6
    property bool isAnime: true
    property int animationDuration: 150


    MouseArea{
        anchors.fill: parent
        hoverEnabled: root.hoverEnabled
        onClicked:
        {
            if(pressImageSrc!="") image.source=pressImageSrc;
            if(isAnime) anime.start();
            else root.clicked();
        }
        onEntered: {if(hoverEnabled) {root.color=root.hoverColor;if(hoverImageSrc!="") image.source=hoverImageSrc;root.entered();}}
        onExited:{if(hoverEnabled) {root.color=root.buttonColor;if(hoverImageSrc!="") image.source=imageSrc;root.exited();}}
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
            id: image
            source: imageSrc
            anchors.horizontalCenter: parent.horizontalCenter
            height: root.height*root.imageHightScale
            width: sourceSize.width*(height/sourceSize.height)
            anchors.verticalCenter: parent.verticalCenter
        }
    }


    PropertyAnimation
    {
        id:anime
        target: root
        properties: "opacity";
        to: 0.5
        duration: animationDuration
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
