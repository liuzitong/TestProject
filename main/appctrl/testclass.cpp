#include "testclass.h"

#include <QJsonArray>
#include <qfile.h>
#include <qjsondocument.h>
#include <QJsonObject>
#include <perimeter/main/model/checkResultModel.h>
#include <perimeter/main/viewModel/checkResultVm.h>
#include "perimeter/main/viewModel/settings.h"
#include "perimeter/main/Model/Rx.h"
#include "perimeter/main/Model/utility.h"
#include "perimeter/main/viewModel/progressAnalysislistVm.h"
#include <perimeter/main/services/analysis_svc.h>
#include "qxpack/indcom/common/qxpack_ic_global.hxx"


namespace Perimeter {
void TestClass::test()
{

//    using Params=StaticParams::CommonParams;
//    CheckResultModel<Type::Dynamic> checkResultModel;
//    checkResultModel.m_type=Type::Dynamic;
//    checkResultModel.m_OS_OD=0;
//    checkResultModel.m_params={{}};
//    checkResultModel.m_data={0,2,10,4,10,3,10,3,6,100,50,30,30,{4,8,10,11,15},{{-9,30},{30,20},{30,-10},{-20,-10}}};
//    checkResultModel.m_time=QDateTime::currentDateTime();
//    checkResultModel.m_patient_id=1;
//    checkResultModel.m_program_id=1;
//    auto ptr=checkResultModel.ModelToDB();

//    qx::dao::insert(ptr);

//    auto pp=new Patient;
//    Rx rx={0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
//    pp->m_rx=Utility::entityToQString(rx);
//    Patient_ptr ppp(pp);
//    qx::dao::insert(ppp);

//    auto tt=new ProgressAnalysisListVm(QVariantList{1});
//    delete tt;
//    QImage img({600,300}, QImage::Format_RGB32);
//    auto analysisMethodSvc=AnalysisSvc::getSingleton();
////    analysisMethodSvc->drawBaseLine({0,-1,-2,-4,-8,-7},80,{5,12,33,35,37,55},img);
////    img.save("./previewImage/baseLineDiagram.bmp");
//    QVector<QPointF> locsbase1={{-9,27},{-3,21},{-3,3},{9,-9}};
//    QVector<QPointF> locsbase2={{-9,27},{-3,21},{-3,3},{9,-9}};
//    QVector<QPointF> locs2={{-9,27},{-3,21},{-3,3},{9,-9}};
//    QVector<QPointF> locs3={{-9,27},{-3,21},       {9,-9}};
//    QVector<QPointF> locs4={{-9,27},{-3,21},{-3,3},{9,-9}};
//    QVector<int> valuesbase1={28,23,30,27};
//    QVector<int> valuesbase2={28,23,30,27};
//    QVector<int> values2=    {19,15,28,23};
//    QVector<int> values3=    {10,10,   19};
//    QVector<int> values4=    {1,  5,24,20};
//    QVector<int> progress;
//    QVector<QVector<QPointF>> resultLocs;
//    QVector<QVector<int>> resultVal;
//    QVector<QVector<int>> resultPicVal;
//    analysisMethodSvc->ProgressAnalysis({valuesbase1,valuesbase2,values2,values3,values4},{locsbase1,locsbase2,locs2,locs3,locs4},0,resultLocs,resultVal,resultPicVal,progress);
//    for(auto& i:resultLocs)
//    {
//        qDebug()<<i;
//    }
//    for(auto& i:resultVal)
//    {
//        qDebug()<<i;
//    }
//    for(auto& i:resultPicVal)
//    {
//        qDebug()<<i;
//    }

//    createPatientData();
    createCheckResultData();
}

void TestClass::createPatientData()
{

    QxPack::IcLCG  lcg;
    Patient_ptr patient_1;
//    QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
//    bool bCommit = db.transaction();
    QList<Patient_ptr> pplist;
    for(int i=0;i<2000;i++)
    {
        Patient_ptr patient_ptr;
        QDateTime time = QDateTime::currentDateTime();
        time = time.addDays(-lcg.value()%365);
        time = time.addMonths(-lcg.value()%12);
        time = time.addYears(-lcg.value()%80);
        QString nm = QString("%1%2%3").arg( QChar( ( lcg.value() % 26 ) + 'A' ))
                         .arg( QChar( ( lcg.value() % 26 ) + 'a' ))
                         .arg( QChar( ( lcg.value() % 26 ) + 'a' ));
        QString id = QString("PID_%1").arg( lcg.value() % 5000 );


    //        Patient::sex sex=Patient::sex::male;
    //        pp.reset(new Patient("998","fdsfds",sex,QDate::fromString("1988-05-11","yyyy-MM-dd")));
        auto pp=new Patient(id,nm,sex(lcg.value()%2),time.date(),
                  R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
                    <!DOCTYPE boost_serialization>
                    <boost_serialization signature="serialization::archive" version="19">
                    <t class_id="0" tracking_level="0" version="0">
                        <rx1_r>6.000000238e-01</rx1_r>
                        <rx2_r>6.999999881e-01</rx2_r>
                        <rx3_r>8.000000119e-01</rx3_r>
                        <visual_r>8.999999762e-01</visual_r>
                        <rx1_l>2.000000030e-01</rx1_l>
                        <rx2_l>3.000000119e-01</rx2_l>
                        <rx3_l>4.000000060e-01</rx3_l>
                        <visual_l>5.000000000e-01</visual_l>
                    </t>
                    </boost_serialization>)"
                    );
        pplist.append(patient_ptr);
    }
//    qDebug()<<list.length();
//    qx::dao::insert(list);
    qx::dao::insert(pplist);

    //    db.commit();
}

void TestClass::createCheckResultData()
{
    CheckResultModel<Type::ThreshHold> checkModel;
    checkModel.m_params.commonParams={{1,2}};
    checkModel.m_params.fixedParams={1,2};
    checkModel.m_data={1,2,3};
    checkModel.m_data.checkdata={1,2,3,4};
    checkModel.m_pic={{{"30","aaaa"},{"40","bbbbb"}},{{"30","aaaa"},{"40","bbbbb"}}};
    auto pp=checkModel.ModelToDB();
    qx::dao::insert(pp);

}

void TestClass::createDynamicCheckResultData()
{
    CheckResultModel<Type::ThreshHold> checkModel;
    checkModel.m_params.commonParams={{1,2}};
    checkModel.m_params.fixedParams={1,2};
    checkModel.m_data={1,2,3};
    checkModel.m_data.checkdata={1,2,3,4};
    checkModel.m_pic={{{"30","aaaa"},{"40","bbbbb"}},{{"30","aaaa"},{"40","bbbbb"}}};
    auto pp=checkModel.ModelToDB();
    qx::dao::insert(pp);

}


}
