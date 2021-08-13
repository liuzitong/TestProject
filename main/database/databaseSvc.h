#ifndef DATABASEVM_H
#define DATABASEVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <QDate>
#include "perimeter/main/database/patientVm.h"

namespace Perimeter {
class databaseSvc:public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QObject* Patient READ getPatientVm CONSTANT)
public:
    databaseSvc( )=default;
    Q_INVOKABLE static void initDataBase();
    Q_INVOKABLE static void createTable();
    Q_INVOKABLE static void createData();
    Q_INVOKABLE static QObject* getPatientObjByPatientId(QString id);
    Q_INVOKABLE static void test();
    Q_INVOKABLE static QObject* getPatientModel();
    Q_INVOKABLE static void addPatient(QString patientId,QString name,int sex,QDate date);
    Q_INVOKABLE static void updatePatient(long id,QString patientId, QString name, int sex, QDate date);
    Q_INVOKABLE static void deletePatient(long id);
};
}
#endif // DATABASEVM_H
