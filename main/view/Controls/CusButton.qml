import QtQuick 2.0

Item{
    id:root
    property string buttonColor: "#dcdee0"
    property string hoverBorderColor: "black"
    property string pressBorderColor: "#006486"
    property string commonBorderColor: "#bdc0c6"
    property string borderColor:commonBorderColor
    property bool hoverEnabled: true
    property string text
    property int fontSize:height/3
    property int radius:height/6
    property int borderWidth:1
    property string hoverColor: "white"
    property string imageSrc:""
    property string hoverImageSrc:""
    property string pressImageSrc: ""
    property double imageHightScale: 0.6
    property bool isAnime: true
    property int animationDuration: 150
    height:parent.height;
    width:parent.height*3;
    signal clicked()
    signal entered()
    signal exited()

    Image {
        id: image
        source: imageSrc
        anchors.horizontalCenter: parent.horizontalCenter
        height: root.height*root.imageHightScale
        width: sourceSize.width*(height/sourceSize.height)
        anchors.verticalCenter: parent.verticalCenter
        z:1;
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

    }

    Rectangle{
        id:rec
        width:parent.width
        height:parent.height
        color: root.buttonColor
        radius: root.radius
        border.color: root.borderColor
        border.width: root.borderWidth
        z:-1

        MouseArea{
            anchors.fill: parent
            hoverEnabled: root.hoverEnabled
            onClicked:
            {
                if(pressImageSrc!="") image.source=pressImageSrc;
                if(isAnime) anime.start();
                else rec.clicked();
            }
            onEntered: {if(root.hoverEnabled) {rec.color=root.hoverColor;if(hoverImageSrc!="") image.source=hoverImageSrc;root.entered();}}
            onExited:{if(root.hoverEnabled) {rec.color=root.buttonColor;if(hoverImageSrc!="") image.source=imageSrc;root.exited();}}
        }




        PropertyAnimation
        {
            id:anime
            target: rec
            properties: "opacity";
            to: 0.5
            duration: animationDuration
            onStopped: {
                rec.opacity=1.0;
                root.clicked();
            }
        }

    }

}


/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}
}
##^##*/
