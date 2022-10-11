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
//    createPatientData();
//    createCheckResultData();
    createDynamicCheckResultData();
//    createCheckResultVm();
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
    StaticCheckResultModel checkModel;
    checkModel.m_type=Type::ThreshHold;
    checkModel.m_OS_OD=0;
    checkModel.m_diagnosis="static";
    checkModel.m_time=QDateTime::currentDateTime();

    checkModel.m_params.commonParams={{1,2}};
    checkModel.m_params.fixedParams={1,2};
    checkModel.m_data.checkData={55,2};
    checkModel.m_data.alarm=0;
    checkModel.m_data.pupilDiameter=5.3;
    checkModel.m_data.testTimespan=100;
    checkModel.m_data.fixationDeviation={1,2,3,4};
    checkModel.m_data.falseNegativeCount=1;
    checkModel.m_data.checkData={2,3,4,5};
    checkModel.m_data.pic={{{"30","aaaa"},{"40","bbbbb"}},{{"30","aaaa"},{"40","bbbbb"}}};

    auto pp=checkModel.ModelToDB();
    qx::dao::insert(pp);

}

void TestClass::createDynamicCheckResultData()
{
    DynamicCheckResultModel checkModel;
    checkModel.m_type=Type::Dynamic;
    checkModel.m_OS_OD=0;
    checkModel.m_diagnosis="Dynamic";
    checkModel.m_time=QDateTime::currentDateTime();
    checkModel.m_params={{2,3},DynamicParams::Strategy::darkArea,CursorColor::white};
    checkModel.m_data.alarm=0;
    checkModel.m_data.pupilDiameter=5.3;
    checkModel.m_data.testTimespan=100;
    checkModel.m_data.fixationDeviation={1,2,3,4};
    checkModel.m_data.checkData={{"A",{50,45},{50,45},true},{"B",{50,135},{50,135},true},{"C",{50,225},{50,225},true},{"D",{50,315},{50,315},true}};

    auto pp=checkModel.ModelToDB();
    qx::dao::insert(pp);

}

void TestClass::createCheckResultVm()
{
    StaticCheckResultVm resultVm({1});;
    qDebug()<<resultVm.getResultData()->getCheckData()[2];

}

}
