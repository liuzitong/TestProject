#ifndef PATIENTVM_H
#define PATIENTVM_H
#include "perimeter/base/common/perimeter_def.h"
//#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <memory>
#include <QDate>

namespace Perimeter{
class PERIMETER_API PatientVm:public QObject
{
    Q_OBJECT
    Q_PROPERTY(long id READ getID WRITE setID)
    Q_PROPERTY(QString patientId READ getPatientID WRITE setPatientID)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(int sex READ getSex WRITE setSex)
    Q_PROPERTY(QDate date READ getBirthDate WRITE setBirthDate)

public:
    PatientVm();
    PatientVm(void* patient);
    PatientVm(const PatientVm& other);
    PatientVm(PatientVm&& other);
    PatientVm& operator=(const PatientVm& other);
    ~PatientVm();
    long getID();void setID(long value);
    QString getPatientID();void setPatientID(QString value);
    QString getName();void setName(QString value);
    int getSex();void setSex(int value);
    QDate getBirthDate();void setBirthDate(QDate value);

private:
    void* m_patient;
};
}
#endif // PATIENTVM_H
