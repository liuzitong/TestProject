#include "precompiled.h"
#include "databaseSvc.h"
#include "checkResult.h"
#include "patient.h"
#include "program.h"
//#include "perimeter/base/common/perimeter_memcntr.hxx"

namespace Perimeter {

void databaseSvc::initDataBase()
{
    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(R"(./test.sqlite)");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("");
    qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(true);
    qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(true);
    qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);
}

QObject* databaseSvc::getPatientById(QString id)
{
    qDebug()<<"getPatientById..";
    qDebug()<<id;
    List_Patient list_patient;
    qx_query query("select * from patient where patient_id=:id");
    query.bind(":id",id);
    QSqlError daoError = qx::dao::execute_query(query, list_patient);;

    std::cout<<"*****patient_list*****"<<std::endl;
    for(auto& i:list_patient)
    {
        qDebug()<<i->m_id;
        qDebug()<<i->m_name;
    }
    return static_cast<QObject*>(new PatientObj(list_patient.front()));
}

void databaseSvc::test()
{
    qDebug()<<"test test test..";
}

}
