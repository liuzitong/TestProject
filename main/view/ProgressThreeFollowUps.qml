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
        anchors.fill: parent;anchors.leftMargin: parent.width*0.06;anchors.rightMargin: parent.width*0.06;anchors.topMargin: parent.height*0.04;anchors.bottomMargin: parent.height*0.04;
        Row{
            width: parent.width;height: parent.height*0.04;spacing: parent.width*0.03;
            Item{height: parent.height;width: parent.parent.height*0.32;}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"灰度图";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"阈值(DB)";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"进展差值";font.pixelSize:height*0.6}}
            Item{height: parent.height;width: parent.parent.height*0.32;CusText{anchors.fill: parent;text:"进展分析";font.pixelSize:height*0.6}}
        }
//        Repeater{
//            width: parent.width;height: parent.height*0.31*3;
//            model: [0,1];
//            Row{
//                width: parent.width;height: parent.height*0.31;spacing: parent.width*0.03;
//                Column{
//                    width: height;height: parent.height;spacing: textHeight*-0.4;
//                    Repeater{
//                        property var centerDotCheck: ["打开","关闭"];
//                        property var ght: ["超出界限","普遍敏感度降低","边界","正常范围"];
//                        property var progress: ["没有进展","可能的进展","很有可能的进展"];
//                        model: [
//                            {name:"日期",param:progressAnalysisListVm.getData(modelData,"dateTime").split(' ')[0]},
//                            {name:"选择程序",param:progressAnalysisListVm.getData(modelData,"program")},
//                            {name:"青光眼半视野检查",param:ght[progressAnalysisListVm.getData(modelData,"GHT")]},
//                            {name:"中心点",param:centerDotCheck[progressAnalysisListVm.getData(modelData,"centerDotCheck")?1:0]},
//                            {name:"平均缺损",param:progressAnalysisListVm.getData(modelData,"md").toFixed(2)+(progressAnalysisListVm.getData(0,"p_md")>0?(" (<"+progressAnalysisListVm.getData(0,"p_md")+"%)"):"")},
//                            {name:"平均标准差",param:progressAnalysisListVm.getData(modelData,"psd").toFixed(2)+(progressAnalysisListVm.getData(0,"p_psd")>0?(" (<"+progressAnalysisListVm.getData(0,"p_psd")+"%)"):"")},
//                            {name:"固视丢失率",param:Math.round((progressAnalysisListVm.getData(modelData,"fixationLossRate")*100))+"%"},
//                            {name:"假阴性率",param:Math.round((progressAnalysisListVm.getData(modelData,"falseNegativeRate")*100))+"%"},
//                            {name:"假阳性率",param:Math.round((progressAnalysisListVm.getData(modelData,"falsePositiveRate")*100))+"%"},
////                            {name:"进展",param:progress[progressAnalysisResult[modelData]]},
//                        ]
//                       CusText{text:modelData.name+":  "+modelData.param; horizontalAlignment: Text.AlignLeft;height:textHeight;width: parent.width;}
//                    }

//                }
////                Repeater{
////                    model:["/previewImage/threeFollowUps_grey"+modelData+".bmp","/previewImage/baseLine_PatternPE"+modelData+".bmp"];
////                    Image{
////                       property string picSource: modelData;
////                       height: sourceSize.height;width: sourceSize.width;
////                       smooth: false;
////                       cache: false;        //to refresh image
////                       Component.onCompleted: {root.refresh.connect(function(){source="";source="file:///" + applicationDirPath + picSource;})}
////                    }
////                }
//            }
//            Item{width: parent.width;height: parent.height*0.01;}
//        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
