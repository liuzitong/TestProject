#include "PatientVm.h"
#include "patient.h"
#include <QDate>
namespace Perimeter {

PatientVm::PatientVm(std::shared_ptr<Patient> patient)
{
    m_patient=patient;
}

PatientVm &PatientVm::operator=(const PatientVm &other)
{
    this->m_patient=other.m_patient;
    return *this;
}
PatientVm::PatientVm(const PatientVm &&other)
{
    this->m_patient=other.m_patient;
}

PatientVm::PatientVm(const PatientVm &other)
{
    this->m_patient=other.m_patient;
}

PatientVm::~PatientVm()
{
    m_patient=0;
    qDebug()<<"patient  destructed";
}

long PatientVm::getID()
{
    return m_patient->m_id;
}

void PatientVm::setID(long value)
{
    m_patient->m_id=value;
}

QString PatientVm::getPatientID()
{
   return m_patient->m_patinetId;
}

void PatientVm::setPatientID(QString value)
{
    m_patient->m_patinetId=value;
}

QString PatientVm::getName()
{
    return m_patient->m_name;
}

void PatientVm::setName(QString value)
{
    m_patient->m_name=value;
}

int PatientVm::getSex()
{
    return m_patient->m_sex;
}

void PatientVm::setSex(int value)
{
    m_patient->m_sex=Patient::sex(value);
}

QDate PatientVm::getBirthDate()
{
    return m_patient->m_birthDate;
}

void PatientVm::setBirthDate(QDate value)
{
    m_patient->m_birthDate=value;
}

}
