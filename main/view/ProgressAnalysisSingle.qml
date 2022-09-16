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
    property int textHeight:height*0.05;
    property var currentProgram: null;
    property var currentPatient: null;
    property var currentCheckResult: null;
    property var progressAnalysisListVm: null;//<-
    property var progressAnalysisResult: null;//-<
    property var staticAnalysisVm: null;//<-
    property var staticAnalysisResult: null;
    property string rx:"";
    signal clearDot;
    property int range: currentProgram.params.commonParams.Range[1];
    width: 1366;
    height: 660;
    onRefresh:
    {
        var checkResultId=progressAnalysisListVm.selectedResultId;

        if(currentCheckResult!==null)
        {IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::CheckResultVm", currentCheckResult);}
        currentCheckResult=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::CheckResultVm", false,[checkResultId]);

        if(currentProgram!==null)
           IcUiQmlApi.appCtrl.objMgr.detachObj("Perimeter::StaticProgramVM", currentProgram);
        currentProgram=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::StaticProgramVM", false,[currentCheckResult.program_id]);


        if(currentPatient.os_od===0)  //左
            rx=currentPatient.rx.rx1_l.toFixed(2)+"DS "+currentPatient.rx.rx2_l.toFixed(2)+"DC "+currentPatient.rx.rx3_l.toFixed(2)+"X";
        else
            rx=currentPatient.rx.rx1_r.toFixed(2)+"DS "+currentPatient.rx.rx2_r.toFixed(2)+"DC "+currentPatient.rx.rx3_r.toFixed(2)+"X";

        if(staticAnalysisResult!==null)
        {
            staticAnalysisResult.destory();
        }
        staticAnalysisResult=staticAnalysisVm.getResult();
    }

    Row{
        anchors.fill: parent;
        Rectangle{width:parent.width*0.75;height: parent.height;color:"white";
            Row{anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;spacing: width*0.10;
                Column{ id: column;width: parent.width*0.25;height: parent.height;spacing:parent.height*0.02
                    CusText{text:currentProgram.name; font.bold: true; horizontalAlignment: Text.AlignLeft;height:parent.height*0.10;}
                    Column{id:ttt;width:parent.width;height: parent.height*0.25;spacing: textHeight*-0.36;
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
                    Column{width:parent.width;height: parent.height*0.12;spacing: textHeight*-0.36;
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
                    Column{width:parent.width;height: parent.height*0.12;spacing: textHeight*-0.36;
                        Repeater{
                            model: [
                                {name:"瞳孔直径",param:currentCheckResult.resultData.pupilDiameter.toFixed(2)+"mm"},
                                {name:"视力",param:!currentCheckResult.OS_OD?currentPatient.rx.visual_l.toFixed(2):currentPatient.rx.visual_r.toFixed(2)},
                                {name:"屈光度",param:"Rx:"+rx}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
                    }

                    Column{width:parent.width;height: parent.height*0.25;spacing: textHeight*-0.18;
                        CusText{text:"诊断:"; horizontalAlignment: Text.AlignLeft;width:parent.width;height: parent.height*0.24;}
                        Rectangle{ id: rectangle;width:parent.width;height: parent.height*0.73;radius: 5;border.color: "black";smooth: false;
                            TextInput
                            {
                                id:diagnosis;anchors.fill: parent;anchors.margins: 3;
                                width:parent.width*1.0;height: parent.height*0.70;
                                text:currentCheckResult==null?"":currentCheckResult.diagnosis;
                                selectionColor: "blue";selectByMouse: true;
                                font.pointSize: textHeight*0.3;font.family: "Consolas";
                                wrapMode: Text.WrapAnywhere;renderType: Text.NativeRendering;
                            }

                            CusButton{ id: cusButton;height: parent.height*0.25;width: height*2;
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


                    Item{ id: item2;width:parent.width;height: parent.height*0.10;anchors.left: parent.left;
                        FixationDeviation{ dots:if(currentCheckResult!==null) currentCheckResult.resultData.fixationDeviation; }
                    }
                }
                Row {width: parent.width*0.65;height:parent.height
                    Column{width: parent.width*0.7;height:parent.height/*spacing:height*0.035*/
                        Row{ id: row;width: parent.width;height:parent.height*0.96/3;spacing:width*0.06
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
                            width: parent.width;height:parent.height*0.96/3;spacing:width*0.06
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
                        Row{width: parent.width;height:parent.height*0.03;spacing:width*0.06;
                            CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "总体偏差"; verticalAlignment: Text.AlignVCenter; font.pointSize: height*0.50}
                            CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "模式偏差";  verticalAlignment: Text.AlignVCenter;font.pointSize: height*0.50}
                        }
                        Row{
                            width: parent.width;height:parent.height*0.96/3;spacing:width*0.06
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
                    Column{width: parent.width*0.3;height:parent.height;
                        CusText{width:parent.width;height:textHeight;text:"VFI:"+Math.round(staticAnalysisResult.VFI*100)+"%" ;horizontalAlignment: Text.AlignLeft;}
                        CusText{
                            property var ght: if(staticAnalysisResult.GHT===0){return "超出界限";} else if(staticAnalysisResult.GHT===1){return "普遍敏感度降低";}else if(staticAnalysisResult.GHT===2){return "边界";}else if(staticAnalysisResult.GHT===3){return "正常范围";}
                            width:parent.width;height:textHeight;horizontalAlignment: Text.AlignLeft;text:"青光眼半视野检查: "+ ght;}
                        CusText{width:parent.width;height:textHeight;text:"平均缺损: "+staticAnalysisResult.md.toFixed(2)+(staticAnalysisResult.p_md>0?" (<"+staticAnalysisResult.p_md+"%)":"") ;horizontalAlignment: Text.AlignLeft;}
                        CusText{width:parent.width;height:textHeight;text:"模式标准偏差: "+staticAnalysisResult.psd.toFixed(2)+(staticAnalysisResult.p_psd>0?" (<"+staticAnalysisResult.p_psd+"%)" :"");horizontalAlignment: Text.AlignLeft;}

                        Repeater{
                            anchors.fill: parent;
                            model: [{image:"qrc:/grays/PE1.bmp",pe:"<5%"},{image:"qrc:/grays/PE2.bmp",pe:"<2%"},{image:"qrc:/grays/PE3.bmp",pe:"<1%"},{image:"qrc:/grays/PE4.bmp",pe:"<0.5%"}]
                            Row{
                                height: childAt(0,0).height*1.2;width: parent.width;
//                                height:parent.height*0.14;width: parent.width;spacing: height*0.5
                                Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                                CusText{height: parent.height;text:modelData.pe; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: height*0.70}
                            }
                        }

                    }
                }


            }
        }


        Rectangle{width:parent.width*0.25;height: parent.height;color:"white";border.color: "black";}
    }

}


