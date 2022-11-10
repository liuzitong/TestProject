import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Utils 1.0

Item
{
    id:root;
    signal refresh();
    property var currentProgram: null;
    property var currentPatient: null;
    property var currentCheckResult: null;
    property var progressAnalysisListVm: null;//<-
    property var progressAnalysisResult: null;//-<
    property var staticAnalysisVm: null;//<-
    property var staticAnalysisResult: null;
    property int fontPointSize: CommonSettings.fontPointSize;
    property int textHeight:CommonSettings.textHeight;
    property string rx:"";
    signal clearDot;
    property int range: currentProgram.params.commonParams.Range[1];
    width: 1366;
    height: 660;
    onRefresh:
    {
        var checkResultId=progressAnalysisListVm.selectedResultId;
        if(currentCheckResult!==null)
        {
            if(currentCheckResult.type!==2)
                IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticCheckResultVm", currentCheckResult);
            else
                IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::DynamicCheckResultVm", currentCheckResult);
        }
        currentCheckResult=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticCheckResultVm", false,[checkResultId]);

        if(currentProgram!==null)
           IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);
        currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false,[currentCheckResult.program_id]);

        if(currentPatient.os_od===0)  //左
            rx=currentPatient.rx.rx1_l.toFixed(2)+"DS "+currentPatient.rx.rx2_l.toFixed(2)+"DC "+currentPatient.rx.rx3_l.toFixed(2)+"X";
        else
            rx=currentPatient.rx.rx1_r.toFixed(2)+"DS "+currentPatient.rx.rx2_r.toFixed(2)+"DC "+currentPatient.rx.rx3_r.toFixed(2)+"X";

        if(staticAnalysisResult!==null)
        {
            staticAnalysisResult.destroy();
        }
        staticAnalysisResult=staticAnalysisVm.getResult();
    }

    Row{
        anchors.fill: parent;
        Rectangle{width:parent.width*0.75;height: parent.height;color:"white";
            Row{anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;spacing: width*0.05;
                Column{ id: column;width: parent.width*0.25;height: parent.height;spacing:parent.height*0.02
                    CusText{text:currentProgram.name; font.bold: true; horizontalAlignment: Text.AlignLeft;height:parent.height*0.10;font.pointSize: fontPointSize*2;}
                    Column{id:ttt;width:parent.width;height: parent.height*0.25;spacing: textHeight*0.5;
                        Repeater{
                            property var params: currentCheckResult.params.commonParams;
                            property int timeSpan:currentCheckResult.resultData.testTimespan;
                            property var fixationMonitor: ["不报警","只报警","报警并暂停"];
                            property var fixationTarget: ["中心点","小菱形","大菱形","底点"]
                            property var centerDotCheck: ["打开","关闭"];

                            model: [
                                {name:"眼动报警模式",param:fixationMonitor[params.fixationMonitor]},
                                {name:"固视选择",param:fixationTarget[params.fixationTarget]},
                                {name:"固视丢失率",param:Math.round(currentCheckResult.resultData.fixationLostCount/currentCheckResult.resultData.fixationLostTestCount*100)+"%"},
                                {name:"假阳性率",param:Math.round(currentCheckResult.resultData.falsePositiveCount/currentCheckResult.resultData.falsePositiveTestCount*100)+"%"},
                                {name:"假阴性率",param:Math.round(currentCheckResult.resultData.falseNegativeCount/currentCheckResult.resultData.falseNegativeTestCount*100)+"%"},
                                {name:"测试用时",param:Math.floor(timeSpan/60)+":"+timeSpan%60},
                                {name:"中心点检测",param:params.centerDotCheck?centerDotCheck[0]:centerDotCheck[1]}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
                    }
                    Column{width:parent.width;height: parent.height*0.12;spacing: textHeight*0.5;
                        Repeater{
                            property var params: currentCheckResult.params.commonParams;
                            property var cursorSize: ["I","II","III","IV","V"];
                            property var cursorColor: ["白色","红色","蓝色"];
                            property var backGroundColor: ["31.5 ASB","315 ASB"];
                            property var strategy: ["全阈值","智能交互式","快速智能交互式","二区法","三区法","量化缺损","单刺激"]
                            model: [
                                {name:"刺激光标",param:cursorSize[params.cursorSize]+","+cursorColor[params.cursorColor]},
                                {name:"背景光",param:backGroundColor[params.backGroundColor]},
                                {name:"策略",param:strategy[params.strategy]}]
                            CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
                    }
                    Column{width:parent.width;height: parent.height*0.12;spacing: textHeight*0.5;
                        Repeater{
                            model: [
                                {name:"瞳孔直径",param:currentCheckResult.resultData.pupilDiameter.toFixed(2)+"mm"},
                                {name:"视力",param:!currentCheckResult.OS_OD?currentPatient.rx.visual_l.toFixed(2):currentPatient.rx.visual_r.toFixed(2)},
                                {name:"屈光度",param:"Rx:"+rx}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
                    }

                    Column{width:parent.width;height: parent.height*0.20;spacing: textHeight*0.3;
                        CusText{text:"诊断:"; horizontalAlignment: Text.AlignLeft;width:parent.width;height:textHeight;}
                        Rectangle{ id: rectangle;width:parent.width;height: parent.height-1.3*textHeight;radius: 5;border.color: "black";smooth: false;
                            TextInput
                            {
                                id:diagnosis;anchors.fill: parent;anchors.margins: 3;
                                width:parent.width*1.0;height: parent.height*0.70;
                                text:currentCheckResult==null?"":currentCheckResult.diagnosis;
                                selectionColor: "blue";selectByMouse: true;
                                font.pointSize: fontPointSize;font.family: "Consolas";
                                wrapMode: Text.WrapAnywhere;renderType: Text.NativeRendering;
                            }

                            CusButton{ id: cusButton;height: parent.height*0.28;width: height*2;
                                text:"保存";anchors.right: parent.right;anchors.bottom: parent.bottom;
                                anchors.rightMargin: 5;anchors.bottomMargin: 5;
                                onClicked:
                                {
                                    currentCheckResult.diagnosis=diagnosis.text;
                                    currentCheckResult.update();
                                }
                            }
                        }
                    }


                    Item{width:parent.width;height: parent.height*0.08;
                        FixationDeviation{ dots:if(currentCheckResult!==null) currentCheckResult.resultData.fixationDeviation; }
                    }
                }
                Row {width: parent.width*0.70;height:parent.height
                    Column{width: parent.width*0.70;height:parent.height/*spacing:height*0.035*/
                        Row{ id: row;width: parent.width;height:parent.height*0.96/3;spacing:width*0.1
                            AnalysisDiagram{
                                width: parent.height;height:parent.height*1.25;
                                anchors.verticalCenter: parent.verticalCenter
                                source:"/previewImage/dbDiagram.bmp";
                                range:root.range;
                                analysisVm: root.staticAnalysisVm;
                                onClicked:root.clearDot();
                                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
                            }
                            AnalysisDiagram{
                                width: parent.height;height:parent.height*1.25;
                                anchors.verticalCenter: parent.verticalCenter
                                source:"/previewImage/gray.bmp";
                                range:root.range;
                                analysisVm: root.staticAnalysisVm;
                                onClicked:root.clearDot();
                                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
                            }
                        }
                        Item{width: parent.width;height:parent.height*0.01;}

                        Row{
                            width: parent.width;height:parent.height*0.96/3;spacing:width*0.1
                            AnalysisDiagram{
                                width: parent.height;height:parent.height*1.25;
                                anchors.verticalCenter: parent.verticalCenter
                                source:"/previewImage/TotalDeviation.bmp";
                                range:root.range;
                                analysisVm: root.staticAnalysisVm;
                                onClicked:root.clearDot();
                                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
                            }

                            AnalysisDiagram{
                                width: parent.height;height:parent.height*1.25;
                                anchors.verticalCenter: parent.verticalCenter
                                source:"/previewImage/PatternDeviation.bmp";
                                range:root.range;
                                analysisVm: root.staticAnalysisVm;
                                onClicked:root.clearDot();
                                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
                            }


                        }
                        Row{width: parent.width;height:parent.height*0.03;spacing:width*0.1;
                            CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "总体偏差"; verticalAlignment: Text.AlignVCenter; font.pointSize: height*0.50}
                            CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "模式偏差";  verticalAlignment: Text.AlignVCenter;font.pointSize: height*0.50}
                        }
                        Row{
                            width: parent.width;height:parent.height*0.96/3;spacing:width*0.1;
                            AnalysisDiagram{
                                width: parent.height;height:parent.height*1.25;
                                anchors.verticalCenter: parent.verticalCenter
                                source:"/previewImage/TotalPE.bmp";
                                range:root.range;
                                analysisVm: root.staticAnalysisVm;
                                onClicked:root.clearDot();
                                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
                            }
                            AnalysisDiagram{
                                width: parent.height;height:parent.height*1.25;
                                anchors.verticalCenter: parent.verticalCenter
                                source:"/previewImage/PatternPE.bmp";
                                range:root.range;
                                analysisVm: root.staticAnalysisVm;
                                onClicked:root.clearDot();
                                Component.onCompleted: {root.refresh.connect(refresh);root.clearDot.connect(clearDot);}
                            }

//                            }
                        }
                    }
                    Column{width: parent.width*0.3;height:parent.height;spacing: textHeight*0.5;
                        CusText{width:parent.width;height:textHeight;text:"VFI:"+Math.round(staticAnalysisResult.VFI*100)+"%" ;horizontalAlignment: Text.AlignLeft;}
                        CusText{
                            property var ght: if(staticAnalysisResult.GHT===0){return "超出界限";} else if(staticAnalysisResult.GHT===1){return "普遍敏感度降低";}else if(staticAnalysisResult.GHT===2){return "边界";}else if(staticAnalysisResult.GHT===3){return "正常范围";}
                            width:parent.width;height:textHeight;horizontalAlignment: Text.AlignLeft;text:"青光眼半视野检查: "+ ght;}
                        CusText{width:parent.width;height:textHeight;text:"平均缺损: "+staticAnalysisResult.md.toFixed(2)+(staticAnalysisResult.p_md>0?" (<"+staticAnalysisResult.p_md+"%)":"") ;horizontalAlignment: Text.AlignLeft;}
                        CusText{width:parent.width;height:textHeight;text:"模式标准偏差: "+staticAnalysisResult.psd.toFixed(2)+(staticAnalysisResult.p_psd>0?" (<"+staticAnalysisResult.p_psd+"%)" :"");horizontalAlignment: Text.AlignLeft;}
                        Rectangle{
                            width: parent.width;height: parent.height*0.66;border.color: "black";
                            Column{
                                width: parent.width*0.9;height: parent.height; anchors.horizontalCenter: parent.horizontalCenter;spacing: textHeight*0.5;
                                CusText{width: parent.width;height: textHeight;text:"GPA";}
                                Image {
                                    height: sourceSize.height; anchors.horizontalCenter: parent.horizontalCenter;width: sourceSize.width;
                                    smooth: false;
                                    cache: false;        //to refresh image
                                    Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/previewImage/single_progressPic.bmp";})}
                                }
                                CusText{
                                    property var progress: ["没有进展","可能的进展","很有可能的进展"];
                                    height: textHeight;text:progressAnalysisResult===null?"":progress[progressAnalysisResult] ;horizontalAlignment: Text.AlignLeft
                                }
                                CusText{height: textHeight;text:"基线检查:" ;horizontalAlignment: Text.AlignLeft}
                                Row{width: parent.width;height: textHeight;spacing: parent.width*0.1;
                                    CusText{height: parent.height;width:parent.width*0.4;text:progressAnalysisListVm===null?"":Qt.formatDateTime(progressAnalysisListVm.getData(0,"dateTime"),"yyyy/MM/dd"); horizontalAlignment: Text.AlignLeft}
                                    CusText{height: parent.height;width:parent.width*0.4;text:progressAnalysisListVm===null?"":Qt.formatDateTime(progressAnalysisListVm.getData(1,"dateTime"),"yyyy/MM/dd"); horizontalAlignment: Text.AlignLeft}
                                }
                                CusText{height: textHeight;text:"以前的追踪检查:" ;horizontalAlignment: Text.AlignLeft}
                                Row{width: parent.width;height: textHeight;spacing: parent.width*0.1;
                                    CusText{height: parent.height;width:parent.width*0.4;text:progressAnalysisListVm===null?"":Qt.formatDateTime(progressAnalysisListVm.getData(progressAnalysisListVm.selectedIndex-2,"dateTime"),"yyyy/MM/dd"); horizontalAlignment: Text.AlignLeft}
                                    CusText{height: parent.height;width:parent.width*0.4;text:progressAnalysisListVm===null?"":Qt.formatDateTime(progressAnalysisListVm.getData(progressAnalysisListVm.selectedIndex-1,"dateTime"),"yyyy/MM/dd"); horizontalAlignment: Text.AlignLeft}
                                }
                                Repeater{
                                    anchors.fill: parent;
                                    model: [{image:"qrc:/grays/GPA1.bmp",progress:"<5% (恶化)"},{image:"qrc:/grays/GPA2.bmp",progress:"<5% (2连续)"},{image:"qrc:/grays/GPA3.bmp",progress:"<5% (3连续)"},{image:"qrc:/grays/GPA4.bmp",progress:"超出范围"}]
                                    Row{
                                        height: textHeight;width: parent.width;
        //                                height:parent.height*0.14;width: parent.width;spacing: height*0.5
                                        Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                                        CusText{height: textHeight;text:modelData.progress; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;}
                                    }
                                }
                            }
                        }


                        Column
                        {
                            width: parent.width;height: parent.height*0.25;spacing: textHeight*0.5;
                            Repeater{
                                anchors.fill: parent;
                                model: [{image:"qrc:/grays/PE1.bmp",pe:"<5%"},{image:"qrc:/grays/PE2.bmp",pe:"<2%"},{image:"qrc:/grays/PE3.bmp",pe:"<1%"},{image:"qrc:/grays/PE4.bmp",pe:"<0.5%"}]
                                Row{
                                    height: textHeight;width: parent.width;
    //                                height:parent.height*0.14;width: parent.width;spacing: height*0.5
                                    Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                                    CusText{height: textHeight;text:modelData.pe; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;}
                                }
                            }
                        }


                    }
                }


            }
        }


        Rectangle{width:parent.width*0.25;height: parent.height;color:"white";}
    }

}


