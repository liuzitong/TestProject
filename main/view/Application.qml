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
        id:content
        anchors.fill: parent;
    }

    Login{
        id:login
        visible: false;
        anchors.fill: parent;
    }



    Component.onCompleted: {
        idPriv.init();
        content.changePage.connect(changePage);
        login.changePage.connect(changePage);
//        var param=IcUiQmlApi.appCtrl.databaseSvc.getParams();
//        console.log(param.commonParams.Range[0]);
//        console.log(param.commonParams.Range[1]);
    }

    function changePage(pageName)
    {
        console.log(pageName);
        switch(pageName)
        {
            case "main":content.visible=true;login.visible=false;break;
            case "login":content.visible=false;login.visible=true;break;
        }
    }


//    Loader {
//        id: pageLoader
//        anchors.fill: parent;
//        source: "qrc:/perimeter/main/view/Main.qml";
//        function changePage(pageName)
//        {
//                console.log(pageName);
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
//    Component.onDestruction: { idPriv.deinit(); }
    QtObject{
        id: idPriv;
        property var patient_vm:null;
        property var patientList_vm:null;
        property var currentProgram:null;
        function init()
        {
            IcUiQmlApi.appCtrl.databaseSvc.initDataBase();
//            patient_vm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::PatientVm", true);
//            patient_vm.hello();
//            IcUiQmlApi.appCtrl.objMgr.detachObj( "Perimeter::PatientVm", patient_vm );
//            patientList_vm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::PatientListModelVm", true);
//            patientList_vm.hello();
//            IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::PatientListModelVm", patientList_vm);
//            currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false);
//            currentProgram.hello();
//            IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);
        }
    }


}
