#ifndef PATIENTVM_H
#define PATIENTVM_H
#include "perimeter/base/common/perimeter_def.h"
//#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <memory>
#include <QDate>

class Patient;
namespace Perimeter{
class PERIMETER_API PatientObj:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ getID WRITE setID)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(int sex READ getSex WRITE setSex)
    Q_PROPERTY(QDate date READ getBirthDate WRITE setBirthDate)

public:
    PatientObj()=default;
    PatientObj(std::shared_ptr<Patient> patient);
    PatientObj(const PatientObj& other);
    PatientObj(const PatientObj&& other);
    PatientObj& operator=(const PatientObj& other);
    ~PatientObj();
    QString getID();void setID(QString value);
    QString getName();void setName(QString value);
    int getSex();void setSex(int value);
    QDate getBirthDate();void setBirthDate(QDate value);

private:
    std::shared_ptr<Patient> m_patient;
};
}
#endif // PATIENTVM_H
