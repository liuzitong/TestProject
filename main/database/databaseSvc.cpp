#include "precompiled.h"
#include "databaseSvc.h"
#include "checkResult.h"
#include "patient.h"
#include "program.h"
//#include <perimeter/main/model/patientListModel.h>
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include <QFile>
#include "qxpack/indcom/common/qxpack_ic_global.hxx"
#include <QDebug.h>


namespace Perimeter {

databaseSvc::databaseSvc()
{
    initDataBase();
}

databaseSvc::~databaseSvc()
{
}

void databaseSvc::initDataBase()
{
    QFileInfo fi((R"(./db.sqlite)"));
    bool isDbThere=fi.exists();
    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(R"(./db.sqlite)");
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

//QObject* databaseSvc::getPatientObjByPatientId(QString id)
//{
//    qDebug()<<"getPatientById..";
//    qDebug()<<id;
//    Patient_List Patient_List;
//    qx_query query("select * from patient where patient_id=:id");
//    query.bind(":id",id);
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);;

//    std::cout<<"*****patient_list*****"<<std::endl;
//    for(auto& i:Patient_List)
//    {
//        qDebug()<<i->m_id;
//        qDebug()<<i->m_name;
//    }
//    PatientVm * po=perimeter_new(PatientVm,(void*)Patient_List.front().get());
//    return po;
//}



//void databaseSvc::setPatientModel()
//{
//    Patient_List Patient_List;
//    qx_query query("select * from patient");
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);
//    qDebug()<<Patient_List.front()->m_name;
//    m_plm->setPatientList(Patient_List);
//    emit patientListChanged();
//}



//void databaseSvc::addPatient(QString patientId, QString name, int sex, QDate date,QDateTime updateTime)
//{
//    Patient_ptr patient_ptr(new Patient(patientId,name,Patient::sex(sex),date,updateTime));
//    QSqlError daoError = qx::dao::insert(patient_ptr);

//}

//void databaseSvc:: deletePatient(long id)
//{
//    QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
//    bool bCommit = db.transaction();
//    QSqlError daoError;qx_query query;
//    query.query("delete from Patient where id=:id");
//    query.bind(":id",(int)id);
//    daoError = qx::dao::call_query(query);
//    query.query("delete from CheckResult where patient_id=:id");
//    query.bind(":id",(int)id);
//    daoError = qx::dao::call_query(query);
//    bCommit = (bCommit && ! daoError.isValid());
//    qAssert(bCommit);
//    db.commit();
//}

//QObject* databaseSvc::getPatientByTimeSpan(QDate from,QDate to)
//{
//    if(from.toString()=="") from.setDate(1900,1,1);
//    if(to.toString()=="") to=QDate::currentDate();
//    qx_query query("select * from patient where lastUpdate>=:from and lastUpdate<=:to ORDER BY lastUpdate DESC");
//    QString fromStr=convertQDateToQString(from);
//    QString toStr=convertQDateToQString(to);
//    qDebug()<<QString("from:1%,to:2%").arg(fromStr).arg(toStr);
//    query.bind(":from",convertQDateToQString(from));
//    query.bind(":to",convertQDateToQString(to));
//    Patient_List Patient_List;
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);
////    m_plm->setPatientList(Patient_List);
//    return new PatientListModel(Patient_List);
//    emit patientListChanged();
//}

//QObject* databaseSvc::getPatientByPatientId(QString id)
//{
//    qx_query query("select * from patient where patientId=:patientId");
//    query.bind(":patientId",id);
//    Patient_List Patient_List;
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);
////    m_plm->setPatientList(Patient_List);
//    return new PatientListModel(Patient_List);
//    emit patientListChanged();
//}

//QObject* databaseSvc::getPatientByName(QString name, QDate from, QDate to)
//{
//    if(from.toString()=="") from.setDate(1900,1,1);
//    if(to.toString()=="") to.setDate(QDate::currentDate().year(),QDate::currentDate().month(),QDate::currentDate().day());
//    qx_query query("select * from patient where name=:name and lastUpdate>=:from and lastUpdate<=:to ORDER BY lastUpdate DESC");
//    query.bind(":name",name);query.bind(":from",convertQDateToQString(from));query.bind(":to",convertQDateToQString(to));
//    Patient_List Patient_List;
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);
////    m_plm->setPatientList(Patient_List);
//    return new PatientListModel(Patient_List);
//    emit patientListChanged();
//}

//QObject* databaseSvc::getPatientBySex(int sex, QDate from, QDate to)
//{
//    if(from.toString()=="") from.setDate(1900,1,1);
//    if(to.toString()=="") to=QDate::currentDate();
//    qx_query query("select * from patient where sex=:sex and lastUpdate>=:from and lastUpdate<=:to ORDER BY lastUpdate DESC");
//    query.bind(":sex",sex);
//    QString fromStr=convertQDateToQString(from);
//    QString toStr=convertQDateToQString(to);
//    qDebug()<<QString("from:1%,to:2%").arg(fromStr).arg(toStr);
//    query.bind(":from",convertQDateToQString(from));
//    query.bind(":to",convertQDateToQString(to));
//    Patient_List Patient_List;
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);
////    m_plm->setPatientList(Patient_List);
//    return new PatientListModel(Patient_List);
//    emit patientListChanged();
//}

