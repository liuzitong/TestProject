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
        console.log(progressAnalysisResult[2]);
    }
    Column
    {
        anchors.fill: parent;anchors.leftMargin: parent.width*0.04;anchors.rightMargin: parent.width*0.03;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;
        spacing: height*0.02
        Row{
            width: parent.width;height: parent.height*0.04;spacing: parent.width*0.85*0.02;
            Item{height: parent.height;width: parent.width*0.85*0.2;}
            Item{height: parent.height;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"灰度图";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"阈值(DB)";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"进展差值";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"进展分析";font.pixelSize:height*0.6}}
        }
        Row{
            width: parent.width;height: parent.height*0.96;
            Column{
                width: parent.width*0.85;height: parent.height;spacing: parent.height*0.02;
                Repeater{
                    model: [0,1,2];
                    Row{
                        width: parent.width;height: parent.height*0.96/3;spacing: width*0.02;
                        Column{
                            width: parent.width*0.2;height: parent.height;spacing: textHeight*-0.5;
                            Repeater{
                                property var centerDotCheck: ["打开","关闭"];
                                property var ght: ["超出界限","普遍敏感度降低","边界","正常范围"];
                                property var progress: ["没有进展","可能的进展","很有可能的进展"];
                                property int index: progressAnalysisListVm.selectedIndex;
                                model: [
                                    {name:"日期",param:progressAnalysisListVm.getData(index-2+modelData,"dateTime").split(' ')[0]},
                                    {name:"选择程序",param:progressAnalysisListVm.getData(index-2+modelData,"program")},
                                    {name:"青光眼半视野检查",param:ght[progressAnalysisListVm.getData(index-2+modelData,"GHT")]},
                                    {name:"中心点",param:centerDotCheck[progressAnalysisListVm.getData(index-2+modelData,"centerDotCheck")?1:0]},
                                    {name:"平均缺损",param:progressAnalysisListVm.getData(index-2+modelData,"md").toFixed(2)+(progressAnalysisListVm.getData(0,"p_md")>0?(" (<"+progressAnalysisListVm.getData(0,"p_md")+"%)"):"")},
                                    {name:"平均标准差",param:progressAnalysisListVm.getData(index-2+modelData,"psd").toFixed(2)+(progressAnalysisListVm.getData(0,"p_psd")>0?(" (<"+progressAnalysisListVm.getData(0,"p_psd")+"%)"):"")},
                                    {name:"固视丢失率",param:Math.round((progressAnalysisListVm.getData(index-2+modelData,"fixationLossRate")*100))+"%"},
                                    {name:"假阴性率",param:Math.round((progressAnalysisListVm.getData(index-2+modelData,"falseNegativeRate")*100))+"%"},
                                    {name:"假阳性率",param:Math.round((progressAnalysisListVm.getData(index-2+modelData,"falsePositiveRate")*100))+"%"},
                                    {name:"进展",param:progress[progressAnalysisResult[modelData]]},
                                ]
                                CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                            }
                        }
                        Repeater{
                            model:["/previewImage/threeFollowUps_grey"+modelData+".bmp","/previewImage/threeFollowUps_PatternPE"+modelData+".bmp","/previewImage/threeFollowUps_progressVal"+modelData+".bmp","/previewImage/threeFollowUps_progressPic"+modelData+".bmp"];
                            Image{
                               property string picSource: modelData;
                               height: sourceSize.height;width: sourceSize.width;
                               smooth: false;
                               cache: false;        //to refresh image
                               Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + picSource;})}
                            }
                        }
                    }
                }
            }
            Column
            {
                width: parent.width*0.15;
                height: parent.height;
                CusText{text:"基线平均缺损:"+((progressAnalysisListVm.getData(0,"md")+progressAnalysisListVm.getData(1,"md"))/2).toFixed(2); horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                CusText{text:"基线检查:"+progressAnalysisListVm.getData(0,"dateTime").split(' ')[0]+"  "+progressAnalysisListVm.getData(0,"dateTime").split(' ')[0]; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                Row{
                    width: parent.width;height: parent.height*0.4
                    Column{
                        height: parent.height*0.5;  width:parent.width/2;spacing:height*0.08;
                        Repeater{
                            anchors.fill: parent;
                            model: [{image:"qrc:/grays/PE1.bmp",pe:"<5%"},{image:"qrc:/grays/PE2.bmp",pe:"<2%"},{image:"qrc:/grays/PE3.bmp",pe:"<1%"},{image:"qrc:/grays/PE4.bmp",pe:"<0.5%"}]
                            Row{
                                height:childAt(0,0).height;width: parent.width;spacing: height*0.5
                                Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                                CusText{height: parent.height;text:modelData.pe; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: height*0.70}
                            }
                        }
                    }
                    Column{
                        height: parent.height*0.5;  width:parent.width/2;spacing:height*0.08;
                        Repeater{
                            anchors.fill: parent;
                            model: [{image:"qrc:/grays/GPA1.bmp",progress:"<5% (恶化)"},{image:"qrc:/grays/GPA2.bmp",progress:"<5% (2连续)"},{image:"qrc:/grays/GPA3.bmp",progress:"<5% (3连续)"},{image:"qrc:/grays/GPA4.bmp",progress:"超出范围"}]
                            Row{
                                height:childAt(0,0).height;width: parent.width;spacing: height*0.5
                                Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                                CusText{height: parent.height;text:modelData.progress; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;font.pointSize: height*0.70}
                            }
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
