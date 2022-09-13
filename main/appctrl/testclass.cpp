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
    QImage img({600,300}, QImage::Format_RGB32);
    auto analysisMethodSvc=AnalysisSvc::getSingleton();
//    analysisMethodSvc->drawBaseLine({0,-1,-2,-4,-8,-7},80,{5,12,33,35,37,55},img);
//    img.save("./previewImage/baseLineDiagram.bmp");
    QVector<QPointF> locsbase1={{-9,27},{-3,21},{-3,3},{9,-9}};
    QVector<QPointF> locsbase2={{-9,27},{-3,21},{-3,3},{9,-9}};
    QVector<QPointF> locs2={{-9,27},{-3,21},{-3,3},{9,-9}};
    QVector<QPointF> locs3={{-9,27},{-3,21},       {9,-9}};
    QVector<QPointF> locs4={{-9,27},{-3,21},{-3,3},{9,-9}};
    QVector<int> valuesbase1={28,23,30,27};
    QVector<int> valuesbase2={28,23,30,27};
    QVector<int> values2=    {19,15,28,23};
    QVector<int> values3=    {10,10,   19};
    QVector<int> values4=    {1,  5,24,20};
    QVector<QVector<QPointF>> resultLocs;
    QVector<QVector<int>> resultVal;
    QVector<QVector<int>> resultPicVal;
    analysisMethodSvc->ProgressAnalysis({valuesbase1,valuesbase2,values2,values3,values4},{locsbase1,locsbase2,locs2,locs3,locs4},0,resultLocs,resultVal,resultPicVal);
    for(auto& i:resultLocs)
    {
        qDebug()<<i;
    }
    for(auto& i:resultVal)
    {
        qDebug()<<i;
    }
    for(auto& i:resultPicVal)
    {
        qDebug()<<i;
    }

}
}
