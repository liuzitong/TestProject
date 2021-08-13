#include "precompiled.h"
#include "databaseSvc.h"
#include "checkResult.h"
#include "patient.h"
#include "program.h"
#include "patientListModel.h"
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include <QMessageBox>
#include <QFile>

namespace Perimeter {

void databaseSvc::initDataBase()
{
    QFileInfo fi((R"(./test.sqlite)"));
    bool isDbThere=fi.exists();
    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(R"(./test.sqlite)");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("");
    qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(true);
    qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(true);
    qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);
    if(!isDbThere)
        createTable();
}


void databaseSvc::createTable(){
    // Create all tables in database
    QSqlError daoError = qx::dao::create_table<Patient>();
    daoError = qx::dao::create_table<CheckResult>();
    daoError = qx::dao::create_table<Program>();
}

QObject* databaseSvc::getPatientObjByPatientId(QString id)
{
    qDebug()<<"getPatientById..";
    qDebug()<<id;
    Patient_List Patient_List;
    qx_query query("select * from patient where patient_id=:id");
    query.bind(":id",id);
    QSqlError daoError = qx::dao::execute_query(query, Patient_List);;

    std::cout<<"*****patient_list*****"<<std::endl;
    for(auto& i:Patient_List)
    {
        qDebug()<<i->m_id;
        qDebug()<<i->m_name;
    }
    PatientVm * po=perimeter_new(PatientVm,Patient_List.front());
    return po;
}


void databaseSvc::test()
{
    qDebug()<<"test test test..";
}

QObject* databaseSvc::getPatientModel()
{
    Patient_List Patient_List;
    qx_query query("select * from patient");
    QSqlError daoError = qx::dao::execute_query(query, Patient_List);;
    PatientListModel* plm=perimeter_new(PatientListModel);
    plm->setPatientList(Patient_List);
    return plm;
}

void databaseSvc::addPatient(QString patientId, QString name, int sex, QDate date)
{
    Patient_ptr patient_ptr(new Patient(patientId,name,Patient::sex(sex),date));
//    patient->m_id=id;
//    patient->m_name=name;
//    pat
     QSqlError daoError = qx::dao::insert(patient_ptr);

}

void databaseSvc::updatePatient(long id,QString patientId, QString name, int sex, QDate date)
{
    Patient_List Patient_List;
    qx_query query("select * from patient where id=:id");
    query.bind(":id",QString::number(id));
    QSqlError daoError = qx::dao::execute_query(query, Patient_List);;
    Patient_ptr pp = Patient_List.front();
    pp->m_patinetId=patientId;pp->m_name=name;pp->m_sex=Patient::sex(sex);pp->m_birthDate=date;
    daoError = qx::dao::update(pp);
}

void databaseSvc::createData()
{

    Patient_ptr patient_1,patient_2;
    CheckResult_ptr checkResult_1,checkResult_2,checkResult_3,checkResult_4;
    Program_ptr program_1,program_2;
    program_1.reset(new Program);
    program_2.reset(new Program(2,true,Program::Type::Screening,"30-2","program2 params","program2 data"));

    program_1->m_id=1;
    program_1->m_data="program1data";
    program_1->m_name="24-2";
    program_1->m_type=Program::Type::ThreshHold;
    program_1->m_params="ttt";
    program_1->m_data="bbbb";

    patient_1.reset(new Patient("5001","lzt",Patient::sex::male,QDate::currentDate()));
    patient_2.reset(new Patient("5003","yangzhiqun",Patient::sex::female,QDate::currentDate()));


    checkResult_1.reset(new CheckResult(1,CheckResult::Strategy::strategy1,"params1","data1",QDateTime::currentDateTime(),patient_1,program_1));
    checkResult_2.reset(new CheckResult(2,CheckResult::Strategy::strategy1,"params2","data2",QDateTime::currentDateTime(),patient_2,program_1));
    checkResult_3.reset(new CheckResult(3,CheckResult::Strategy::strategy2,"params3","data3",QDateTime::currentDateTime(),patient_1,program_2));
    checkResult_4.reset(new CheckResult(4,CheckResult::Strategy::strategy3,"params4","data4",QDateTime::currentDateTime(),patient_2,program_2));

    QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
    bool bCommit = db.transaction();

    QSqlError daoError;
    daoError = qx::dao::insert(program_1, &db);
    daoError = qx::dao::insert(program_2, &db);
    bCommit = (bCommit && ! daoError.isValid());
    qAssert(bCommit);
    db.commit();

    bCommit = db.transaction();
    daoError = qx::dao::insert(patient_1, &db);
    daoError = qx::dao::insert(patient_2, &db);
    bCommit = (bCommit && ! daoError.isValid());
    qAssert(bCommit);
    db.commit();

    bCommit = db.transaction();
    daoError = qx::dao::insert(checkResult_1, &db);
    daoError = qx::dao::insert(checkResult_2, &db);
    daoError = qx::dao::insert(checkResult_3, &db);
    daoError = qx::dao::insert(checkResult_4, &db);
    bCommit = (bCommit && ! daoError.isValid());
    qAssert(bCommit);
    db.commit();
}

}
