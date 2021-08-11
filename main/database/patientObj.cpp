#include "patientObj.h"
#include "patient.h"
#include <QDate>
namespace Perimeter {

PatientObj::PatientObj(std::shared_ptr<Patient> patient)
{
    m_patient=patient;
}

PatientObj &PatientObj::operator=(const PatientObj &other)
{
    this->m_patient=other.m_patient;
    return *this;
}
PatientObj::PatientObj(const PatientObj &&other)
{
    this->m_patient=other.m_patient;
}

PatientObj::PatientObj(const PatientObj &other)
{
    this->m_patient=other.m_patient;
}

PatientObj::~PatientObj()
{
    m_patient=0;
    qDebug()<<"patient  destructed";
}

QString PatientObj::getID()
{
    return m_patient->m_id;
}

void PatientObj::setID(QString value)
{
    m_patient->m_id=value;
}

QString PatientObj::getName()
{
    return m_patient->m_name;
}

void PatientObj::setName(QString value)
{
    m_patient->m_name=value;
}

int PatientObj::getSex()
{
    return m_patient->m_sex;
}

void PatientObj::setSex(int value)
{
    m_patient->m_sex=Patient::sex(value);
}

QDate PatientObj::getBirthDate()
{
    return m_patient->m_birthDate;
}

void PatientObj::setBirthDate(QDate value)
{
    m_patient->m_birthDate=value;
}

}
