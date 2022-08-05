#ifndef PATIENTVM_H
#define PATIENTVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "perimeter/main/database/patient.h"
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
    Q_PROPERTY(QString birthDate READ getBirthDate WRITE setBirthDate)
    Q_PROPERTY(QObject* rx READ getRx WRITE setRx NOTIFY rxChanged)
    Q_PROPERTY(QDateTime lastUpdate READ getLastUpdate WRITE setLastUpdate)
    Q_PROPERTY(int age READ getAge)

public:
//    Q_INVOKABLE PatientVm();
    Q_INVOKABLE explicit PatientVm(const QVariantList &);
//    Q_INVOKABLE void deletePatient();
    Q_INVOKABLE virtual ~PatientVm() Q_DECL_OVERRIDE;
    Q_INVOKABLE void update();
    Q_INVOKABLE void insert();
//    Q_INVOKABLE int age();
//    PatientVm& operator=(const PatientVm& other);
    long getID();void setID(long value);
    QString getPatientID();void setPatientID(QString value);
    QString getName();void setName(QString value);
    int getSex();void setSex(int value);
    QString getBirthDate();void setBirthDate(QString value);
    QObject* getRx(){return m_rx;}void setRx(QObject* value){m_rx=value;emit rxChanged(value);}Q_SIGNAL void rxChanged(QObject* value);
    QDateTime getLastUpdate();void setLastUpdate(QDateTime lastUpdate);
    int getAge(){return m_age;}

private:
    Patient_ptr getPatientData();
    long m_id;
    QString m_patientID;
    QString m_name;
    int m_sex;
    QObject* m_rx=nullptr;
    QDate m_birthDate;
    QDateTime m_lastUpdate;
    int m_age;

};
}
#endif // PATIENTVM_H
