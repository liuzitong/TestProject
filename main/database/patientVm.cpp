#include "PatientVm.h"
#include "patient.h"
#include "perimeter/base/common/perimeter_def.h"
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include <QDate>
#define T_PrivPtr( o )  perimeter_objcast( Patient*, o )

namespace Perimeter {

PatientVm::PatientVm()
{
    m_patient=perimeter_new(Patient);
}

PatientVm::PatientVm(void* patient)
{
    m_patient=patient;
}

PatientVm &PatientVm::operator=(const PatientVm &other)
{
    this->m_patient=other.m_patient;
    return *this;
}

PatientVm::PatientVm(PatientVm &&other)
{
    this->m_patient=other.m_patient;
    other.m_patient=nullptr;
}

PatientVm::PatientVm(const PatientVm &other)
{
    this->m_patient=other.m_patient;
}

PatientVm::~PatientVm()
{
//    m_patient=0;
//    qDebug()<<"patient  destructed";
    if(m_patient!=nullptr)
        perimeter_delete(m_patient,Patient);

}

long PatientVm::getID()
{
    return T_PrivPtr( m_patient )->m_id;
}

void PatientVm::setID(long value)
{
    T_PrivPtr( m_patient )->m_id=value;
}

QString PatientVm::getPatientID()
{
   return T_PrivPtr( m_patient )->m_patinetId;
}

void PatientVm::setPatientID(QString value)
{
    T_PrivPtr( m_patient )->m_patinetId=value;
}

QString PatientVm::getName()
{
    return T_PrivPtr( m_patient )->m_name;
}

void PatientVm::setName(QString value)
{
    T_PrivPtr( m_patient )->m_name=value;
}

int PatientVm::getSex()
{
    return T_PrivPtr( m_patient )->m_sex;
}

void PatientVm::setSex(int value)
{
    T_PrivPtr( m_patient )->m_sex=Patient::sex(value);
}

QDate PatientVm::getBirthDate()
{
    return T_PrivPtr( m_patient )->m_birthDate;
}

void PatientVm::setBirthDate(QDate value)
{
    T_PrivPtr( m_patient )->m_birthDate=value;
}

QDateTime PatientVm::getLastUpdate()
{
    return T_PrivPtr( m_patient )->m_lastUpdate;
}

void Perimeter::PatientVm::setLastUpdate(QDateTime value)
{
    T_PrivPtr(m_patient)->m_lastUpdate=value;
}

}
