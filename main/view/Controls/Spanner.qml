import QtQuick 2.7
Item{
    id: spannerRoot
    width: 30
    height: titleText.height + viewRect.height
    property int itemCount: 3
    property int viewModel: 0
    property int viewModelBase: 0
    property int dataLength: 0
    property string title: ""
    property string currentText: view.currentItem.text
    signal movementEnded

    Text{
        id: titleText
        width: parent.width
        height: 20
        color: "white"
        text: title
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Item{
        id: viewRect
        anchors.top: titleText.bottom
        width: parent.width
        height: itemCount * 30

        ListView{
            id: view
            anchors.fill: parent
            clip: true          //去除在拖动时顶部和底部多显示的一个item
            preferredHighlightBegin: highlightItem.height
            preferredHighlightEnd: 2 * highlightItem.height
            highlightRangeMode: ListView.StrictlyEnforceRange
            model: viewModel
            delegate: Text{
                width: parent.width
                height: 30
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                //设置不够位数左边用0补齐
                text: (new Array(dataLength).join('0') + (viewModelBase + index)).slice(-dataLength)
                color: "white"
            }
            highlight: Rectangle{
                color: "lightgreen"
            }
            onMovementEnded: spannerRoot.movementEnded(); //当滚动结束时将信号导出
        }
        //添加一个渐变色
        Rectangle{
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "transparent";
                }
                GradientStop {
                    position: 0.329;
                    color: "red";
                }
                GradientStop {
                    position: 0.33;
                    color: "transparent";
                }
                GradientStop {
                    position: 0.67;
                    color: "transparent";
                }
                GradientStop {
                    position: 0.671;
                    color: "red";
                }
                GradientStop {
                    position: 1.00;
                    color: "transparent";
                }
            }
        }
    }
    //根据index设置ListView的当前item
    function setCurrentIndex(index){
        view.currentIndex = index;
    }
}