//inline QString databaseSvc::convertQDateToQString(QDate date){
//    int year=date.year(),month=date.month(),day=date.day();
//    QString dateStr=QString("%1-%2-%3").arg(year).arg(month).arg(day);
//    return dateStr;
//}


//QObject* databaseSvc::getPatientByBirthDate(QDate date)
//{
//    qx_query query("select * from patient where birthDate=:birthDate ORDER BY lastUpdate DESC");
//    query.bind(":birthDate",convertQDateToQString(date));
//    Patient_List Patient_List;
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);
////    m_plm->setPatientList(Patient_List);
//    return new PatientListModel(Patient_List);
//    emit patientListChanged();
//}

void databaseSvc::addProgram(int type, QString name, QString params, QString data, int category)
{

    Program_ptr program_ptr(new Program(type,name,params,data,category));
    QSqlError daoError = qx::dao::insert(program_ptr);
}

QObject *databaseSvc::getParams()
{
    StaticParams* param=new StaticParams{{{3,2},false,100,StaticParams::CommonParams::Strategy::fullThreshold},{100,200}};
    StaticParamsVM* paramVM=new StaticParamsVM(*param);
    return paramVM;
}

//QObject *databaseSvc::getPatient()
//{
//    auto patient_ptr=new Patient();
//    patient_ptr->m_name="gauss dog";
//    patient_ptr->m_patientId="5001";
//    return new PatientVm(patient_ptr);
//}



//QObject *databaseSvc::getPatientListModel()
//{
//    return m_plm;
//}


//void databaseSvc::updatePatient(long id,QString patientId, QString name, int sex, QDate date)
//{
//    Patient_List Patient_List;
//    qx_query query("select * from patient where id=:id");
//    query.bind(":id",QString::number(id));
//    QSqlError daoError = qx::dao::execute_query(query, Patient_List);;
//    Patient_ptr pp = Patient_List.front();
//    pp->m_patientId=patientId;pp->m_name=name;pp->m_sex=Patient::sex(sex);pp->m_birthDate=date;pp->m_lastUpdate=QDateTime::currentDateTime();
//    daoError = qx::dao::update(pp);
//}

void databaseSvc::createData()
{
    QxPack::IcLCG  lcg;
    Patient_ptr patient_ptr,patient_1,patient_2;
    CheckResult_ptr checkResult_1,checkResult_2,checkResult_3,checkResult_4;
    Program_ptr program_1,program_2;
    program_1.reset(new Program);
    program_2.reset(new Program(2,2,"30-2","program2 params","program2 data"));

    program_1->m_id=1;
    program_1->m_data="program1data";
    program_1->m_name="24-2";
    program_1->m_type=3;
    program_1->m_params="ttt";
    program_1->m_data="bbbb";
    patient_1.reset(new Patient("5001","lzt",Patient::sex::male,QDate::fromString("1988-05-11","yyyy-MM-dd")));
    patient_2.reset(new Patient("5003","yangzhiqun",Patient::sex::female,QDate::fromString("1988-05-11","yyyy-MM-dd")));




    checkResult_1.reset(new CheckResult(1,"params1","data1",QDateTime::currentDateTime(),patient_1,program_1));
    checkResult_2.reset(new CheckResult(2,"params2","data2",QDateTime::currentDateTime(),patient_2,program_1));
    checkResult_3.reset(new CheckResult(3,"params3","data3",QDateTime::currentDateTime(),patient_1,program_2));
    checkResult_4.reset(new CheckResult(4,"params4","data4",QDateTime::currentDateTime(),patient_2,program_2));

    QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
    bool bCommit = db.transaction();

    QSqlError daoError;
    daoError = qx::dao::insert(program_1, &db);
    daoError = qx::dao::insert(program_2, &db);

    bCommit = (bCommit && ! daoError.isValid());
    qAssert(bCommit);
    db.commit();

    bCommit = db.transaction();
//    daoError = qx::dao::insert(patient_1, &db);
//    daoError = qx::dao::insert(patient_2, &db);
    for(int i=0;i<100;i++)
    {
        QDate date = QDate::currentDate();
        date=date.addYears(-(lcg.value()%80)-1).addMonths(-(lcg.value()%12)).addDays(-(lcg.value()%31));
        qDebug()<<QString("year:%1,month:%2,day:%3").arg(date.year()).arg(date.month()).arg(date.day());
        QDateTime time = QDateTime::currentDateTime();
        int year,month,day;
        year=-lcg.value()%(time.date().year()-date.year());
        month=-lcg.value()%12;
        day=-lcg.value()%31;
        time = time.addYears(year).addMonths(month).addDays(day);
        qDebug()<<QString("year:%1,month:%2,day:%3").arg(time.date().year()).arg(time.date().month()).arg(time.date().day());

        QString nm = QString("%1%2%3").arg( QChar( ( lcg.value() % 26 ) + 'A' ))
                         .arg( QChar( ( lcg.value() % 26 ) + 'a' ))
                         .arg( QChar( ( lcg.value() % 26 ) + 'a' ));
        QString id = QString("PID_%1").arg( lcg.value() % 10000 );
        patient_ptr.reset(new Patient(id ,nm,Patient::sex(lcg.value()%3),date,time));
        daoError = qx::dao::insert(patient_ptr, &db);
    }
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
