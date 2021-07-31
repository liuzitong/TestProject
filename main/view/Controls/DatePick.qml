import QtQuick 2.9
//import QtQuick.Window 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
//import QtQuick.Controls 2.3 //不能和QtQuick.Extras一起用
import QtQuick.Layouts 1.3
import qxpack.indcom.ui_qml_base 1.0     // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0  // [HINT] ModalPopupDialog is in it

Item {
    visible: true
    width: 640
    height: 480
//    title: qsTr("Tumbler")
    //color: "black"
    property var list: null;
    property int a: 3;
    property int b: 4;
    property int c;
    MouseArea {
        anchors.fill: parent;
        onClicked:
        {
            c=a+b;
            console.log(c);
            console.log("list[1].currentValue");
//            console.log(list[1].currentValue);
        }
    }

    property var years: [2015,2016,2017,2018,2019,2020]
    property var mouths: [1,2,3,4,5,6,7,8,9,10,11,12]
    property var days: [1,2,3,4,5,6,7,8,9,10,11,12]

    Tumbler {
        id : thumberp;anchors.centerIn: parent;
        TumblerColumn {id:yearColumn;model: years;width: 100;}
//        TumblerColumn {id:monthColumn;model: mouths;width: 100; }
//        TumblerColumn {id:dayColumn;model: days; width: 100;}

        style: TumblerStyle{id : styleData;
            //外面的边框
            frame : Rectangle{
               // implicitWidth : 2
                color: "green"
                opacity: 0.6
            }
            //背景色设置

            delegate : Rectangle{
                width: 120
                height: thumberp.height/3
                color:  "black"
                Text {
                    anchors.centerIn: parent
                    text: styleData.value
                    opacity: 0.4 + Math.max(0, 1 - Math.abs(styleData.displacement)) * 0.6
                    color: "white"
                    font.pointSize: 15
                }
            }

        }
    }

    Component.onCompleted:
    {
        idPriv.init();
    }
    Component.onDestruction: { idPriv.deinit(); }

    QtObject {
           id: idPriv;
           property var calendar_vm: null;

           // ==================================================================
           // initailize the page
           // ==================================================================
           function init( )
           {
               calendar_vm       = IcUiQmlApi.appCtrl.objMgr.attachObj( "Perimeter::CalendarVm", true );
               list=calendar_vm.yearListModel;
//               yearColumn.model=calendar_vm.yearListModel;
//               idYearList.model  = calendar_vm.yearListModel;
//               idMonthList.model = calendar_vm.monthListModel;
//               idDayList.model   = calendar_vm.dayListModel;
//               idYearList.movementEnded.connect( yearChanged );
//               idMonthList.movementEnded.connect( monthChanged );
//               idDayList.movementEnded.connect( dayChanged );

           }

           // ====================================================================
           // de-init the page
           // ====================================================================
           function deinit( )
           {
//               idYearList.model  = null;
//               idMonthList.model = null;
//               idDayList.model   = null;
//               idYearList.movementEnded.disconnect( yearChanged );
//               idMonthList.movementEnded.disconnect( monthChanged );
//               idDayList.movementEnded.disconnect( dayChanged );
               IcUiQmlApi.appCtrl.objMgr.detachObj( "Perimeter::CalendarVm", calendar_vm );
           }

           function yearChanged( )
           {
               idPriv.calendar_vm.yearListModel.currentValue = idYearList.currentItem.text;
           }

           function monthChanged( )
           {
               idPriv.calendar_vm.monthListModel.currentValue = idMonthList.currentItem.text;
           }

           function dayChanged( )
           {
               idPriv.calendar_vm.dayListModel.currentValue = idDayList.currentItem.text;
           }
       }
}
