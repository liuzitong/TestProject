pragma Singleton
import QtQuick 2.0

Item {
    property string backGroundColor:"#dcdee0"
    property string backGroundBorderColor:"#bdc0c6"
    property string ribbonColor: "#333e44"
    property string darkButtonColor:"#e0e0e0";
    property int windowHeight;

    property int textHeight: windowHeight*0.018;
    property int fontPointSize: textHeight*0.8;
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
