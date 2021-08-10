#ifndef Patient_H
#define Patient_H
#include "precompiled.h"
#include <QxOrm_Impl.h>
#include <QDate>
#include "checkResult.h"
class QX_DLL_EXPORT_API Patient{
public:
    enum sex { male, female, unknown };
// -- typedef
    typedef std::shared_ptr<CheckResult> CheckResult_ptr;
    typedef std::vector<CheckResult_ptr> List_CheckResult;

    QString m_id;
    QString m_name;
    sex m_sex;
    QDate m_birthDate;
    List_CheckResult m_checkResultX;
    Patient() = default;
    Patient(QString id,QString name,sex sex,QDate date):m_id(id),m_name(name),m_sex(sex),m_birthDate(date){}
    virtual ~Patient(){}
};

QX_REGISTER_PRIMARY_KEY(Patient, QString)
QX_REGISTER_HPP_QX_API(Patient, qx::trait::no_base_class_defined, 0)

typedef std::shared_ptr<Patient> Patient_ptr;
//typedef qx::QxCollection<QString, Patient_ptr> List_Patient;
typedef QList<Patient_ptr> List_Patient;

#endif // Patient_H
