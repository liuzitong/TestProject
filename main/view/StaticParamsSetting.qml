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
                    CusText{text:"参数设置"; anchors.right: parent.right; anchors.rightMargin: height*0.5;font.pointSize:fontPointSize;}
                }
                Column{
                    width: parent.width;height: parent.height-header.height;
                    Rectangle{
                        id: rectangle
                        height: parent.height*0.07;width:parent.width-2;color: "#D2D2D3";anchors.horizontalCenter: parent.horizontalCenter;
                        TabBar {id: bar;currentIndex: 0;height: parent.height*0.8;width:isCustomProg?parent.width*0.20:parent.width*0.10; anchors.bottom: parent.bottom; anchors.bottomMargin: 0;anchors.left: parent.left; anchors.leftMargin: 0.01*parent.width;spacing: 0;
                            Repeater {
                                model:isCustomProg?["常用参数","固定参数"]:["常用参数"]
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
                                        CusText{text:"测试范围"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;text:currentProgram===null?0:currentProgram.params.commonParams.Range[1];}

                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"测试点数"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        LineEdit{width: parent.width*0.6; anchors.right: parent.right;readOnly: true;text:currentProgram===null?0:currentProgram.dots.length;}
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"策略"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            property var threshold:["全阈值","智能交互式","快速智能交互式"];
                                            property var screening: ["二区法","三区法","量化缺损","单刺激"];
                                            width: parent.width*0.6; anchors.right: parent.right;model:currentProgram===null?null:currentProgram.type===0?threshold:screening;
                                            currentIndex: currentProgram===null?0:(currentProgram.type===0?currentProgram.params.commonParams.strategy:currentProgram.params.commonParams.strategy-3);
                                            Component.onCompleted: idPopup.ok.connect(function(){if(currentProgram.type===0) {currentProgram.params.commonParams.strategy=currentIndex;}else {currentProgram.params.commonParams.strategy=(currentIndex+3);}});
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"策略模式"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["年龄相关","阈值相关","单刺激"];
                                            currentIndex: currentProgram===null?0:currentProgram.params.commonParams.strategyMode;
                                            Component.onCompleted: idPopup.ok.connect(function(){currentProgram.params.commonParams.strategyMode=currentIndex;});
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"光标大小"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["I","II","III","IV","V"];
                                            currentIndex: currentProgram===null?0:currentProgram.params.commonParams.cursorSize;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.cursorSize=currentIndex;})}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"光标颜色"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["白色","红色","蓝色"];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.cursorColor;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.cursorColor=currentIndex;})}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"背景光颜色"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["白色","黄色"];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.backGroundColor;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.backGroundColor=currentIndex;})}
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"兰·黄测试"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["打开","关闭"];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.cyanYellowTest;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.cyanYellowTest=currentIndex;})}
                                        }
                                    }
                                }
                                Column{
                                   height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.45;
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"响应自适应"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                           width: parent.width*0.6; anchors.right: parent.right;model:["打开","关闭"];currentIndex: currentProgram===null?0:currentProgram.params.commonParams.responseAutoAdapt;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.responseAutoAdapt=currentIndex;})}
                                       }

                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"自适应时间"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:50;max:5000;min:0;
                                           Component.onCompleted: {
                                               idPopup.ok.connect(function(){currentProgram.params.commonParams.intervalTime=value;})
                                               idPopup.currentProgramChanged.connect(function(){if(currentProgram!==null){value=currentProgram.params.commonParams.intervalTime;}});
                                           }
                                       }

                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"中心点监测"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["打开","关闭"];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.centerDotCheck;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.centerDotCheck=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"短期波动"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["打开","关闭"];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.shortTermFluctuation;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.shortTermFluctuation=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"固视选择"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                            width: parent.width*0.6; anchors.right: parent.right;model:["中心点","小菱形","大菱形","底点"];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.fixationTarget;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.fixationTarget=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"眼动模式报警"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                           width: parent.width*0.6; anchors.right: parent.right;model:["不报警","只报警","报警并暂停"];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.fixationMonitor;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.commonParams.fixationMonitor=currentIndex;})}
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"盲点测试"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       CusComboBox{
                                           width: parent.width*0.6; anchors.right: parent.right;model:["打开","关闭"];currentIndex:currentProgram===null?0:currentProgram.params.commonParams.blindDotTest;
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
                                        CusText{text:"刺激时间"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:10;max:500;min:0;value:currentProgram===null?0:currentProgram.params.fixedParams.stimulationTime;
                                            Component.onCompleted:
                                            {
                                                idPopup.ok.connect(function(){currentProgram.params.fixedParams.stimulationTime=value;
                                                idPopup.currentProgramChanged.connect(function(){if(currentProgram!==null);value=currentProgram.params.fixedParams.stimulationTime;});}) }
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"间隔时间"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:50;max:5000;min:0;value:currentProgram===null?0:currentProgram.params.fixedParams.intervalTime;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.intervalTime=value;idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.intervalTime;});}) }
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"假阳性周期"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:0;value:currentProgram===null?0:currentProgram.params.fixedParams.falsePositiveCycle;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.falsePositiveCycle=value;idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.falsePositiveCycle;});}) }
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"假阴性周期"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:0;value:currentProgram===null?0:currentProgram.params.fixedParams.falseNegativeCycle;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.falseNegativeCycle=value;idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.falseNegativeCycle;});}) }
                                        }
                                    }
                                    Item{
                                        width: parent.width; height:parent.parent.rowHeight;
                                        CusText{text:"固视丢失率周期"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                        NumberLineEdit{
                                            width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:0;value:currentProgram===null?0:currentProgram.params.fixedParams.fixationViewLossCycle;
                                            Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.fixationViewLossCycle=value;idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.fixationViewLossCycle;});}) }
                                        }
                                    }

                                }
                                Column{
                                   height: parent.height;width: parent.width*0.45;spacing:parent.rowHeight*0.45;
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"单刺激分贝"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:1;max:51;min:0;value:currentProgram===null?0:currentProgram.params.fixedParams.singleStimulationDB;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.singleStimulationDB=value;idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.singleStimulationDB;});}) }
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"盲点刺激分贝"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:1;max:51;min:0;value:currentProgram===null?0:currentProgram.params.fixedParams.blindDotStimulationDB;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.blindDotStimulationDB=value;idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.blindDotStimulationDB;});}) }
                                       }
                                   }
                                   Item{
                                       width: parent.width; height:parent.parent.rowHeight;
                                       CusText{text:"短期波动次数"; anchors.left: parent.left; anchors.leftMargin: 0;width: parent.width*0.35;horizontalAlignment: Text.AlignLeft;font.pointSize:fontPointSize;}
                                       NumberLineEdit{
                                           width: parent.width*0.6; anchors.right: parent.right;step:1;max:50;min:1;value:currentProgram===null?0:currentProgram.params.fixedParams.shortTermFluctuationCount;
                                           Component.onCompleted: {idPopup.ok.connect(function(){currentProgram.params.fixedParams.shortTermFluctuationCount=value;idPopup.currentProgramChanged.connect(function(){value=currentProgram.params.fixedParams.shortTermFluctuationCount;});}) }
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
                            CusButton{text:"取消";onClicked: idPopup.close();}
                            CusButton{text:"确定";onClicked:{ok();idPopup.close();dataRefreshed();}}
//                             CusButton{text:"aa";onClicked:{
//                                     console.log(currentProgram.params.commonParams.cursorSize) ;
//                                     console.log(currentProgram.params.commonParams.intervalTime) ;
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
