import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0     // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0  // [HINT] ModalPopupDialog is in it
import perimeter.main.view.Utils 1.0

ModalPopupDialog /*Rectangle*/{   // this is the wrapped Popup element in ui_qml_contro
    id:idPopup
    property alias color: idContent.color;
    reqEnterEventLoop:false;
//    width:1366; height: 640;
//    property alias contentWidth : idContent.implicitWidth;
//    property alias contentHeight: idContent.implicitHeight;
    property string backGroundColor:"#dcdee0"
    property string backGroundBorderColor:"#bdc0c6"
    property int type;
    signal ok();
    signal dataRefreshed();
    signal cancel();
    property bool isCustomProg:false;
    property var currentProgram:null;


    property int fontPointSize: CommonSettings.fontPointSize;




   contentItem:
   Rectangle{
        id: idContent; color: "#60606060";implicitWidth: idPopup.width; implicitHeight: idPopup.height;
        Rectangle
        {
        // [HINT] Popup element need implicitWidth & implicitHeight to calc. the right position
            id: menu; width:idPopup.width*0.5; height: idPopup.height*0.98;color: "#dcdee0";radius: 5;/*width:480; height:480;*/
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            border.color: "#7C7C7C";
            Column{
                id: column
                anchors.fill: parent;
                Canvas{
                    id:header;height: idPopup.height*0.05;width: parent.width;property alias radius: menu.radius;
                    onPaint: {var ctx = getContext("2d");ctx.beginPath(); // Start the path
                        ctx.moveTo(1,height);
                        ctx.lineTo(1, radius-1); // Set the path origin
                        ctx.arc(radius,radius,radius-1,Math.PI,Math.PI*1.5);
                        ctx.lineTo(width-radius, 1);// Set the path destination
                        ctx.arc(width-radius,radius,radius-1,Math.PI*1.5,0);
                        ctx.lineTo(width-1, height);
                        ctx.closePath();
                        context.fillStyle = "#D2D2D3"
                        ctx.fill();
                    }
                    CusText{text:qsTr("Params setting");horizontalAlignment: Text.AlignLeft;height:parent.height; anchors.left: parent.left; anchors.leftMargin:height*0.5;font.pointSize:fontPointSize;}
                }
                Column{
                    width: parent.width;height: parent.height-header.height;
                    Rectangle{
                        id: rectangle
                        height: parent.height*0.07;width:parent.width-2;color: "#D2D2D3";anchors.horizontalCenter: parent.horizontalCenter;
                        TabBar {id: bar;currentIndex: 0;height: parent.height*0.8;width:isCustomProg?parent.width*0.20:parent.width*0.10; anchors.bottom: parent.bottom; anchors.bottomMargin: 0;anchors.left: parent.left; anchors.leftMargin: 0.01*parent.width;spacing: 0;
                            Repeater {
                                model:isCustomProg?[qsTr("Common params"),qsTr("Fixed params")]:[qsTr("Common params")]
                                TabButton {width: isCustomProg?bar.width*0.5:bar.width*1;height: parent.height;
                                    Rectangle{anchors.fill: parent;color:parent.checked? "#E3E5E8":"#D2D2D3";
                                        Rectangle{width: parent.width;height: 3;color: "#0064B6";visible: parent.parent.checked? true:false; }
                                        CusText{text:modelData; anchors.fill: parent;color:parent.parent.checked?"#0064B6":"black";font.pointSize: height*0.3}
                                    }
                                }
                            }
                        }
                    }

                    StackLayout {
                        width: parent.width;height: parent.height*0.78;currentIndex: bar.currentIndex;
                        Rectangle{
                            anchors.fill: parent;
                            anchors.margins: parent.width*0.05;
                            radius: width*0.02;
                            id:eyeOptionsGroup;
                            width: parent.width*0.83;
                            height: parent.height*0.35;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            border.color: backGroundBorderColor;
                            color: backGroundColor;
                            Row{
                                anchors.fill: parent;
                                anchors.margins: parent.width*0.04;
                                spacing:width*0.1;
                                property int rowHeight:height*0.09;
                                Column{
                                    height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.45;
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Check range"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;text:currentProgram===null?0:currentProgram.params.commonParams.Range[1];}

                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Dots count"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;text:currentProgram===null?0:currentProgram.data.dots.length;}
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Strategy"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            property var strategies: currentProgram.data.strategies;
//                                            property var threshold:["全阈值","智能交互式","快速智能交互式"];
//                                            property var model: [/*"二区法","三区法","量化缺损","单刺激"*/];
//                                            property var threshold:[];
//                                            property var screening:[];
                                            property var listModel: ListModel{}
                                            width: parent.width*0.6; anchors.right: parent.right;
                                            model:/*currentProgram===null?null:currentProgram.type===0?threshold:screening;*/listModel;
                                            currentIndex: 0/*:(currentProgram.type===0?currentProgram.params.commonParams.strategy:currentProgram.params.commonParams.strategy-3)*/;
                                            Component.onCompleted: idPopup.ok.connect(function(){currentProgram.params.commonParams.strategy=strategies[currentIndex];});
                                            onStrategiesChanged:
                                            {

                                                console.log(strategies);
                                                var i;
                                                listModel.clear();
//                                                listModel.append({modelData:"全阈值"});
//                                                listModel.append({modelData:"智能交互式"});
//                                                listModel.append({modelData:"快速智能交互式"});
                                                if(currentProgram.type===0)
                                                {
                                                    for(i=0;i<strategies.length;i++)
                                                    {
                                                        switch (strategies[i])
                                                        {
                                                        case 0:listModel.append({modelData:qsTr("Full threshold")});break;
                                                        case 1:listModel.append({modelData:qsTr("Smart interactive")});break;
                                                        case 2:listModel.append({modelData:qsTr("Fast interactive")});break;
                                                        }
                                                        if(currentProgram.params.commonParams.strategy===strategies[i])
                                                        {
                                                            currentIndex=i;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    for(i=0;i<strategies.length;i++)
                                                    {
                                                        switch (strategies[i])
                                                        {
                                                        case 3:listModel.append({modelData:qsTr("One stage")});break;
                                                        case 4:listModel.append({modelData:qsTr("Two stages")});break;
                                                        case 5:listModel.append({modelData:qsTr("Quantify defects")});break;
                                                        case 6:listModel.append({modelData:qsTr("Single stimulus")});break;
                                                        }
                                                        if(currentProgram.params.commonParams.strategy===strategies[i])
                                                        {
                                                            currentIndex=i;
                                                        }
                                                    }
                                                }

                                            }
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Strategy mode"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("Age related"),qsTr("Threshold related"),qsTr("Single stimulus")];
                                            currentIndex: currentProgram===null?0:currentProgram.params.commonParams.strategyMode;
                                            Component.onCompleted: idPopup.ok.connect(function(){currentProgram.params.commonParams.strategyMode=currentIndex;});
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Cursor size"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["I","II","III","IV","V"];
                                            currentIndex: currentProgram===null?0:currentProgram.params.commonParams.cursorSize;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.cursorSize=currentIndex;})}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Cursor color"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("White"),qsTr("Red"),qsTr("Blue")];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.cursorColor;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.cursorColor=currentIndex;})}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Background color"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("White"),qsTr("Yellow")];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.backGroundColor;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.backGroundColor=currentIndex;})}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Cyan·yellow Check"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("On"),qsTr("Off")];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.cyanYellowTest;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.cyanYellowTest=currentIndex;})}
                                        }
                                    }
                                }
                                Column{
                                   height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.45;
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Response auto adapt"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                           width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("On"),qsTr("Off")];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.responseAutoAdapt;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.responseAutoAdapt=currentIndex;})}
                                       }

                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Response delay time"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:50;max:5000;min:0;
                                           Component.onCompleted: {
                                               idPopup.ok.connect(function(){currentProgram.params.commonParams.intervalTime=value;})
                                               idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.commonParams.responseDelayTime;});
                                           }
                                       }
                                   }


                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Center dot check"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("On"),qsTr("Off")];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.centerDotCheck;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.centerDotCheck=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Short term fluctuation"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("On"),qsTr("Off")];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.shortTermFluctuation;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.shortTermFluctuation=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Fixation target"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("Center dot"),qsTr("Small diamond"),qsTr("Big diamond"),qsTr("Bottom dot")];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.fixationTarget;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.fixationTarget=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Eye move alarm mode"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                           width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("No alarm"),qsTr("Only alarm"),qsTr("Alarm and pause")];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.fixationMonitor;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.fixationMonitor=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Blind dot test"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                           width: parent.width*0.6; anchors.right: parent.right;model:[qsTr("On"),qsTr("Off")];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.blindDotTest;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.blindDotTest=currentIndex;})}
                                       }
                                   }
                                }

                            }

                        }
                        Rectangle{
                            anchors.fill: parent;
                            anchors.margins: parent.width*0.04;
                            radius: width*0.02;
                            width: parent.width*0.83;
                            height: parent.height*0.35;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            border.color: backGroundBorderColor;
                            color: backGroundColor;
                            Row{
                                anchors.fill: parent;
                                anchors.margins: parent.width*0.04;
                                spacing:width*0.1;
                                property int rowHeight:height*0.09
                                Column{
                                    height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.45;
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Stimulus time"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:50;max:5000;min:0;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.stimulationTime=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.stimulationTime;});}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Inteval time"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:50;max:5000;min:0;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.intervalTime=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.intervalTime;});}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("False positive cycle"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:0;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.falsePositiveCycle=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.falsePositiveCycle;}); }
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("False negative cycle"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:0;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.falseNegativeCycle=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.falseNegativeCycle;}); }
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:qsTr("Fixation loss cycle"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:0;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.fixationViewLossCycle=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.fixationViewLossCycle;}); }
                                        }
                                    }

                                }
                                Column{
                                   height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.45;
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Single stimulus DB"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:1;max:51;min:0;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.singleStimulationDB=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.singleStimulationDB;}); }
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Blind dot stimulus DB"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:1;max:51;min:0;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.blindDotStimulationDB=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.blindDotStimulationDB;}); }
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:qsTr("Short term fluctuation count"); anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:1;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.shortTermFluctuationCount=value;});idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.shortTermFluctuationCount;}); }
                                       }
                                   }
                                }
                            }
                        }
                    }

                    Item {
                        width: parent.width;
                        height: parent.height*0.15
                        Flow{
                            anchors.top: parent.top
                            anchors.topMargin: parent.height*0.34;
                            anchors.right: parent.right
                            anchors.margins: parent.height*0.40;
                            spacing: height;
                            layoutDirection: Qt.RightToLeft
                            anchors.verticalCenter: parent.verticalCenter
//
                            CusButton{text:qsTr(qsTr("Cancel"));onClicked: idPopup.close();}
                            CusButton{text:qsTr(qsTr("OK"));onClicked:{ok();idPopup.close();dataRefreshed();}}
//                             CusButton{text:"aa";onClicked:{
//                                     console.log(currentProgram.params.commonParams.cursorSize) ;
//                                     console.log(currentProgram.params.fixedParams.stimulationTime) ;
//                                     stim.value=currentProgram.params.fixedParams.stimulationTime;
////                                     console.log(intevalTimeididid.value);

////                                     intevalTimeididid.value+=currentProgram.params.commonParams.intervalTime;
////                                     intevalTimeididid.value=currentProgram.params.commonParams.intervalTime;

//                                 }}
                        }
                    }
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
