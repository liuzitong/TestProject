import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Utils 1.0

Row{
    id:root;
    signal refresh();
    property var textHeight;
    property var currentPatient: null;
    property var currentProgram: null;
    property var currentCheckResult: null;
    property var analysisResult: null;

    anchors.fill: parent;
    Rectangle{width:parent.width*0.75;height: parent.height;color:"white";
        Row{anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;spacing: width*0.10;
            Column{ id: column;width: parent.width*0.25;height: parent.height;spacing:parent.height*0.05
                CusText{text:currentProgram.name; font.bold: true; horizontalAlignment: Text.AlignLeft;height:parent.height*0.10;}
                Column{id:ttt;width:parent.width;height: parent.height*0.30;spacing: textHeight*-0.36;
                    Repeater{
                        property var params: currentCheckResult.params.commonParams;
                        property int timeSpan:currentCheckResult.resultData.testTimespan;
                        property var alarmMode: ["不报警","只报警","报警并暂停"];
                        property var fixationViewSelection: ["中心点","小菱形","大菱形","底点"]
                        property var centerDotCheck: ["打开","关闭"];

                        model: [
                            {name:"眼动报警模式",param:alarmMode[params.eyeMoveAlarmMode]},
                            {name:"固视选择",param:fixationViewSelection[params.fixationViewSelection]},
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
                        property var strategy: ["全阈值","智能交互式","快速智能交互式","二区法","三区法","量化缺损","单刺激"]
                        model: [
                            {name:"刺激光标",param:cursorSize[params.cursorSize]+","+cursorColor[params.cursorColor]},
                            {name:"背景光",param:"31.5 ASB"},
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
            Column{width: parent.width*0.65;height:parent.height;/*spacing:height*0.035*/
                Row{width: parent.width;height:parent.height*0.97/3;
                    Item{width: parent.width*0.18;height: parent.height;}
                    Image {
                        height:parent.height;width:parent.height;cache: false;        //to refresh image
                        Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/temp/dbDiagram.bmp";})}
                    }
                    Item{width: parent.width*0.1;height: parent.height;}
                    Image {
                        height:parent.height;width:parent.height;cache: false;        //to refresh image
                        Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/temp/gray.bmp";})}
                    }
                    Item{
                        width:parent.width*0.72-parent.height*2;height:parent.height;
                        CusText{ width:parent.width;height:parent.height*0.15;verticalAlignment: Text.AlignVCenter;
                            text: "    ("+currentProgram.params.commonParams.Range[1]+")" ;
                            horizontalAlignment: Text.AlignLeft; anchors.left: parent.left; anchors.leftMargin: 0;anchors.verticalCenter: parent.verticalCenter}
                    }

                }

                Row{
                    width: parent.width;height:parent.height*0.97/3;spacing:width*0.05
                    Image {
                        height:parent.height;width:parent.height;cache: false;        //to refresh image
                        Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/temp/TotalDeviation.bmp";})}
                    }
                    Image {
                        height:parent.height; width:parent.height;cache: false;        //to refresh image
                        Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/temp/PatterDeviation.bmp";})}
                    }
                    Column {
                        height:parent.height;width:parent.width*0.9-parent.height*2
                        Item{width:parent.width;height:parent.height*0.2;}
                        CusText{width:parent.width;height:parent.height*0.12;text:"VFI:"+Math.round(analysisResult.VFI*100)+"%" ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
                        Item{width:parent.width;height:parent.height*0.1;}
                        CusText{
                        property var ght: if(analysisResult.GHT===0){return "超出界限";} else if(analysisResult.GHT===1){return "普遍敏感度降低";}else if(analysisResult.GHT===2){return "边界";}else if(analysisResult.GHT===3){return "正常范围";}
                        width:parent.width;height:parent.height*0.12;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40;text:"青光眼半视野检查: "+ ght;}
                        Item{width:parent.width;height:parent.height*0.1;}
                        CusText{width:parent.width;height:parent.height*0.12;text:"平均缺损: "+analysisResult.md.toFixed(2)+" (<"+analysisResult.p_md+"%)" ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
                        CusText{width:parent.width;height:parent.height*0.12;text:"模式标准偏差: "+analysisResult.psd.toFixed(2)+" (<"+analysisResult.p_psd+"%)" ;horizontalAlignment: Text.AlignLeft;font.pointSize: height*0.40}
                    }
                }
                Row{width: parent.width;height:parent.height*0.03;spacing:width*0.05;
                    CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "总体偏差"; verticalAlignment: Text.AlignVCenter; font.pointSize: height*0.50}
                    CusText{width:parent.parent.height*0.98/3;height: parent.height;text: "模式偏差";  verticalAlignment: Text.AlignVCenter;font.pointSize: height*0.50}
                }
                Row{
                    width: parent.width;height:parent.height*0.97/3;spacing:width*0.05
                    Image {
                        height:parent.height;width:parent.height;cache: false;z:-1;        //to refresh image
                        Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/temp/TotalPE.bmp";})}

                    }
                    Canvas
                    {
                        property var dot: null;
                        height:parent.height;width:parent.height;

                        function drawDot()
                        {
                            if(dot==null) return;
                            var pix=IcUiQmlApi.appCtrl.diagramProvider.getPixFromPoint(dot,width,height);
                            console.log(pix.x+" "+pix.y);
                            var dotRadius=width/25;
                            var ctx = getContext("2d");
                            ctx.lineWidth = 0;
                            ctx.strokeStyle = "blue";
                            ctx.beginPath();
                            ctx.arc(pix.x, pix.y, dotRadius, 0, Math.PI*2);
                            ctx.stroke();
                            ctx.closePath();
                        }
                        onPaint:
                        {
                            var ctx = getContext("2d")
                            ctx.clearRect(0, 0, width, height);
                            drawDot();
                        }


                        MouseArea
                        {
                            height:parent.height;width:parent.height;
                            onClicked: {
                                parent.dot=IcUiQmlApi.appCtrl.diagramProvider.getClickDot(mouseX,mouseY,width,height);
                                parent.requestPaint();}
                        }

                        Image {
                            height:parent.height;width:parent.height;cache: false;z:-1;        //to refresh image
                            Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + "/temp/PatternPE.bmp";})}
                        }
                    }

                    Item{
                        height: parent.height;width:parent.width*0.9-2*parent.height;
                        Column{
                            height: parent.height*0.5; anchors.bottom: parent.bottom; width:parent.width;spacing:height*0.08;
                            Repeater{
                                anchors.fill: parent;
                                model: [{image:"qrc:/grays/PE1.bmp",pe:"<5%"},{image:"qrc:/grays/PE2.bmp",pe:"<2%"},{image:"qrc:/grays/PE3.bmp",pe:"<1%"},{image:"qrc:/grays/PE4.bmp",pe:"<0.5%"}]
                                Row{
                                    height: parent.height*0.18;width: parent.width;spacing: height*0.5
                                    Image {height: sourceSize.height;width:sourceSize.width;anchors.verticalCenter: parent.verticalCenter;source: modelData.image}
                                    CusText{height: parent.height;text:modelData.pe; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: height*0.50}
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Rectangle{width:parent.width*0.25;height: parent.height;color:"white";}
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
