import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Controls 1.0

Window {
    id: window;visible: true;width: 1366;height: 768;title: qsTr("Hello World");
    minimumWidth: 960;minimumHeight: 640;property string currentpage: "Login";
    property string language: IcUiQmlApi.appCtrl.language

    Content{
        anchors.fill: parent;
    }

    Login{
        visible: false;
        anchors.fill: parent;
    }


//    Loader {
//        id: pageLoader
//        anchors.fill: parent;
//        source: "qrc:/perimeter/main/view/Main.qml";
//        function changePage(pageName)
//        {
////            console.log(pageName);
//            switch(pageName)
//            {
//                case "main": source="qrc:/perimeter/main/view/main.qml";break;
//                case "login": source="qrc:/perimeter/main/view/login.qml";break;
//            }
//        }
//    }

    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted:   { idPriv.init();   }
//    Component.onDestruction: { idPriv.deinit(); }
    QtObject{
        id: idPriv;
        property var patient_vm:null;
        function init()
        {

            IcUiQmlApi.appCtrl.databaseSvc.initDataBase();
//            IcUiQmlApi.appCtrl.databaseSvc.test();
//            console.log("sssss");
//            patient_vm=IcUiQmlApi.appCtrl.databaseSvc.getPatientById("5001");
//            console.log("sssss");
//            console.log(patient_vm.name);
//            console.log("sssss");
//            delete patient_vm;
        }
    }


}
