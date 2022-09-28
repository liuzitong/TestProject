/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtDataVisualization 1.0
import perimeter.main.view.Controls 1.0
import qxpack.indcom.ui_qml_base 1.0
import "."

Item {
    id: mainview
//    width: 1280
//    height: 720

    property real fontSize: 12

    property var heightMapSurfaceDataProxy1: null;
    property var heightMapSurfaceDataProxy2: null;
    property var heightMapSurfaceDataProxy3: null;


    signal refresh;
//    Image {
//        id: name
//        source: "file:///" + applicationDirPath + "/visionFieldIsland/layer_1.png"
//        height: sourceSize.height;width: sourceSize.width;
//        anchors.left: parent.left;anchors.top: parent.top;
//        z:1;
//    }
//    onRefresh: proxy1.heightMapFile=":/layers/layer_1.png";
//    onRefresh: proxy1.heightMapFile="file:///" + applicationDirPath + "/visionFieldIsland/layer_1.png";
//     onRefresh: proxy1.heightMapFile="file:///D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png";
    onRefresh: {
//        if(heightMapSurfaceDataProxy1==null) {
//            heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
//        }
//        heightMapSurfaceDataProxy1.heightMapFile=":/layers/layer_1.png";
//        layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
    }

    Component.onCompleted:
    {
        heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
        layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
//        heightMapSurfaceDataProxy2=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
//        heightMapSurfaceDataProxy3=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
//        heightMapSurfaceDataProxy1.heightMapFile=":/layers/layer_1.png";
//        heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
//        heightMapSurfaceDataProxy1.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png");
//        heightMapSurfaceDataProxy2=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
//        heightMapSurfaceDataProxy2.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_2.png");
//        heightMapSurfaceDataProxy3=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
//        heightMapSurfaceDataProxy3.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_3.png");
    }

//    onRefresh: console.log(applicationDirPath);



    Item {
        id: surfaceView
        width: mainview.width - buttonLayout.width
        height: mainview.height
        anchors.right: mainview.right;

        //! [0]
        ColorGradient {
            id: layerOneGradient
            ColorGradientStop { position: 0.0; color: "black" }
            ColorGradientStop { position: 0.31; color: "tan" }
            ColorGradientStop { position: 0.32; color: "green" }
            ColorGradientStop { position: 0.40; color: "darkslategray" }
            ColorGradientStop { position: 1.0; color: "white" }
        }

        ColorGradient {
            id: layerTwoGradient
            ColorGradientStop { position: 0.315; color: "blue" }
            ColorGradientStop { position: 0.33; color: "white" }
        }

        ColorGradient {
            id: layerThreeGradient
            ColorGradientStop { position: 0.0; color: "red" }
            ColorGradientStop { position: 0.15; color: "black" }
        }
        //! [0]

        Surface3D {
            id: surfaceLayers
            width: surfaceView.width
            height: surfaceView.height
            theme: Theme3D {
                type: Theme3D.ThemeEbony
                font.pointSize: 35
                colorStyle: Theme3D.ColorStyleRangeGradient
            }
            shadowQuality: AbstractGraph3D.ShadowQualityNone
            selectionMode: AbstractGraph3D.SelectionRow | AbstractGraph3D.SelectionSlice
            scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeft
            axisY.min: 20
            axisY.max: 200
            axisX.segmentCount: 5
            axisX.subSegmentCount: 2
            axisX.labelFormat: "%i"
            axisZ.segmentCount: 5
            axisZ.subSegmentCount: 2
            axisZ.labelFormat: "%i"
            axisY.segmentCount: 5
            axisY.subSegmentCount: 2
            axisY.labelFormat: "%i"

            //! [1]
            //! [2]



            Surface3DSeries {
                id: layerOneSeries
                baseGradient: layerOneGradient
                //! [2]
//                dataProxy:IcUiQmlApi.appCtrl.objMgr.attachObj("perimeter::QHeightMapSurfaceDataProxyWarpper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
//                dataProxy:heightMapSurfaceDataProxy1;
//                dataProxy:HeightMapSurfaceDataProxy {
//                    id:proxy1;
//                    heightMapFile: ":/layers/layer_1.png"
////                    heightMapFile: "file:///D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png";

//                }
                flatShadingEnabled: false
                drawMode: Surface3DSeries.DrawSurface
                //! [4]
                visible: layerOneToggle.checked // bind to checkbox state
                //! [4]
            }

//            Surface3DSeries {
//                id: layerTwoSeries
//                baseGradient: layerTwoGradient
//                HeightMapSurfaceDataProxy {
//                    heightMapFile: ":/layers/layer_2.png"
//                }
//                flatShadingEnabled: false
//                drawMode: Surface3DSeries.DrawSurface
//                visible: layerTwoToggle.checked // bind to checkbox state
//            }

//            Surface3DSeries {
//                id: layerThreeSeries
//                baseGradient: layerThreeGradient
//                HeightMapSurfaceDataProxy {
//                    heightMapFile: ":/layers/layer_3.png"
//                }
//                flatShadingEnabled: false
//                drawMode: Surface3DSeries.DrawSurface
//                visible: layerThreeToggle.checked // bind to checkbox state
//            }
            //! [1]
        }
    }

    ColumnLayout {
        id: buttonLayout
        anchors.top: parent.top
        anchors.left: parent.left
        spacing: 0

        //! [3]
        GroupBox {
            flat: true
            Layout.fillWidth: true
            Column {
                spacing: 10

                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Layer Selection"
                }

                CheckBox {
                    id: layerOneToggle
                    checked: true
                    style: CheckBoxStyle {
                        label: Label {
                            font.pointSize: fontSize
                            text: "Show Ground Layer"
                        }
                    }
                }

                CheckBox {
                    id: layerTwoToggle
                    checked: true
                    style: CheckBoxStyle {
                        label: Label {
                            font.pointSize: fontSize
                            text: "Show Sea Layer"
                        }
                    }
                }

                CheckBox {
                    id: layerThreeToggle
                    checked: true
                    style: CheckBoxStyle {
                        label: Label {
                            font.pointSize: fontSize
                            text: "Show Tectonic Layer"
                        }
                    }
                }
            }
        }
        //! [3]

        //! [5]
        GroupBox {
            flat: true
            Layout.fillWidth: true
            Column {
                spacing: 10

                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Layer Style"
                }

                CheckBox {
                    id: layerOneGrid
                    style: CheckBoxStyle {
                        label: Label {
                            font.pointSize: fontSize
                            text: "Show Ground as Grid"
                        }
                    }
                    onCheckedChanged: {
                        if (checked)
                            layerOneSeries.drawMode = Surface3DSeries.DrawWireframe
                        else
                            layerOneSeries.drawMode = Surface3DSeries.DrawSurface
                    }
                }

                CheckBox {
                    id: layerTwoGrid
                    style: CheckBoxStyle {
                        label: Label {
                            font.pointSize: fontSize
                            text: "Show Sea as Grid"
                        }
                    }
                    onCheckedChanged: {
                        if (checked)
                            layerTwoSeries.drawMode = Surface3DSeries.DrawWireframe
                        else
                            layerTwoSeries.drawMode = Surface3DSeries.DrawSurface
                    }
                }

                CheckBox {
                    id: layerThreeGrid
                    style: CheckBoxStyle {
                        label: Label {
                            font.pointSize: fontSize
                            text: "Show Tectonic as Grid"
                        }
                    }
                    onCheckedChanged: {
                        if (checked)
                            layerThreeSeries.drawMode = Surface3DSeries.DrawWireframe
                        else
                            layerThreeSeries.drawMode = Surface3DSeries.DrawSurface
                    }
                }
            }
        }
        //! [5]
        CusButton {
            text: "aa"
            fontSize: fontSize
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            property int index: 0;
            onClicked:
            {

//                  console.log("aa clicked");
//                  heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);

//                layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
//                if(index%3==0)
//                {
//                    console.log(0);
//                    heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
////                    heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
////                    heightMapSurfaceDataProxy1.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png");
//                    layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
//                }


//                if(index%3==1)
//                {
//                    console.log(1);
//                    heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_2.png"]);
////                    heightMapSurfaceDataProxy2=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png"]);
////                    heightMapSurfaceDataProxy2.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_2.png");
//                    layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
//                }
//                if(index%3==2)
//                {
//                    console.log(2);
//                    heightMapSurfaceDataProxy1=IcUiQmlApi.appCtrl.objMgr.attachObj("Perimeter::QHeightMapSurfaceDataProxyWrapper", false,["D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_3.png"]);
////                    heightMapSurfaceDataProxy3.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_2.png");
////                    layerOneSeries.dataProxy=heightMapSurfaceDataProxy3
//                    layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
//                }


                if(index%3==0)
                {
                    console.log(0);
                    layerOneSeries.dataProxy.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_1.png");
                    layerOneSeries.baseGradient=layerOneGradient;
//                    layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
                }


                if(index%3==1)
                {
                    console.log(1);
                    layerOneSeries.dataProxy.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_2.png");
                    layerOneSeries.baseGradient=layerTwoGradient;
//                    layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
                }
                if(index%3==2)
                {
                    console.log(2);
                    layerOneSeries.dataProxy.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_3.png");
                    layerOneSeries.baseGradient=layerThreeGradient;
                }
                index++;
//                if(index%3==2)
//                {
//                    heightMapSurfaceDataProxy1.setHeightMap("D:/perimeterProject/perimeter/bin/debug/visionFieldIsland/layer_3.png");
//                    layerOneSeries.dataProxy=heightMapSurfaceDataProxy1;
//                }

            }
        }
        //! [6]
        CusButton {
            id: sliceButton
            text: "Slice All Layers"
            fontSize: fontSize
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            onClicked: {
                if (surfaceLayers.selectionMode & AbstractGraph3D.SelectionMultiSeries) {
                    surfaceLayers.selectionMode = AbstractGraph3D.SelectionRow
                            | AbstractGraph3D.SelectionSlice
                    text = "Slice All Layers"
                } else {
                    surfaceLayers.selectionMode = AbstractGraph3D.SelectionRow
                            | AbstractGraph3D.SelectionSlice
                            | AbstractGraph3D.SelectionMultiSeries
                    text = "Slice One Layer"
                }
            }
        }
        //! [6]

        CusButton {
            id: shadowButton
            fontSize: fontSize
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            text: surfaceLayers.shadowsSupported ? "Show Shadows" : "Shadows not supported"
            enabled: surfaceLayers.shadowsSupported
            onClicked: {
                if (surfaceLayers.shadowQuality === AbstractGraph3D.ShadowQualityNone) {
                    surfaceLayers.shadowQuality = AbstractGraph3D.ShadowQualityLow
                    text = "Hide Shadows"
                } else {
                    surfaceLayers.shadowQuality = AbstractGraph3D.ShadowQualityNone
                    text = "Show Shadows"
                }
            }
        }

        CusButton {
            id: renderModeButton
            fontSize: fontSize
            text: "Switch Render Mode"
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            onClicked: {
                var modeText = "Indirect "
                var aaText
                if (surfaceLayers.renderingMode === AbstractGraph3D.RenderIndirect &&
                        surfaceLayers.msaaSamples === 0) {
                    surfaceLayers.renderingMode = AbstractGraph3D.RenderDirectToBackground
                    modeText = "BackGround "
                } else if (surfaceLayers.renderingMode === AbstractGraph3D.RenderIndirect &&
                           surfaceLayers.msaaSamples === 4) {
                    surfaceLayers.renderingMode = AbstractGraph3D.RenderIndirect
                    surfaceLayers.msaaSamples = 0
                } else if (surfaceLayers.renderingMode === AbstractGraph3D.RenderIndirect &&
                           surfaceLayers.msaaSamples === 8) {
                    surfaceLayers.renderingMode = AbstractGraph3D.RenderIndirect
                    surfaceLayers.msaaSamples = 4
                } else {
                    surfaceLayers.renderingMode = AbstractGraph3D.RenderIndirect
                    surfaceLayers.msaaSamples = 8
                }

                if (surfaceLayers.msaaSamples <= 0) {
                    aaText = "No AA"
                } else {
                    aaText = surfaceLayers.msaaSamples + "xMSAA"
                }

                renderLabel.text = modeText + aaText
            }
        }

        TextField {
            id: renderLabel
            font.pointSize: fontSize
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            enabled: false
            horizontalAlignment: TextInput.AlignHCenter
            text: "Indirect, " + surfaceLayers.msaaSamples + "xMSAA"
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
