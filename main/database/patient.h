#ifndef Patient_H
#define Patient_H
#include "precompiled.h"
#include <QxOrm_Impl.h>
#include <QDate>
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <QObject>
#include <QDateTime>

class CheckResult;
class QX_DLL_EXPORT_API Patient{
public:
    enum sex { male, female, unknown };
// -- typedef
    typedef std::shared_ptr<CheckResult> CheckResult_ptr;
    typedef std::vector<CheckResult_ptr> CheckResult_List;

    long m_id;
    QString m_patinetId;
    QString m_name;
    sex m_sex;
    QDate m_birthDate;
    QDateTime m_lastUpdate;
    CheckResult_List m_checkResultX;
    Patient() = default;
    Patient(QString patientId,QString name,sex sex,QDate date,QDateTime lastUpdate=QDateTime::currentDateTime()):m_patinetId(patientId),m_name(name),m_sex(sex),m_birthDate(date),m_lastUpdate(lastUpdate){}
    Patient(long id,QString patientId,QString name,sex sex,QDate date,QDateTime lastUpdate=QDateTime::currentDateTime()):m_id(id),m_patinetId(patientId),m_name(name),m_sex(sex),m_birthDate(date),m_lastUpdate(lastUpdate){}
    virtual ~Patient(){}
};

typedef std::shared_ptr<Patient> Patient_ptr;
typedef QList<Patient_ptr> Patient_List;
QX_REGISTER_PRIMARY_KEY(Patient, long)
QX_REGISTER_HPP_QX_API(Patient, qx::trait::no_base_class_defined, 0)

#endif



