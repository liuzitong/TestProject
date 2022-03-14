#ifndef PATIENTVM_H
#define PATIENTVM_H
#include "perimeter/base/common/perimeter_def.h"
//#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <memory>
#include <QDate>
#include <QDateTime>
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"

namespace Perimeter{
class PERIMETER_API PatientVm: public QObject
{
    Q_OBJECT
    Q_PROPERTY(long id READ getID WRITE setID)
    Q_PROPERTY(QString patientId READ getPatientID WRITE setPatientID)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(int sex READ getSex WRITE setSex)
    Q_PROPERTY(QDate date READ getBirthDate WRITE setBirthDate)
    Q_PROPERTY(QDateTime lastUpdate READ getLastUpdate WRITE setLastUpdate)

public:
//    Q_INVOKABLE PatientVm();
    Q_INVOKABLE explicit PatientVm(const QVariantList &);
    Q_INVOKABLE virtual ~PatientVm() Q_DECL_OVERRIDE;

    Q_INVOKABLE void hello();
    PatientVm& operator=(const PatientVm& other);
    long getID();void setID(long value);
    QString getPatientID();void setPatientID(QString value);
    QString getName();void setName(QString value);
    int getSex();void setSex(int value);
    QDate getBirthDate();void setBirthDate(QDate value);
    QDateTime getLastUpdate();void setLastUpdate(QDateTime lastUpdate);

private:
    void* m_patient;
};
}
#endif // PATIENTVM_H
