import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0

Window {
    id: window;visible: true;width: 1366;height: 768;title: qsTr("Hello World");
    minimumWidth: 960;minimumHeight: 640;property string currentpage: "Login";

    MouseArea {
            anchors.fill: parent
            onClicked: changePage();
        }

    function changePage() {
        if(isFirst) {
            pageLoader.source = "Page1.qml"
        } else {
            pageLoader.source = "Page2.qml"
        }

        isFirst = !isFirst;
    }

}
