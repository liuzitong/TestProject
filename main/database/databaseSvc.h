#ifndef DATABASEVM_H
#define DATABASEVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <QDate>

namespace Perimeter {
class databaseSvc:public QObject
{
    Q_OBJECT
public:
    databaseSvc( )=default;
    Q_INVOKABLE static void initDataBase();
    Q_INVOKABLE static void createTable();
    Q_INVOKABLE static void createData();
    Q_INVOKABLE static QObject* getPatientById(QString id);
    Q_INVOKABLE static void test();
    Q_INVOKABLE static QObject* getPatientModel();
    Q_INVOKABLE static void addPatient(QString id,QString name,int sex,QDate date);
};
}
#endif // DATABASEVM_H
