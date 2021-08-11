#include "patientVm.h"
#include "patient.h"
#include <QDate>
namespace Perimeter {

QString PatientVm::getID()
{
    return m_patient->m_id;
}

void PatientVm::setID(QString value)
{
    m_patient->m_id=value;
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
