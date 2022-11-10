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
    property var progressAnalysisListVm: null;
    property var progressAnalysisResult: null;
    property int fontPointSize: CommonSettings.fontPointSize;
    property int textHeight:CommonSettings.textHeight;
    width: 1366;
    height: 660;
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
            Item{height: textHeight;width: parent.width*0.85*0.2;}
            Item{height: textHeight;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"灰度图"  ;}}
            Item{height: textHeight;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"阈值(DB)";}}
            Item{height: textHeight;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"进展差值";}}
            Item{height: textHeight;width: parent.parent.height*0.96*0.32;CusText{anchors.fill: parent;text:"进展分析";}}
        }
        Row{
            width: parent.width;height: parent.height*0.96;
            Column{
                width: parent.width*0.85;height: parent.height;spacing: parent.height*0.02;
                Repeater{
                    model: if(progressAnalysisListVm.selectedIndex>=4) return [{index_list:-2,index_progress:0},{index_list:-1,index_progress:1},{index_list:0,index_progress:2}];
                           else if(progressAnalysisListVm.selectedIndex===3) return [{index_list:-1,index_progress:0},{index_list:0,index_progress:1}];
                           else return [{index_list:0,index_progress:0}];
                    Row{
                        width: parent.width;height: parent.height*0.96/3;spacing: width*0.02;
                        Column{
                            width: parent.width*0.2;height: parent.height;spacing: textHeight*0.3;
                            Repeater{
                                property var centerDotCheck: ["打开","关闭"];
                                property var ght: ["超出界限","普遍敏感度降低","边界","正常范围"];
                                property var progress: ["没有进展","可能的进展","很有可能的进展"];
                                property int index: progressAnalysisListVm.selectedIndex;
                                model: [
                                    {name:"日期",param:Qt.formatDateTime(progressAnalysisListVm.getData(index+modelData.index_list,"dateTime"),"yyyy/MM/dd")},
                                    {name:"选择程序",param:progressAnalysisListVm.getData(index+modelData.index_list,"program")},
                                    {name:"青光眼半视野检查",param:ght[progressAnalysisListVm.getData(index+modelData.index_list,"GHT")]},
                                    {name:"中心点",param:centerDotCheck[progressAnalysisListVm.getData(index+modelData.index_list,"centerDotCheck")?1:0]},
                                    {name:"平均缺损",param:progressAnalysisListVm.getData(index+modelData.index_list,"md").toFixed(2)+(progressAnalysisListVm.getData(index+modelData.index_list,"p_md")>0?(" (<"+progressAnalysisListVm.getData(index+modelData.index_list,"p_md")+"%)"):"")},
                                    {name:"平均标准差",param:progressAnalysisListVm.getData(index+modelData.index_list,"psd").toFixed(2)+(progressAnalysisListVm.getData(index+modelData.index_list,"p_psd")>0?(" (<"+progressAnalysisListVm.getData(index+modelData.index_list,"p_psd")+"%)"):"")},
                                    {name:"固视丢失率",param:progressAnalysisListVm.getData(index+modelData.index_list,"fixationLostCount")+"/"+progressAnalysisListVm.getData(index+modelData.index_list,"fixationLostTestCount")},
                                    {name:"假阴性率",param:Math.round((progressAnalysisListVm.getData(index+modelData.index_list,"falseNegativeRate")*100))+"%"},
                                    {name:"假阳性率",param:Math.round((progressAnalysisListVm.getData(index+modelData.index_list,"falsePositiveRate")*100))+"%"},
                                    {name:"进展",param:progress[progressAnalysisResult[modelData.index_progress]]},
                                ]
                                CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                            }
                        }
                        Repeater{
                            model:["/previewImage/threeFollowUps_grey"+modelData.index_progress+".bmp","/previewImage/threeFollowUps_PatternPE"+modelData.index_progress+".bmp","/previewImage/threeFollowUps_progressVal"+modelData.index_progress+".bmp","/previewImage/threeFollowUps_progressPic"+modelData.index_progress+".bmp"];
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
                height: parent.height;spacing:textHeight;
                CusText{text:"基线平均缺损:"+((progressAnalysisListVm.getData(0,"md")+progressAnalysisListVm.getData(1,"md"))/2).toFixed(2); horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                CusText{text:"基线检查:"+Qt.formatDateTime(progressAnalysisListVm.getData(0,"dateTime"),"yyyy/MM/dd")+"  "+Qt.formatDateTime(progressAnalysisListVm==null?"":progressAnalysisListVm.getData(1,"dateTime"),"yyyy/MM/dd"); horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
                Row{
                    width: parent.width;height: parent.height*0.4
                    Column{
                        height: parent.height;  width:parent.width/2;spacing:height*0.08;
                        Repeater{
                            anchors.fill: parent;
                            model: [{image:"qrc:/grays/PE1.bmp",pe:"<5%"},{image:"qrc:/grays/PE2.bmp",pe:"<2%"},{image:"qrc:/grays/PE3.bmp",pe:"<1%"},{image:"qrc:/grays/PE4.bmp",pe:"<0.5%"}]
                            Row{
                                height:textHeight;width: parent.width;spacing: height*0.5
                                Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                                CusText{height: textHeight;text:modelData.pe; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter;}
                            }
                        }
                    }
                    Column{
                        height: parent.height;  width:parent.width/2;spacing:textHeight*0.5;
                        Repeater{
                            anchors.fill: parent;
                            model: [{image:"qrc:/grays/GPA1.bmp",progress:"<5% (恶化)"},{image:"qrc:/grays/GPA2.bmp",progress:"<5% (2连续)"},{image:"qrc:/grays/GPA3.bmp",progress:"<5% (3连续)"},{image:"qrc:/grays/GPA4.bmp",progress:"超出范围"}]
                            Row{
                                height:textHeight;width: parent.width;spacing: height*0.5
                                Image {height: sourceSize.height*2; fillMode: Image.PreserveAspectFit; width:sourceSize.height*2;smooth: false;source: modelData.image}
                                CusText{height: textHeight;text:modelData.progress; horizontalAlignment: Text.AlignLeft;anchors.verticalCenter: parent.verticalCenter}
                            }
                        }
                    }

                }
            }
        }
    }
}
