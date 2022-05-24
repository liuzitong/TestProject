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
        return;
    }
    else
    {
        Patient_List Patient_List;
        Patient_ptr patient_ptr;
        int id=args[0].toInt();
        qx_query query("select * from patient where id=:id");
        query.bind(":id",id);
        QSqlError daoError = qx::dao::execute_query(query, Patient_List);
        if(Patient_List.count()==0)
            return;
        else
        {
            patient_ptr=Patient_List.first();
            m_id=patient_ptr->m_id;
            m_patientID=patient_ptr->m_patientId;
            m_name=patient_ptr->m_name;
            m_sex=int(patient_ptr->m_sex);
            m_birthDate=patient_ptr->m_birthDate;
            m_lastUpdate=patient_ptr->m_lastUpdate;
        }
    }


}

void PatientVm::update()
{
    m_lastUpdate=QDateTime::currentDateTime();
    auto patient_ptr=QSharedPointer<Patient>(new Patient(m_id,m_patientID,m_name,Patient::sex(m_sex),m_birthDate,m_lastUpdate));
    qx::dao::update(patient_ptr);
}

void PatientVm::insert()
{
    m_lastUpdate=QDateTime::currentDateTime();
    auto patient_ptr=QSharedPointer<Patient>(new Patient(m_patientID,m_name,Patient::sex(m_sex),m_birthDate,m_lastUpdate));
    qx::dao::update(patient_ptr);
}




//PatientVm &PatientVm::operator=(const PatientVm &other)
//{
//    this->m_patient=other.m_patient;
//    return *this;
//}

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
    return  m_id;
}

void PatientVm::setID(long value)
{
    m_id=value;
}

QString PatientVm::getPatientID()
{
   return  m_patientID;
}

void PatientVm::setPatientID(QString value)
{
    m_patientID=value;
}

QString PatientVm::getName()
{
    return  m_name;
}

void PatientVm::setName(QString value)
{
   m_name=value;
}

int PatientVm::getSex()
{
    return m_sex;
}

void PatientVm::setSex(int value)
{
    m_sex=value;
}

QString PatientVm::getBirthDate()
{
    return  m_birthDate.toString("yyyy-MM-dd");
}

void PatientVm::setBirthDate(QString date)
{
    QList<QString> list=date.split("-");
    int year=list[0].toInt();
    int month=list[1].toInt();
    int day=list[2].toInt();
    m_birthDate.setDate(year,month,day);
    qDebug()<<year;
    qDebug()<<month;
    qDebug()<<day;
}

QDateTime PatientVm::getLastUpdate()
{
    return m_lastUpdate;
}

void Perimeter::PatientVm::setLastUpdate(QDateTime value)
{
    m_lastUpdate=value;
}

}
