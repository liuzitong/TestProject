import QtQuick 2.0
import qxpack.indcom.ui_qml_base 1.0

Text {
    height: parent.height
    text: qsTr("中文")
    wrapMode: Text.NoWrap
    width: IcUiQmlApi.appCtrl.settings.language==="Chinese"?(text.length*0.9)*height:(text.length*0.4)*height;
    font.pointSize: height*0.8
    fontSizeMode: Text.Fit
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.family:"Microsoft YaHei"
    renderType: Text.NativeRendering
}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.25}
}
##^##*/
