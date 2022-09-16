import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.3
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import perimeter.main.view.Utils 1.0

Rectangle {
    id:root;
    anchors.fill: parent;
    color: "white";
    property int textHeight:height*0.05;
    property var progressAnalysisListVm: null;
    property var progressAnalysisResult: null;
    signal refresh();
    onRefresh: {
        console.log(progressAnalysisListVm.getData(0,"dateTime"));
        console.log(progressAnalysisResult.slopeType);
    }
    Column
    {
        anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.rightMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;
        Row{
            width: parent.width;height: parent.height*0.04;spacing: parent.width*0.03;
            Item{height: parent.height;width: parent.parent.height*0.32;}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"灰度图";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"阈值(DB)";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"总体偏差";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"模式偏差";font.pixelSize:height*0.6}}
        }

        Row{
            width: parent.width;height: parent.height*0.32;spacing: parent.width*0.03;
            Column{
                width: height;height: parent.height;spacing: textHeight*-0.4;
                Repeater{
                    property var centerDotCheck: ["打开","关闭"];
                    property var ght: ["超出界限","普遍敏感度降低","边界","正常范围"];
                    model: [
                        {name:"日期",param:progressAnalysisListVm.getData(0,"dateTime").split(' ')[0]},
                        {name:"选择程序",param:progressAnalysisListVm.getData(0,"program")},
                        {name:"青光眼半视野检查",param:ght[progressAnalysisListVm.getData(0,"GHT")]},
                        {name:"中心点",param:centerDotCheck[progressAnalysisListVm.getData(0,"centerDotCheck")?1:0]},
                        {name:"平均缺损",param:progressAnalysisListVm.getData(0,"md").toFixed(2)+(progressAnalysisListVm.getData(0,"p_md")>0?(" (<"+progressAnalysisListVm.getData(0,"p_md")+"%)"):"")},
                        {name:"平均标准差",param:progressAnalysisListVm.getData(0,"psd").toFixed(2)+(progressAnalysisListVm.getData(0,"p_psd")>0?(" (<"+progressAnalysisListVm.getData(0,"p_psd")+"%)"):"")},
                        {name:"固视丢失率",param:Math.round((progressAnalysisListVm.getData(0,"fixationLossRate")*100))+"%"},
                        {name:"假阴性率",param:Math.round((progressAnalysisListVm.getData(0,"falseNegativeRate")*100))+"%"},
                        {name:"假阳性率",param:Math.round((progressAnalysisListVm.getData(0,"falsePositiveRate")*100))+"%"},
                    ]
                   CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                }

            }
            Repeater{
                model:["/previewImage/baseLine_gray0.bmp","/previewImage/baseLine_dBDiagram0.bmp","/previewImage/baseLine_TotalPE0.bmp","/previewImage/baseLine_PatternPE0.bmp"];
                Image{
                   property string picSource: modelData;
                   height: sourceSize.height;width: sourceSize.width;
                   smooth: false;
                   cache: false;        //to refresh image
                   Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + picSource;})}
                }
            }
            Item{
                height: parent.height;width:parent.height*0.5;
                Column{
                    height: parent.height*0.5; anchors.bottom: parent.bottom; width:parent.width;spacing:height*0.08;
                    Repeater{
                        anchors.fill: parent;
                        model: [{image:"qrc:/grays/PE1.bmp",pe:"<5%"},{image:"qrc:/grays/PE2.bmp",pe:"<2%"},{image:"qrc:/grays/PE3.bmp",pe:"<1%"},{image:"qrc:/grays/PE4.bmp",pe:"<0.5%"}]
                        Row{
                            height: parent.height*0.14;width: parent.width;spacing: height*0.5
                            Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                            CusText{height: parent.height;text:modelData.pe; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: height*0.70}
                        }
                    }
                }
            }
        }
        Item{width: parent.width;height: parent.height*0.01;}
        Row{
            width: parent.width;height: parent.height*0.32;spacing: parent.width*0.03;
            Column{
                width: height;height: parent.height;spacing: textHeight*-0.4;
                Repeater{
                    property var centerDotCheck: ["打开","关闭"];
                    property var ght: ["超出界限","普遍敏感度降低","边界","正常范围"];
                    model: [
                        {name:"日期",param:progressAnalysisListVm.getData(1,"dateTime").split(' ')[0]},
                        {name:"选择程序",param:progressAnalysisListVm.getData(1,"program")},
                        {name:"青光眼半视野检查",param:ght[progressAnalysisListVm.getData(1,"GHT")]},
                        {name:"中心点",param:centerDotCheck[progressAnalysisListVm.getData(1,"centerDotCheck")?1:0]},
                        {name:"平均缺损",param:progressAnalysisListVm.getData(1,"md").toFixed(2)+(progressAnalysisListVm.getData(1,"p_md")>0?(" (<"+progressAnalysisListVm.getData(0,"p_md")+"%)"):"")},
                        {name:"平均标准差",param:progressAnalysisListVm.getData(1,"psd").toFixed(2)+(progressAnalysisListVm.getData(1,"p_psd")>0?(" (<"+progressAnalysisListVm.getData(0,"p_psd")+"%)"):"")},
                        {name:"固视丢失率",param:Math.round((progressAnalysisListVm.getData(1,"fixationLossRate")*100))+"%"},
                        {name:"假阴性率",param:Math.round((progressAnalysisListVm.getData(1,"falseNegativeRate")*100))+"%"},
                        {name:"假阳性率",param:Math.round((progressAnalysisListVm.getData(1,"falsePositiveRate")*100))+"%"},
                    ]
                   CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                }

            }
            Repeater{
                model:["/previewImage/baseLine_gray1.bmp","/previewImage/baseLine_dBDiagram1.bmp","/previewImage/baseLine_TotalPE1.bmp","/previewImage/baseLine_PatternPE1.bmp"];
                Image{
                   property string picSource: modelData;
                   height: sourceSize.height;width: sourceSize.width;
                   smooth: false;
                   cache: false;        //to refresh image
                   Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + picSource;})}
                }
            }
        }
        Item{width: parent.width;height: parent.height*0.01;}
        Row{
            width: parent.width;height: parent.height*0.30;spacing: parent.width*0.03;
            Column{
                width: height;height: parent.parent.height*0.32;spacing: textHeight*-0.4;
                Repeater{
                    property var slopeType: ["斜率不明显","斜率明显"];
                    model: [
                        {name:"进展速度",param:progressAnalysisResult.progressSpeedBase.toFixed(2)+"±"+progressAnalysisResult.progressSpeedDeviation.toFixed(2)+" DB/YEAR"},
                        {name:"基线平均缺损",param:((progressAnalysisListVm.getData(0,"md")+progressAnalysisListVm.getData(1,"md"))/2).toFixed(2)},
                        {name:"斜率情况",param:slopeType[progressAnalysisResult.slopeType]},
                    ]
                   CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                }

            }
            ScrollView
            {
                width: 600+18;height: parent.height;
                Image{
                    property string picSource: "/previewImage/baseLine.bmp"
                    height: sourceSize.height;width: sourceSize.width;
                    smooth: false; cache: false;
                    Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + picSource;})}
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
