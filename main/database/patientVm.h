#ifndef PATIENTVM_H
#define PATIENTVM_H
#include "perimeter/base/common/perimeter_def.h"
//#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <memory>
#include <QDate>
class Patient;
namespace Perimeter{
class PERIMETER_API PatientVm:QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ getID WRITE setID)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(int sex READ getSex WRITE setSex)
    Q_PROPERTY(QDate date READ getBirthDate WRITE setBirthDate)

public:
    PatientVm()=default;
    PatientVm(std::shared_ptr<Patient> patient);
    PatientVm(const PatientVm& other);
    PatientVm(const PatientVm&& other);
    PatientVm& operator=(const PatientVm& other);
    ~PatientVm(){};
    QString getID();void setID(QString value);
    QString getName();void setName(QString value);
    int getSex();void setSex(int value);
    QDate getBirthDate();void setBirthDate(QDate value);

private:
    std::shared_ptr<Patient> m_patient;
};
}
#endif // PATIENTVM_H
