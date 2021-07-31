import QtQuick 2.0

Item {
    width: dateEditRow.width
    height: dateEditRow.height

    property var dataLengthArray: [4,2,2]
    property var titleArray: ["年","月","日"]
    property var modelArray: [14,12,31] //显示model范围
    property var modelBaseArray: [year_base(modelArray[0]) + 1,1,1]//显示起点
    property string curDate: ""

    Row{
        id: dateEditRow
        spacing: 3
        Repeater{
            id: spannerRepeater
            model: 3
            Spanner{
                itemCount: 3                      //显示的item个数
                dataLength: dataLengthArray[index]//数据是几位，不够的用0在左边补齐
                title: titleArray[index]          //上面的标题
                viewModel: modelArray[index]      //总共可滚动的项目数
                viewModelBase: modelBaseArray[index] //从哪里开始
            }
            onItemAdded: {
                item.movementEnded.connect(autoCorrectDate); //绑定滚动结束的函数
            }
        }
    }

    Component.onCompleted: {
        //初始化显示当前日期
        var currentDate = new Date();
        spannerRepeater.itemAt(0).setCurrentIndex(modelArray[0] - 1);
        spannerRepeater.itemAt(1).setCurrentIndex(currentDate.getMonth());
        spannerRepeater.itemAt(2).setCurrentIndex(currentDate.getDate() - 1);
    }

    function year_base(modelCount){
        var curDate = new Date();
        var yearBase;

        if(modelArray && modelArray.length > 0)
            yearBase = curDate.getFullYear() - modelCount;
        else
            yearBase = curDate.getFullYear() - 10;

        return yearBase;
    }

    function autoCorrectDate(){
        var year = Number(spannerRepeater.itemAt(0).currentText);
        var month = Number(spannerRepeater.itemAt(1).currentText);
        var day = Number(spannerRepeater.itemAt(2).currentText);
        var monthType = month_type(month);
        switch (monthType){
        case 1:
            break;
        case 2:
            if(is_runnian(year)){
                if(day > 29)
                    spannerRepeater.itemAt(2).setCurrentIndex(modelArray[2] - 3); //最大31天润年2月最大29天，31-29+1=3
            }else{
                if(day > 28)
                    spannerRepeater.itemAt(2).setCurrentIndex(modelArray[2] - 4); //最大31天平年2月最大28天，31-28+1=4
            }
            break;
        case 3:
            if(day > 30){
                spannerRepeater.itemAt(2).setCurrentIndex(modelArray[2] - 2); //最大31天小月30天，31-30+1=2
            }
        }

        curDate = spannerRepeater.itemAt(0).currentText + '-' + spannerRepeater.itemAt(1).currentText + '-' + spannerRepeater.itemAt(2).currentText;
    }

    function is_runnian(year){
        var cond1 = year % 4 == 0;  //条件1：年份必须要能被4整除
        var cond2 = year % 100 != 0;  //条件2：年份不能是整百数
        var cond3 = year % 400 ==0;  //条件3：年份是400的倍数
        //当条件1和条件2同时成立时，就肯定是闰年，所以条件1和条件2之间为“与”的关系。
        //如果条件1和条件2不能同时成立，但如果条件3能成立，则仍然是闰年。所以条件3与前2项为“或”的关系。
        //所以得出判断闰年的表达式：
        var cond = cond1 && cond2 || cond3;
        if(cond) {
            return true;
        } else {
            return false;
        }
    }

    function month_type(month){
        if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
            return 1;
        else if(month == 2)
            return 2;
        else
            return 3;
    }
}
