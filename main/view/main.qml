import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0


Rectangle {
    id: window;visible: true;width: 1366;height: 768;/*title: qsTr("Hello World");*/
   /* minimumWidth: 960;minimumHeight: 640;*/color: backGroudColor;
    property int commonRadius:height*0.03
    property string backGroudColor:"#dcdee0"
    property string backGroudBorderColor:"#bdc0c6"
    property string version: "Chinese"/*"English"*/


    Column {
        id: column;anchors.fill: parent
        Rectangle{id:topRibbon;width: parent.width;height: parent.height*0.1;color: "#333e44";}

        Loader {
            id: pageLoader
            width:parent.width;height: parent.height*0.84;
            source: "qrc:/perimeter/main/view/patientManagement.qml";
            function changePage(pageName)
            {
                switch(pageName)
                {
                    case "patientManagement": source="qrc:/perimeter/main/view/patientManagement.qml";break;
                }
            }
        }
        Rectangle{
            id:bottomRibbon;width: parent.width;height: parent.height*0.06;color: "#333e44";
        }
    }
}








