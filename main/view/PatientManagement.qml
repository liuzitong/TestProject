import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0

Item{
    id:root;
    property string language:IcUiQmlApi.appCtrl.language
    property bool doubleName:IcUiQmlApi.appCtrl.doubleName
    property string backGroundColor:"#dcdee0"
    property string backGroundBorderColor:"#bdc0c6"
    property int pageSize: 15;
    signal queryStarted;
    signal changePage(var pageName);
    width: 1440
    height: 700
    anchors.fill:parent;

    Item{
        width: parent.width;
        height: parent.height*14/15
        anchors.top: parent.top
        id:content;
        property int layoutGroupMargin: 0.03*height
        CusCalendar{id: calendar;width: parent.width;height: parent.height;}
        Row{
            anchors.fill: parent;anchors.margins: content.layoutGroupMargin;spacing:content.layoutGroupMargin
            Rectangle{
                width: (parent.width-content.layoutGroupMargin)*0.6;height: parent.height;
                border.color: backGroundBorderColor;color: backGroundColor;radius: commonRadius
                Item{
                    anchors.fill: parent;anchors.margins: content.layoutGroupMargin
                    Column{
                        id:patientInfo;anchors.fill: parent;spacing: rowHight
                        property int rowHight: height*0.06

                        Row{
                            id:dateSelection; width: parent.width;opacity: 1;
                            height: patientInfo.rowHight;spacing: height*0.4;
                            CusText{text: "检查日期";horizontalAlignment: Text.AlignLeft;width: height*2.5;}
                            LineEdit{property string name: "dateFrom";id:dateFrom;radius: height/6;width: height*3.1;}
                            CusButton{id:dateFromButton;text:"选择";width:height*2;onClicked:{calendar.inputObj=dateFrom;calendar.open();}}
                            CusText{text:"到";width: height*0.6;}
                            LineEdit{property string name: "dateTo";id:dateTo;radius: height/6;width: height*3.1;}
                            CusButton{id:dateToButton;text:"选择";width:height*2;
                                onClicked:{
                                    if(dateFrom.text!==""&&dateTo.text=="")
                                        dateTo.text=dateFrom.text;
                                    calendar.inputObj=dateTo;calendar.open();
                                }}
                        }
                        Row{
                            id:query; width: parent.width; height:patientInfo.rowHight;spacing: height*0.5
                            CusComboBox{
                                id:queryStrategy;height: parent.height;width: parent.height*5;
                                borderColor: backGroundBorderColor;font.family:"Microsoft YaHei";
                                imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                                model: ListModel {ListElement { text: "按时间查询" } ListElement { text: "患者ID" } ListElement { text: "姓名" }ListElement { text: "性别" }ListElement { text: "出生日期" }}
                                onCurrentIndexChanged: {
                                    patientID.visible=false;chineseName.visible=false;englishNameGroup.visible=false;sex.visible=false;birthDateGroup.visible=false;dateSelection.opacity=0;
                                    switch(currentIndex)
                                    {
                                    case 0:dateSelection.opacity=1;break;
                                    case 1:patientID.visible=true;dateSelection.opacity=0;break;
                                    case 2:if(!doubleName) {chineseName.visible=true;} else{englishNameGroup.visible=true;} dateSelection.opacity=1;break;
                                    case 3:sex.visible=true;dateSelection.opacity=1;break;
                                    case 4:birthDateGroup.visible=true;dateSelection.opacity=0;break;
                                    }
                                }
                                Component.onCompleted: {
                                    IcUiQmlApi.appCtrl.doubleNameChanged.connect(onCurrentIndexChanged)
                                }
                            }

                            LineEdit{id:patientID;radius:height/6;width: height*4}
                            LineEdit{id:chineseName;radius:height/6;width: height*4;onEnterPressed:{query.startQuery();}}
                            Flow{
                                id:englishNameGroup;height: parent.height;spacing: height*0.4;
                                CusText{text:"名字:";width:height*2.5}
                                LineEdit{id:firstName;radius:height/6;width: height*4}
                                CusText{text:"姓氏:";width:height*2.5}
                                LineEdit{id:lastName;radius:height/6;width: height*4}
                            }
                            CusComboBox{
                                id:sex;height: parent.height;width: parent.height*3;
                                borderColor: backGroundBorderColor;font.family:"Microsoft YaHei";
                                imageSrc: "qrc:/Pics/base-svg/btn_drop_down.svg";
                                model: ListModel {ListElement { text: "男" } ListElement { text: "女" } ListElement { text: "其它" } }
                            }
                            Flow{
                                id:birthDateGroup;visible: false;height: parent.height;spacing: height*0.4;
                                LineEdit{id:birthDate;height: parent.height;radius:height/6;width: height*4;visible: true;}
                                CusButton{text:"选择";width:height*2;onClicked:{calendar.inputObj=birthDate;calendar.open();}}
                            }
                            CusButton{
                                height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"
                                onClicked:query.startQuery();
                            }
                            function startQuery()
                            {
//                                console.log(patientID.text);
                                switch (queryStrategy.currentIndex)
                                {
                                case 0:patientInfoListView.patientListModelVm.getPatientListByTimeSpan(dateFrom.text,dateTo.text);break;
                                case 1:
                                    patientInfoListView.patientListModelVm.getPatientListByPatientId(patientID.text);break;
                                case 2:
                                    console.log(chineseName.text);
                                    if( !IcUiQmlApi.appCtrl.doubleName) patientInfoListView.patientListModelVm.getPatientListByName(chineseName.text,dateFrom.text,dateTo.text);
                                    else  patientInfoListView.patientListModelVm.getPatientListByName(firstName.text+" "+lastName.text,dateFrom.text,dateTo.text);
                                    break;
                                case 3:patientInfoListView.patientListModelVm.getPatientListBySex(sex.currentIndex,dateFrom.text,dateTo.text);break;
                                case 4:patientInfoListView.patientListModelVm.getPatientListByBirthDate(birthDate.text);
                                }
//                                queryStarted();
                            }
                        }
                        Item{
                            width: parent.width;height: parent.height-6*patientInfo.rowHight
                            Column{
                                id:patientInfoCol;
                                anchors.fill:parent;spacing: -1;
                                Row{
                                    id:header
                                    width: parent.width; height:patientInfo.rowHight;spacing: -1;z:1;
                                    property bool isAllSelected: false;
                                    CusButton{
                                        width: parent.width*1/10;height: parent.height;buttonColor: "#D2D3D5"; borderColor: "#656566";radius:0;imageSrc:"qrc:/Pics/base-svg/btn_select_normal.svg"
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
                                    Rectangle{width: parent.width*2/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"患者ID"}}
                                    Rectangle{width: parent.width*3/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"姓名"}}
                                    Rectangle{width: parent.width*1/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"性别"}}
                                    Rectangle{width: parent.width*2/10+1;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"出生日期"}}
                                    Rectangle{width: parent.width*1/10;height: parent.height;color: "#D2D3D5"; border.color: "#656566";CusText{anchors.fill: parent;text:"载入"}}
                                }
                                ListView{
                                    id:patientInfoListView
                                    property var seletedPatient:[];
                                    property var patientListModelVm:null;
                                    width: parent.width;height:patientInfoCol.height-patientInfo.rowHight +1; interactive: false; spacing: -1;clip:true;snapMode: ListView.SnapPosition;/*interactive: false;*/
                                    delegate: patientInfoDelegate
                                    model:patientListModelVm;
                                    Component.onCompleted: {
                                        patientListModelVm=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::PatientListModelVm", true);
//                                        patientListModelVm.getPatientListByTimeSpan(dateFrom.text,dateTo.text);
                                        patientListModelVm.hello();
                                    }
                                    Component.onDestruction: {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::PatientListModelVm",patientListModelVm);}
                                    Component{
                                        id:patientInfoDelegate
                                        Item{
                                            width: patientInfoListView.width;height: (patientInfoCol.height-patientInfo.rowHight)/pageSize+1;
                                            MouseArea{
                                                anchors.fill: parent;
//                                                onClicked: {console.log("Ted Say hi id="+model.Id)}
                                            }
                                            Row{
                                                id:patientInfoRow;anchors.fill: parent;spacing: -1;
                                                property bool isSelected: false;
                                                CusButton{
                                                    width: parent.width*1/10;height: parent.height;buttonColor: "white"; borderColor: backGroundBorderColor;radius:0;isAnime:false;imageSrc:"qrc:/Pics/base-svg/btn_select_normal.svg"
                                                    onClicked:
                                                    {
                                                        if(!patientInfoRow.isSelected){patientInfoRow.isSelected=true;imageSrc="qrc:/Pics/base-svg/btn_select_click.svg";patientInfoListView.seletedPatient.push(model.Id);}
                                                        else{patientInfoRow.isSelected=false;imageSrc="qrc:/Pics/base-svg/btn_select_normal.svg";patientInfoListView.seletedPatient.pop(model.Id);}
                                                    }
                                                }
                                                Rectangle{width: parent.width*2/10+1;height: parent.height;color: "white"; border.color: backGroundBorderColor;CusText{anchors.fill: parent;text:model.patientId}}
                                                Rectangle{width: parent.width*3/10+1;height: parent.height;color: "white"; border.color: backGroundBorderColor;CusText{anchors.fill: parent;text:model.name+" "+model.Id+" "+model.lastUpdate}}
                                                Rectangle{width: parent.width*1/10+1;height: parent.height;color: "white"; border.color: backGroundBorderColor;CusText{anchors.fill: parent;text:model.sex}}
                                                Rectangle{width: parent.width*2/10+1;height: parent.height;color: "white"; border.color: backGroundBorderColor;CusText{anchors.fill: parent;text:model.birthDate}}
                                                CusButton
                                                {
                                                    width: parent.width*1/10;height: parent.height;buttonColor: "white"; radius:0;imageSrc:"qrc:/Pics/base-svg/btn_analysis_enter.svg"
                                                    onClicked:
                                                    {
                                                        var name=model.name;var firstName ="";var lastName="";
                                                        if(model.name.indexOf(" ")>-1){ firstName =model.name.split(" ")[0]; lastName=model.name.split(" ")[1];};
                                                        newId.text=model.patientId;newChineseName.text=model.name;genderSelect.selectGender(model.sex);newDateBirth.text=model.birthDate;newEnglishFirstName.text=firstName;newEnglishLastName.text=lastName;
                                                        IcUiQmlApi.appCtrl.currentPatient.id=model.Id;
                                                        IcUiQmlApi.appCtrl.currentPatient.patientId=model.patientId;
                                                        IcUiQmlApi.appCtrl.currentPatient.name=model.name;
                                                        IcUiQmlApi.appCtrl.currentPatient.sex=model.sex;
                                                        IcUiQmlApi.appCtrl.currentPatient.birthDate=model.birthDate;
                                                        IcUiQmlApi.appCtrl.currentPatient.lastUpdate=model.lastUpdate;
                                                        console.log("current patient info:"+IcUiQmlApi.appCtrl.currentPatient.id+" "+IcUiQmlApi.appCtrl.currentPatient.patientId+" "+IcUiQmlApi.appCtrl.currentPatient.name+" "+IcUiQmlApi.appCtrl.currentPatient.sex+" "+IcUiQmlApi.appCtrl.currentPatient.birthDate+" "+IcUiQmlApi.appCtrl.currentPatient.lastUpdate)
                                                        patientSaveButton.enabled=false;patientSaveButton.buttonColor = "#787878"
                                                        patientReviseButton.enabled=true;patientReviseButton.buttonColor="#dcdee0"
                                                       /* patient.id=model.Id;*//*patient.patientId=model.patientId;patient.sex=model.sex;patient.birthDate=model.birtDate;*/

                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Item{
                            id:pageControl;width: parent.width;height:patientInfo.rowHight;
                            Flow{
                                id:pageIndex;
                                property int currentPage: 1;property int totalPage:1;property int totalRecordCount: 0;anchors.verticalCenter: parent.verticalCenter;anchors.left: parent.left;height:parent.height;
                                CusText{id:currentPageNumberText;text:"第 "+pageIndex.currentPage+"/"+pageIndex.totalPage+" 页,共计 "+pageIndex.totalRecordCount+" 条纪录";width: height*6}
                            }
                            Flow{
                                anchors.right: parent.right;height:parent.height*1.3;anchors.verticalCenter: parent.verticalCenter;spacing:0.4*height;
                                CusButton{
                                    id:startPage;
                                    imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_1head_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_1head_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_1head_3press.svg";
                                    onClicked: {
                                        pageIndex.currentPage=1;
                                        patientInfoListView.positionViewAtIndex(0,ListView.Beginning);
                                    }
                                }
                                CusButton{
                                    id:previousPage;
                                    imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_2previous_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_2previous_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_2previous_3press.svg";
                                    onClicked: {
                                        if(pageIndex.currentPage>1)
                                        {
                                            pageIndex.currentPage-=1;
                                            patientInfoListView.positionViewAtIndex((pageIndex.currentPage-1)*pageSize,ListView.Beginning);
                                        }
                                    }
                                }
                                LineEdit{id:currentPage;width: height*2;text:pageIndex.currentPage;radius: height*0.2;horizontalAlignment: Text.AlignHCenter}
                                CusButton{
                                    width: height; text:"Goto";fontSize: height*0.25;onEntered: {borderColor=hoverBorderColor;}onExited: {borderColor=commonBorderColor;}
                                    onClicked:
                                    {
                                        borderColor=pressBorderColor;
                                        pageIndex.currentPage=currentPage.text;
                                        patientInfoListView.positionViewAtIndex((pageIndex.currentPage-1)*pageSize,ListView.Beginning);
                                    }
                                }
                                CusButton{
                                    id:nextPage;
                                    imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_4next_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_4next_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_4next_3press.svg";
                                    onClicked: {
                                        if(pageIndex.currentPage<pageIndex.totalPage)
                                        {
                                            pageIndex.currentPage+=1;
                                            patientInfoListView.positionViewAtIndex((pageIndex.currentPage-1)*pageSize,ListView.Beginning);
                                        }
                                    }
                                }
                                CusButton{id:endPage;
                                    imageHightScale: 1;width: height; borderWidth: 0;imageSrc: "qrc:/Pics/base-svg/page_5end_1normal.svg";hoverImageSrc:"qrc:/Pics/base-svg/page_5end_2hover.svg";pressImageSrc: "qrc:/Pics/base-svg/page_5end_3press.svg";
                                    onClicked: {
                                        pageIndex.currentPage=pageIndex.totalPage;
                                        patientInfoListView.positionViewAtIndex((pageIndex.currentPage-1)*pageSize,ListView.Beginning);
                                    }
                                }
                            }
                            Component.onCompleted: {queryStarted.connect(refresh)}
                            function refresh(){
                                console.log("refresh page...");
                                pageIndex.currentPage=1;
                                var rowCount=patientInfoListView.model.rowCount();
                                console.log("model count:"+rowCount);
                                pageIndex.totalPage=rowCount/pageSize+1;
                                pageIndex.totalRecordCount=rowCount;
                            }
                        }
                    }
                }
            }
            Column{
                width: (parent.width-content.layoutGroupMargin)*0.4; height: parent.height;spacing:content.layoutGroupMargin;
                Rectangle{
                    width: parent.width;height: (parent.height-content.layoutGroupMargin)*0.6;border.color: backGroundBorderColor;color: backGroundColor;radius: commonRadius;
                    Item{
                        anchors.fill: parent;anchors.margins: content.layoutGroupMargin;
                        Column{
                            id:newPatient;anchors.fill: parent;spacing: patientInfo.rowHight
                            Component.onCompleted: {
                                showControl();
                                IcUiQmlApi.appCtrl.doubleNameChanged.connect(showControl)
                            }

                            function showControl()
                            {
//                                console.log(doubleName);
                                if(!doubleName) {newChineseNameRow.visible=true;newEnglishFirstNameRow.visible=false;newEnglishLastNameRow.visible=false;}
                                else {newChineseNameRow.visible=false;newEnglishFirstNameRow.visible=true;newEnglishLastNameRow.visible=true;}
                            }

                            Row{
                                width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                CusText{text:"*患者ID "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                LineEdit{id:newId;width: parent.width*0.6}
                                CusButton{height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"}
                            }
                            Row{
                                id:newChineseNameRow;visible:false;width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                CusText{text:"*姓名 "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                LineEdit{id:newChineseName;width: parent.width*0.6}
                                CusButton{height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"}
                            }

                            Row{
                                id:newEnglishFirstNameRow;visible:false;width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                CusText{text:"*名字 "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                LineEdit{id:newEnglishFirstName;width: parent.width*0.6}
                                CusButton{height: parent.height;width: height;imageSrc:"qrc:/Pics/base-svg/btn_find.svg"}
                            }

                            Row{
                                id:newEnglishLastNameRow;visible:false;width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                CusText{text:"*姓氏"; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                LineEdit{id:newEnglishLastName;width: parent.width*0.6}
                            }

                            Row{
                                width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                CusText{text:"*性别 "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                Row{
                                    id:genderSelect;property int gender;
                                    height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                    CusButton{
                                        id:manButton;property bool chosen:false;imageSrc: "qrc:/Pics/base-svg/btn_sex_man.svg";width: 2*height
                                        onClicked: {genderSelect.selectGender(0)}
                                    }
                                    CusButton{
                                        id:womanButton;property bool chosen:false;imageSrc: "qrc:/Pics/base-svg/btn_sex_woman.svg";width: 2*height
                                        onClicked:  {genderSelect.selectGender(1)}
                                    }
                                    CusButton{id:otherButton;property bool chosen:false;text:"其它";width:height*2;
                                         onClicked: {genderSelect.selectGender(2)}
                                    }
                                    function selectGender(id)

                                    {
                                        manButton.borderColor=manButton.commonBorderColor;
                                        womanButton.borderColor=womanButton.commonBorderColor;
                                        otherButton.borderColor=otherButton.commonBorderColor;
                                        switch(id)
                                        {
                                            case 0:genderSelect.gender=0; manButton.borderColor=manButton.pressBorderColor;break;
                                            case 1:genderSelect.gender=1; womanButton.borderColor=womanButton.pressBorderColor;break;
                                            case 2:genderSelect.gender=2; otherButton.borderColor=otherButton.pressBorderColor;break;
                                        }
                                    }
                                }
                            }
                            Row{
                                id:newBirthDateRow
                                width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                CusText{text:"*出生日期 "; horizontalAlignment: Text.AlignRight ;width:parent.width*0.20}
                                Row{
                                    height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                    Item{
                                        height: parent.height;width: 2*height;
                                        LineEdit
                                        {
                                            id:newDateBirth;width: height*3.1;onTextChanged:
                                            {
                                                var birthDate=new Date(Date.parse(newDateBirth.text));
                                                var now=new Date(Date.now());
                                                var age=now.getFullYear()-birthDate.getFullYear();
                                                if(now.getMonth()<birthDate.getMonth()) age-=1;
                                                if((now.getMonth()==birthDate.getMonth())&&(now.getDay()<birthDate.getDay())) age-=1;
                                                newPatientage.text=age;
                                            }
                                        }
                                    }
                                    CusButton{text:"选择";width:height*2;onClicked:{calendar.inputObj=newDateBirth;calendar.open();}}
                                    LineEdit{id:newPatientage;width: height*2;text:"1";radius: height*0.2;horizontalAlignment: Text.AlignHCenter;readOnly: true;backgroundColor:backGroundColor;}
                                }

                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width;height: (parent.height-content.layoutGroupMargin)*0.4;
                    border.color: backGroundBorderColor;color: backGroundColor;radius: commonRadius;
                   Item{
                       anchors.fill: parent;anchors.margins: content.layoutGroupMargin;
                       Column{
                           anchors.fill: parent;
                           Row{
                               width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                               Item{height:parent.height;width:parent.width*0.20}
                               Row{
                                   height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                   CusText{ text:"球面";width: 2*height}
                                   CusText{ text:"柱面";width: 2*height}
                                   CusText{ text:"轴向";width: 2*height}
                               }
                           }
                           Column{
                               id:eyeBallCalc
                               width: parent.width;height: parent.height-patientInfo.rowHight;spacing: patientInfo.rowHight;
                               Row{
                                   width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                   CusText{text:"右眼";width:parent.width*0.20}
                                   Row{
                                       height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                   }
                               }
                               Row{
                                   width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
                                   CusText{text:"左眼";width:parent.width*0.20}
                                   Row{
                                       height:parent.height;spacing:(width-6*height)/2;width:newPatient.width*0.6
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                       LineEdit{ width: 2*height}
                                   }
                               }
                               Row{
                                   width: parent.width;height:patientInfo.rowHight;spacing: parent.width*0.02
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
    }
    Rectangle{
        id:bottomRibbon;width: parent.width;height: parent.height*1/15;color: "#333e44";
        anchors.bottom: parent.bottom
        Row{
            anchors.fill: parent;anchors.margins:parent.height*0.15;
            Item{
                height: parent.height;width:parent.width*0.6;
                CusButton{text:"关闭"}
                Flow{
                    height: parent.height;spacing: height*0.8
                    anchors.horizontalCenter: parent.horizontalCenter
                    CusButton{text:"复查"}
                    CusButton{
                        id:patientReviseButton;
                        enabled: false;
                        text:"修改";
                        onClicked:
                        {
                            var Name;
                            if(!doubleName) Name=newChineseName.text;else {Name=newEnglishFirstName.text+" "+newEnglishLastName.text;}
                            IcUiQmlApi.appCtrl.databaseSvc.updatePatient(IcUiQmlApi.appCtrl.currentPatient.id,newId.text,Name,genderSelect.gender,newDateBirth.text);
                            query.startQuery();
                        }
                    }
                    CusButton
                    {
                        text:"删除";
                        onClicked: {
                            var pl=patientInfoListView.seletedPatient;
                            for(var i=0;i<pl.length;i++){
                                console.log(pl[i])
                                 IcUiQmlApi.appCtrl.databaseSvc.deletePatient(pl[i]);
                            }
                            query.startQuery();
                        }

                    }
                    CusButton{text:"分析";onClicked: {root.changePage("reportAndAnalysis");}}
                }
            }
            Flow{
                height:parent.height; layoutDirection: Qt.RightToLeft;width:parent.width*0.4;spacing: height*0.8;
                CusButton{text:"进入检测";onClicked: {root.changePage("check");}}
                CusButton{
                    id:patientSaveButton;text:"保存";
                    enabled: false;
                    onClicked:{
                        var Name="";
                        if(!doubleName){ Name=newChineseName.text; }
                        else {Name = newEnglishFirstName.text+" "+newEnglishLastName.text;}
                        IcUiQmlApi.appCtrl.databaseSvc.addPatient(newId.text,Name,genderSelect.gender,newDateBirth.text);
                        query.startQuery();
                    }
                }
                CusButton{
                    text:"新建";
                    onClicked: {
                        patientReviseButton.enabled=false;patientReviseButton.buttonColor="#787878"
                        patientSaveButton.enabled=true; patientSaveButton.buttonColor = "#dcdee0"
                        newId.text="";
                        newChineseName.text="";
                        newEnglishFirstName.text="";
                        newEnglishLastName.text="";
                        genderSelect.selectGender(0);
                        newDateBirth.text="";
                    }
                }
            }
        }
    }
}
