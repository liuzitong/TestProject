import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0

Window {
    id: window;visible: true;width: 1366;height: 768;title: qsTr("Hello World");
    minimumWidth: 960;minimumHeight: 640;property string currentpage: "Login";
    property string language: IcUiQmlApi.appCtrl.language

    Loader {
        id: pageLoader
        anchors.fill: parent;
        source: "qrc:/perimeter/main/view/main.qml";
        function changePage(pageName)
        {
//            console.log(pageName);
            switch(pageName)
            {
                case "main": source="qrc:/perimeter/main/view/main.qml";break;
                case "login": source="qrc:/perimeter/main/view/login.qml";break;
            }
        }
    }

//    Component.onCompleted:
//    {
//        console.log(IcUiQmlApi.appCtrl.language);
//    }
}
