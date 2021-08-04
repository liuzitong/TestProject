import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0


Rectangle {
    id: window;visible: true;width: 1366;height: 768;color: backGroudColor;
    property int commonRadius:height*0.03
    property string backGroudColor:"#dcdee0"
    property string backGroudBorderColor:"#bdc0c6"
    property string version: "Chinese"/*"English"*/
    Column {
        id: column;anchors.fill: parent
        Rectangle{
            id:topRibbon;width: parent.width;height: parent.height*0.1;color: "#333e44";
            CusButton{
                imageHightScale: 1;height:image.sourceSize.height;width:image.sourceSize.width; anchors.right: parent.right;  anchors.top: parent.top; rec.visible: false;imageSrc: "qrc:/Pics/base-svg/window_4close_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/window_4close_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/window_4close_3press.svg";
            }
            Item{
                anchors.fill: parent;anchors.margins:parent.height*0.07;
                Column{
                    anchors.fill:parent;spacing: height*0.1
                    Item{
                        width: parent.width;height: parent.height*0.30;
                        Flow{
                            height:parent.height;spacing: height*0.2
                            Image {width: height;height: parent.height;source: "qrc:/Pics/user-svg/0syseye_logo.svg";}
                            CusText{text:"电脑自动视野仪系统"; horizontalAlignment: Text.AlignLeft;color: "white";font.pointSize: height*0.5;width: height*10;}
                        }
                        Flow{
                            height:parent.height*1.2; anchors.right: parent.right; layoutDirection: Qt.RightToLeft;spacing: height*0.2
                        }
                    }
                    Item{
                        id: item1
                        width: parent.width;height: parent.height*0.60;
                        CusButton{type:"click";isAnime: false;underImageText.text: "登录";underImageText.color: "white"; fontSize: height/4;rec.visible: false;width:image.sourceSize.width;imageSrc: "qrc:/Pics/base-svg/menu_login.svg";pressImageSrc: "qrc:/Pics/base-svg/menu_login_select.svg";}
                        Flow{
                            id:contentSwitcher
                            property string contentType: "patientManagement";
                            height: parent.height;
                            anchors.horizontalCenter: parent.horizontalCenter
                            Rectangle{ id:seperator1;height: parent.height; gradient: Gradient {GradientStop {position: 0;color: "#333e44"}GradientStop { position: 0.5;color: "#7e8588"}GradientStop {position: 1;color: "#333e44"; }} width: 2;}
                            CusButton{id:patientContentButton;isAnime: false;image.source:pressImageSrc;underImageText.text: "患者";underImageText.color: "white"; fontSize: height/4;rec.visible: false;width:image.sourceSize.width*2;imageSrc: "qrc:/Pics/base-svg/menu_patient.svg";pressImageSrc: "qrc:/Pics/base-svg/menu_patient_select.svg";onClicked: {contentSwitcher.switchContent("patientManagement")}}
                            Rectangle{ id:seperator2;height: parent.height; gradient: Gradient {GradientStop {position: 0;color: "#333e44"}GradientStop { position: 0.5;color: "#7e8588"}GradientStop {position: 1;color: "#333e44"; }} width: 2;}
                            CusButton{id:checkContentButton;isAnime: false;underImageText.text: "检测";underImageText.color: "white"; fontSize: height/4;rec.visible: false;width:image.sourceSize.width*2;imageSrc: "qrc:/Pics/base-svg/menu_capture.svg";pressImageSrc: "qrc:/Pics/base-svg/menu_capture_select.svg";onClicked: {contentSwitcher.switchContent("check")}}
                            Rectangle{ id:seperator3;opacity: 0;height: parent.height; gradient: Gradient {GradientStop {position: 0;color: "#333e44"}GradientStop { position: 0.5;color: "#7e8588"}GradientStop {position: 1;color: "#333e44"; }} width: 2;}
                            function switchContent(contentType)
                            {
                                if(contentType==="patientManagement")
                                {
                                    contentSwitcher.contentType="patientManagement";
                                    checkContentButton.image.source=checkContentButton.imageSrc;
                                    seperator1.opacity=1;
                                    seperator3.opacity=0;
                                }
                                if(contentType==="check")
                                {
                                    contentSwitcher.contentType="check";
                                    patientContentButton.image.source=patientContentButton.imageSrc;
                                    seperator1.opacity=0;
                                    seperator3.opacity=1;
                                }

                            }
                        }
                        Flow{
                            height: parent.height;anchors.right: parent.right;spacing: height*1
                            CusButton{type:"click";isAnime: false;underImageText.text: "设置";underImageText.color: "white"; fontSize: height/4;rec.visible: false;width:image.sourceSize.width;imageSrc: "qrc:/Pics/base-svg/menu_set.svg";pressImageSrc: "qrc:/Pics/base-svg/menu_set_select.svg";}
                            CusButton{type:"click";isAnime: false;underImageText.text: "自定义";underImageText.color: "white"; fontSize: height/4;rec.visible: false;width:image.sourceSize.width;imageSrc: "qrc:/Pics/base-svg/menu_customize.svg";pressImageSrc: "qrc:/Pics/base-svg/menu_customize_select.svg";}
                            CusButton{type:"click";isAnime: false;underImageText.text: "关于";underImageText.color: "white"; fontSize: height/4;rec.visible: false;width:image.sourceSize.width;imageSrc: "qrc:/Pics/base-svg/menu_about.svg";pressImageSrc: "qrc:/Pics/base-svg/menu_about_select.svg";}
                        }
                    }
                }
            }
        }

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
            Row{
                anchors.fill: parent;anchors.margins:parent.height*0.15;
                Item{
                    id: item2
                    height: parent.height;width:parent.width*0.6;
                    CusButton{text:"关闭"}
                    Flow{
                        height: parent.height;spacing: height*0.8
                        anchors.horizontalCenter: parent.horizontalCenter
                        CusButton{text:"复查"}
                        CusButton{text:"修改"}
                        CusButton{text:"删除"}
                        CusButton{text:"分析"}
                    }
                }
                Flow{
                    height:parent.height; layoutDirection: Qt.RightToLeft;width:parent.width*0.4;spacing: height*0.8;
                    CusButton{text:"进入检测"}
                    CusButton{text:"保存"}
                    CusButton{text:"新建"}
                }
            }
        }
    }
}








