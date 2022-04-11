#include "PatientVm.h"
#include "perimeter/main/database/patient.h"
#include "perimeter/base/common/perimeter_def.h"
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include <QDate>
#define T_PrivPtr( o )  perimeter_objcast( Patient*, o )

namespace Perimeter {

void PatientVm::hello()
{
    qDebug()<<"patient says hello.";
}



PatientVm::PatientVm(const QVariantList & args)
{
    qDebug()<<"constructor";
    if(args.count()==0)
    {
        m_patient=std::shared_ptr<Patient>(new Patient());
    }
    else
    {
        Patient_List Patient_List;
        int id=args[0].toInt();
        qx_query query("select * from patient where id=:id");
        query.bind(":id",id);
        QSqlError daoError = qx::dao::execute_query(query, Patient_List);
        if(Patient_List.count()==0)
            m_patient=std::shared_ptr<Patient>(new Patient());
        else
        {
            m_patient=Patient_List.first();
        }
    }
}

void PatientVm::update()
{
    m_patient->m_lastUpdate=QDateTime::currentDateTime();
    qx::dao::update(m_patient);
}

void PatientVm::insert()
{
    m_patient->m_lastUpdate=QDateTime::currentDateTime();
    qx::dao::insert(m_patient);
}




PatientVm &PatientVm::operator=(const PatientVm &other)
{
    this->m_patient=other.m_patient;
    return *this;
}

//PatientVm::PatientVm(PatientVm &&other)
//{
//    this->m_patient=other.m_patient;
//    other.m_patient=nullptr;
//}

//PatientVm::PatientVm(const PatientVm &other)
//{
//    this->m_patient=other.m_patient;
//}

//PatientVm::~PatientVm()
//{
//    if(m_patient!=nullptr)
//        perimeter_delete(m_patient,Patient);
//}

long PatientVm::getID()
{
    return  m_patient ->m_id;
}

void PatientVm::setID(long value)
{
    m_patient->m_id=value;
}

QString PatientVm::getPatientID()
{
   return  m_patient->m_patientId;
}

void PatientVm::setPatientID(QString value)
{
    m_patient ->m_patientId=value;
}

QString PatientVm::getName()
{
    return  m_patient ->m_name;
}

void PatientVm::setName(QString value)
{
   m_patient->m_name=value;
}

int PatientVm::getSex()
{
    return static_cast<int>( m_patient ->m_sex);
}

void PatientVm::setSex(int value)
{
    m_patient ->m_sex=Patient::sex(value);
}

QString PatientVm::getBirthDate()
{
    return  m_patient->m_birthDate.toString("yyyy-MM-dd");
}

void PatientVm::setBirthDate(QString date)
{
    QList<QString> list=date.split("-");
    int year=list[0].toInt();
    int month=list[1].toInt();
    int day=list[2].toInt();
    m_patient->m_birthDate.setDate(year,month,day);
    qDebug()<<year;
    qDebug()<<month;
    qDebug()<<day;
}

QDateTime PatientVm::getLastUpdate()
{
    return m_patient->m_lastUpdate;
}

void Perimeter::PatientVm::setLastUpdate(QDateTime value)
{
    m_patient->m_lastUpdate=value;
}

}
