#include "testclass.h"

#include <QJsonArray>
#include <qfile.h>
#include <qjsondocument.h>
#include <QJsonObject>
#include <perimeter/main/model/checkResultModel.h>
#include <perimeter/main/viewModel/checkResultVm.h>

namespace Perimeter {
void TestClass::test()
{
//    qDebug()<<"hello world";
//    QFile jsonFile("./data.json");
//    if( !jsonFile.open(QIODevice::ReadOnly)) {qDebug() << "read file error!";return;}
//    QJsonParseError jsonParserError;
//    QJsonDocument outDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
//    QJsonArray jsonArray=outDoc.array();
//    for(auto i:jsonArray)
//    {
//        auto jo=i.toObject();
//        qDebug()<<jo["name"].toString();
//        auto arr=jo["data"].toArray();
//        QString str;
//        for(auto j:arr)
//        {
//            str+=QString::number(j.toInt())+" ";
//        }
//        qDebug()<<str;

//    }
//    jsonFile.close();

//    using Params=StaticParams::CommonParams;
//    CheckResultModel<Type::ThreshHold> checkResultModel;
//    checkResultModel.m_type=Type::ThreshHold;
//    checkResultModel.m_params={{{15,30},Params::Strategy::fullThreshold},{}};
//    checkResultModel.m_data={0,0,0,0,0,0,0,0,0,0,0,{1,2,3},{2,2,4,5},{27,26,27,27, 27,31,30,31,30,30, 27,31,31,30,27,30,32,27, 28,27,27,30,30,31,29,32,29,24, 29,26,0,33,33,31,33,33,33,21, 29,13,0,29,34,34,33,33,32,29, 26,24,27,31,32,34,33,30,31,27, 29,31,33,31,30,32,29,30, 28,28,29,30,32,28, 29,27,27,28}};
//    checkResultModel.m_time=QDateTime::currentDateTime();
//    checkResultModel.m_patient_id=1;
//    checkResultModel.m_program_id=1;

//    auto ptr=checkResultModel.ModelToDB();

//    qx::dao::insert(ptr);

    auto checkresultvm=new CheckResultVm({3});
    qDebug()<<checkresultvm->getType();
    qDebug()<<checkresultvm->getOS_OD();
    auto data=checkresultvm->getResultData()->getCheckData();
    for(auto &i:data)
    {
        qDebug()<<i.toInt();
    }

}
}
