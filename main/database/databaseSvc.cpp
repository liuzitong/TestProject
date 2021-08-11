#include "precompiled.h"
#include "databaseSvc.h"
#include "checkResult.h"
#include "patient.h"
#include "program.h"

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

PatientVm databaseSvc::GetPatientById(QString id)
{
    List_Patient list_patient;
    qx_query query("select * from patient where id=:id");
    query.bind(":id",id);
    QSqlError daoError = qx::dao::execute_query(query, list_patient);;

    std::cout<<"*****patient_list*****"<<std::endl;
    for(auto& i:list_patient)
        std::cout<<i->m_id.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
    std::cout<<std::endl;
    std::shared_ptr<Patient> pp=list_patient.front();
    PatientVm pv(pp);
    return pv;
}

}
