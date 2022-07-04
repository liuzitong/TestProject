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
    property var textHeight;
    property var currentPatient: null;
    property var currentProgram: null;
    property var currentCheckResult: null;
    property var analysisResult: null;
    property var report:null;                           //0:常规,1:三合一,2:总览,3:筛选
    width: 1366;
    height: 660;
    onRefresh:
    {
        switch(report)
        {
        case 0:content.source="StaticAnalysisReportSingle.qml";
            break;
        case 1:content.source="StaticAnalysisReportThreeInOne.qml";
            break;
        case 2:content.source="StaticAnalysisReportOverView.qml";
            break;
        case 3:content.source="StaticAnalysisReportScreening.qml";
            break;
        }
        content.item.refresh();
    }

    Row{
        anchors.fill: parent;
        Rectangle{width:parent.width*0.75;height: parent.height;color:"white";
            Row{anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;spacing: width*0.10;
                Column{ id: column;width: parent.width*0.25;height: parent.height;spacing:parent.height*0.05
                    CusText{text:currentProgram.name; font.bold: true; horizontalAlignment: Text.AlignLeft;height:parent.height*0.10;}
                    Column{id:ttt;width:parent.width;height: parent.height*0.30;spacing: textHeight*-0.36;
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
    //                        CusText{text:"固视监测";  horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                    }
                    Column{width:parent.width;height: parent.height*0.15;spacing: textHeight*-0.36;
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
                    Column{width:parent.width;height: parent.height*0.15;spacing: textHeight*-0.36;
                        Repeater{
                            model: [
                                {name:"瞳孔直径",param:"2mm"},
                                {name:"视力",param:"0.8"},
                                {name:"屈光度",param:"DS   DC X"}]
                           CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                        }
                    }
                    Item{ id: item2;width:parent.width;height: parent.height*0.10;anchors.left: parent.left;anchors.leftMargin: (width-322)/2
                        ShortTermFluctuation{ dots:if(currentCheckResult!==null) currentCheckResult.resultData.shortTermFluctuation;  }
                    }
                }

                Loader
                {
                    id:content;
                    width: parent.width*0.65;height:parent.height
                }

            }
        }
        Rectangle{width:parent.width*0.25;height: parent.height;color:"white";}
    }

}

