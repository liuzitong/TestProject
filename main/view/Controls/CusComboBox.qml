import QtQuick 2.6
import QtQuick.Controls 2.2

ComboBox {
    id: control
    model: ["First", "Second", "Third"]
    property string borderColor:"black"
    property string borderColorPressed:"Brown"
    property string imageSrc

    delegate: ItemDelegate {
        width: control.width
        height: control.height
        contentItem: Text {
            text: modelData
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex == index
    }

    indicator:
        Image {
            id: indicatorImage
            width: control.height*0.6
            height: control.height/3
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: imageSrc
            anchors.rightMargin: 10
        }

    contentItem: Text {
        leftPadding: control.height*0.5
        rightPadding: control.indicator.width + control.spacing
        text: control.displayText
        font: control.font
//        color: control.pressed ? "#17a81a" : "#21be2b"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        border.color:/* control.pressed ? "grey" :*/ control.borderColor
        border.width: control.visualFocus ? 2 : 1
        radius: height/6
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: listview.contentHeight
        padding: 1

        contentItem: ListView {
            id: listview
            clip: true
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "grey"
            radius:control.height/6
        }
    }
}





/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
