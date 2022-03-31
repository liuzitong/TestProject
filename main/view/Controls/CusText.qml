import QtQuick 2.0
import qxpack.indcom.ui_qml_base 1.0

Text {
    height: parent.height
    text: qsTr("中文")
    width: /*IcUiQmlApi.appCtrl.language==="Chinese"?*/(text.length*0.4+0.4)*height/*:(text.length*0.2+0.4)*height*/;
    font.pointSize: height*0.28
    fontSizeMode: Text.Fit
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.family:"Microsoft YaHei"
}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.25}
}
##^##*/
