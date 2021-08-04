import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0


Window {
    id: window;visible: true;width: 1366;height: 768;title: qsTr("Hello World");
    minimumWidth: 960;minimumHeight: 640;color: backGroudColor;
    property int commonRadius:height*0.03
    property string backGroudColor:"#dcdee0"
    property string backGroudBorderColor:"#bdc0c6"
    property string version: "Chinese"/*"English"*/


    Column {
        id: column;anchors.fill: parent
        Rectangle{id:topRibbon;width: parent.width;height: parent.height*0.1;color: "#333e44";}
        Item{
            id:content;width:parent.width;height: parent.height*0.84;
            property int layoutGroupMargin: 0.03*height
            CusCalendar{id: calendar;width: parent.width;height: parent.height;}
            Row{
                anchors.fill: parent;anchors.margins: content.layoutGroupMargin;spacing:content.layoutGroupMargin
                Rectangle{
                    width: (parent.width-content.layoutGroupMargin)*0.6;height: parent.height;
                    border.color: backGroudBorderColor;color: backGroudColor;radius: commonRadius
                    Item{
                        anchors.fill: parent;anchors.margins: content.layoutGroupMargin
                        Column{
                            id:patientManagement;anchors.fill: parent;spacing: rowHight
                            property int rowHight: height*0.05
                            Row{
                                id:dateSelection; width: parent.width;
                                height: patientManagement.rowHight;spacing: height*0.4;
                                CusText{text: "检查日期";horizontalAlignment: Text.AlignLeft;width: height*2.5}
                                LineEdit{id:dateFrom;radius: height/6;width: height*3.2;}
                                CusButton{id:dateFromButton;text:"选择";width:height*2;onClicked:{calendar.inputObj=dateFrom;calendar.open();}}
                                CusText{text:"到";width: height*0.6;}
                                LineEdit{id:dateTo;radius: height/6;width: height*3.2}
                                CusButton{id:dateToButton;text:"选择";width:height*2;onClicked:{calendar.inputObj=dateTo;calendar.open();}}
                            }
                            Row{
                                id:query; width: parent.width; height:patientManagement.rowHight;spacing: height*0.5
                                CusComboBox{
                                    id:queryStrategy;height: parent.height;width: parent.height*5;
                                    borderColor: backGroudBorderColor;font.family:"Microsoft YaHei";
                                    imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                                    model: ListModel {ListElement { text: "最近诊断" } ListElement { text: "患者ID" } ListElement { text: "姓名" }ListElement { text: "性别" }ListElement { text: "出生日期" }}
                                    onCurrentIndexChanged: {
                                        if(currentIndex==0){patientID.visible=false;chineseName.visible=false;englishNameGroup.visible=false;gender.visible=false;birthDateGroup.visible=false;}
                                        if(currentIndex==1){patientID.visible=true;chineseName.visible=false;englishNameGroup.visible=false;gender.visible=false;birthDateGroup.visible=false;}
                                        if(currentIndex==2)
                                        {
                                            if(version=="Chinese"){ patientID.visible=false;chineseName.visible=true;englishNameGroup.visible=false;gender.visible=false;birthDateGroup.visible=false;}
                                            else{patientID.visible=false;chineseName.visible=false;englishNameGroup.visible=true;gender.visible=false;birthDateGroup.visible=false;}
                                        }
                                        if(currentIndex==3){patientID.visible=false;chineseName.visible=false;englishNameGroup.visible=false;gender.visible=true;birthDateGroup.visible=false;}
                                        if(currentIndex==4){patientID.visible=false;chineseName.visible=false;englishNameGroup.visible=false;gender.visible=false;birthDateGroup.visible=true;}
                                    }
                                }
                                LineEdit{id:patientID;radius:height/6;width: height*4}
                                LineEdit{id:chineseName;radius:height/6;width: height*4}
                                Flow{
                                    id:englishNameGroup;height: parent.height;spacing: height*0.4;
                                    CusText{text:"First Name:";width:height*2.5}
                                    LineEdit{id:firstName;radius:height/6;width: height*4}
                                    CusText{text:"Last Name:";width:height*2.5}
                                    LineEdit{id:lastName;radius:height/6;width: height*4}
                                }
                                CusComboBox{
                                    id:gender;height: parent.height;width: parent.height*3;
                                    borderColor: backGroudBorderColor;font.family:"Microsoft YaHei";
                                    imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                                    model: ListModel {ListElement { text: "男" } ListElement { text: "女" } }
                                }
                                Flow{
                                    id:birthDateGroup;visible: false;height: parent.height;spacing: height*0.4;
                                    LineEdit{id:birthDate;height: parent.height;radius:height/6;width: height*4;visible: true;}
                                    CusButton{text:"选择";width:height*2;onClicked:{calendar.inputObj=birthDate;calendar.open();}}
                                }
                                CusButton{height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"}}
                            Item{
                                width: parent.width;height: parent.height-6*patientManagement.rowHight
                                Column{
                                    id:patientInfoCol;
                                    anchors.fill:parent;spacing: -1;
                                    Row{
                                        id:header
                                        width: parent.width; height:patientManagement.rowHight;spacing: -1;z:1;
                                        property bool isAllSelected: false;
                                        CusButton{
                                            width: parent.width*1/9;height: parent.height;buttonColor: "#D2D3D5"; borderColor: "#656566";radius:0;imageSrc:"qrc:/Pics/base-svg/btn_select_normal.svg"
                                            onClicked: {
                                                if(!header.isAllSelected)
                                                {
                                                    header.isAllSelected=true;imageSrc="qrc:/Pics/base-svg/btn_select_click.svg";
                                                }
                                                else
                                                {
                                                    header.isAllSelected=false;imageSrc="qrc:/Pics/base-svg/btn_select_normal.svg";
                                                }
                                            }
                                        }
                                        Rectangle{width: parent.width*2/9+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"患者ID"}}
                                        Rectangle{width: parent.width*2/9+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"姓名"}}
                                        Rectangle{width: parent.width*1/9+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"性别"}}
                                        Rectangle{width: parent.width*2/9+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"出生日期"}}
                                        Rectangle{width: parent.width*1/9+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"载入"}}
                                    }
                                    ListView{
                                        id:patientInfoListView
                                        width: parent.width;height:patientInfoCol.height-patientManagement.rowHight +1; /*interactive: false;*/model:contactModel;delegate: patientInfoDelegate;spacing: -1;clip:true;snapMode: ListView.SnapPosition;
                                        //List Element会导致界面预览无法显示
                                        ListModel {
                                            id:contactModel
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
//                                            ListElement {patientId: "Bill Smith";name: "555 3264";gender:"男性";birthDate:"1988-5-11";}
                                        }

                                        Component{
                                            id:patientInfoDelegate
                                            Row{
                                                id:patientInfoRow
                                                width: patientInfoListView.width;height: (patientInfoCol.height-patientManagement.rowHight)/14+1;spacing: -1;
                                                property bool isSelected: false;
                                                CusButton{
                                                    width: parent.width*1/9;height: parent.height;buttonColor: "white"; borderColor: backGroudBorderColor;radius:0;isAnime:false;imageSrc:"qrc:/Pics/base-svg/btn_select_normal.svg"
                                                    onClicked: {
                                                        if(!patientInfoRow.isSelected)
                                                        {
                                                            patientInfoRow.isSelected=true;imageSrc="qrc:/Pics/base-svg/btn_select_click.svg";
                                                        }
                                                        else
                                                        {
                                                            patientInfoRow.isSelected=false;imageSrc="qrc:/Pics/base-svg/btn_select_normal.svg";
                                                        }
                                                    }
                                                }
                                                Rectangle{width: parent.width*2/9+1;height: parent.height;color: "white"; border.color: backGroudBorderColor;CusText{anchors.fill: parent;text:model.patientId}}
                                                Rectangle{width: parent.width*2/9+1;height: parent.height;color: "white"; border.color: backGroudBorderColor;CusText{anchors.fill: parent;text:model.name}}
                                                Rectangle{width: parent.width*1/9+1;height: parent.height;color: "white"; border.color: backGroudBorderColor;CusText{anchors.fill: parent;text:model.gender}}
                                                Rectangle{width: parent.width*2/9+1;height: parent.height;color: "white"; border.color: backGroudBorderColor;CusText{anchors.fill: parent;text:model.birthDate}}
                                                CusButton{width: parent.width*1/9+1;height: parent.height;buttonColor: "white"; radius:0;imageSrc:"qrc:/Pics/base-svg/btn_analysis_enter.svg"}
                                            }
                                        }
                                    }
                                }
                            }
                            Item{
                                id:pageControl;width: parent.width;height:patientManagement.rowHight;
                                Flow{
                                    id:pageIndex;
                                    property int currentPage: 0;property int totalPage: 10;property int totalRecordCount: 100;anchors.verticalCenter: parent.verticalCenter;anchors.left: parent.left;height:parent.height;
                                    CusText{id:currentPageNumberText;text:"第 "+pageIndex.currentPage+"/"+pageIndex.totalPage+" 页,共计 "+pageIndex.totalRecordCount+" 条纪录";width: height*6}
                                }
                                Flow{
                                    anchors.right: parent.right;height:parent.height*1.3;anchors.verticalCenter: parent.verticalCenter;spacing:0.4*height;
                                    CusButton{
                                        imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_1head_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_1head_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_1head_3press.svg";
                                    }
                                    CusButton{
                                        imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_2previous_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_2previous_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_2previous_3press.svg";
                                    }
                                    LineEdit{width: height*2;text:"1";radius: height*0.2;horizontalAlignment: Text.AlignHCenter}
                                    CusButton{width: height; text:"Goto";fontSize: height*0.25;onEntered: {borderColor=hoverBorderColor;}onExited: {borderColor=commonBorderColor;}onClicked: {borderColor=pressBorderColor;}}
                                    CusButton{
                                        imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_4next_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_4next_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_4next_3press.svg";
                                    }
                                    CusButton{
                                        imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_5end_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_5end_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_5end_3press.svg";
                                    }
                                }
                            }
                        }
                    }
                }
                Column{
                    width: (parent.width-content.layoutGroupMargin)*0.4; height: parent.height;spacing:content.layoutGroupMargin;
                    Rectangle{
                        width: parent.width;height: (parent.height-content.layoutGroupMargin)*0.6;border.color: backGroudBorderColor;color: backGroudColor;radius: commonRadius;
                        Item{
                            anchors.fill: parent;anchors.margins: content.layoutGroupMargin;
                            Column{
                                id:newPatient;anchors.fill: parent;spacing: patientManagement.rowHight*1.5
                                Component.onCompleted: {
                                    if(version==="Chinese") {newChineseName.visible=true;newEnglishName.visible=false;}
                                    else {newChineseName.visible=false;newEnglishName.visible=true;}
                                }

                                Row{
                                    width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                    CusText{text:"*患者ID "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                    LineEdit{width: parent.width*0.6}
                                    CusButton{height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"}
                                }
                                Row{
                                    id:newChineseName;width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                    CusText{text:"*姓名 "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                    LineEdit{id:name;width: parent.width*0.6}
                                    CusButton{height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"}
                                }
                                Row{
                                    id:newEnglishName;width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02;visible:false;
                                    CusText{text:"*firstName "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                    LineEdit{id:firstname;width: parent.width*0.205}
                                    CusText{text:"*lastName "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.15}
                                    LineEdit{id:last;width: parent.width*0.205}
                                    CusButton{height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"}

                                }
                                Row{
                                    width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                    CusText{text:"*性别 "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                    Row{
                                        id:genderSelect;property string gender;
                                        height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                        CusButton{
                                            id:manButton;property bool chosen:false;imageSrc: "qrc:/Pics/base-svg/btn_sex_man.svg";width: 2*height
                                            onClicked: {genderSelect.selectGender(manButton,this)}
                                        }
                                        CusButton{
                                            id:womanButton;property bool chosen:false;imageSrc: "qrc:/Pics/base-svg/btn_sex_woman.svg";width: 2*height
                                            onClicked:  {genderSelect.selectGender(womanButton,this)}
                                        }
                                        CusButton{id:otherButton;property bool chosen:false;text:"其它";width:height*2;
                                             onClicked: {genderSelect.selectGender(otherButton,this)}
                                        }
                                        function selectGender(id,button)
                                        {
                                            womanButton.borderColor=womanButton.commonBorderColor;
                                            manButton.borderColor=manButton.commonBorderColor;
                                            otherButton.borderColor=otherButton.commonBorderColor;
                                            button.borderColor=button.pressBorderColor;
                                            switch(id)
                                            {
                                                case manButton: genderSelect.gender="man";break;
                                                case womanButton:genderSelect.gender="women";break;
                                                case otherButton:genderSelect.gender="other";break;
                                            }
                                        }
                                    }
                                }
                                Row{
                                    width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                    CusText{text:"*出生日期 "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                    Row{
                                        height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                        Item{height: parent.height;width: 2*height;LineEdit{id:newDateBirth;width: height*3.2}}
                                        CusButton{text:"选择";width:height*2;onClicked:{calendar.inputObj=newDateBirth;calendar.open();}}
                                        LineEdit{width: height*2;text:"1";radius: height*0.2;horizontalAlignment: Text.AlignHCenter;readOnly: true;backgroundColor:backGroudColor}
                                    }
                                }
                            }
                        }
                    }
                    Rectangle{
                        width: parent.width;height: (parent.height-content.layoutGroupMargin)*0.4;
                        border.color: backGroudBorderColor;color: backGroudColor;radius: commonRadius;
                       Item{
                           anchors.fill: parent;anchors.margins: content.layoutGroupMargin;
                           Column{
                               id:eyeBallCalc
                               anchors.fill: parent;spacing: patientManagement.rowHight;
                               Row{
                                   width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                   Item{height:parent.height;width:parent.width*0.20}
                                   Row{
                                       height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                       CusText{ text:"球面";width: 2*height}
                                       CusText{ text:"柱面";width: 2*height}
                                       CusText{ text:"轴向";width: 2*height}
                                   }
                               }
                               Row{
                                   width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                   CusText{text:"右眼";width:parent.width*0.20}
                                   Row{
                                       height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                   }
                               }
                               Row{
                                   width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                   CusText{text:"左眼";width:parent.width*0.20}
                                   Row{
                                       height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                   }
                               }
                               Row{
                                   width: parent.width;height:patientManagement.rowHight;spacing: parent.width*0.02
                                   Item{height:parent.height;width:parent.width*0.20}
                                   Row{
                                       height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                       Item{ height:parent.height;width: 2*height}
                                       CusButton{text:"计算"; width: 2*height}
                                       Item{ height:parent.height;width: 2*height}
                                   }
                               }
                           }
                       }
                    }
                }
            }
        }
        Rectangle{
            id:bottomRibbon;width: parent.width;height: parent.height*0.06;color: "#333e44";
        }
    }
}








