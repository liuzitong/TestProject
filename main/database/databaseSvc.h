#ifndef DATABASEVM_H
#define DATABASEVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <QDate>
#include <QDateTime>
#include "perimeter/main/database/patientVm.h"
#include "patientListModel.h"
#include "program.h"


namespace Perimeter {
class databaseSvc:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* patientListModel READ getPatientListModel NOTIFY patientListChanged)

public:
    PatientListModel* m_plm;
    databaseSvc( );
    ~databaseSvc();
    Q_INVOKABLE void initDataBase();
    Q_INVOKABLE void createTable();
    Q_INVOKABLE void createData();
    QObject* getPatientListModel();
    Q_SIGNAL void patientListChanged();
//    Q_INVOKABLE QObject* getPatientObjByPatientId(QString id);
    Q_INVOKABLE void setPatientModel();
    Q_INVOKABLE void addPatient(QString patientId,QString name,int sex,QDate date,QDateTime updateTime=QDateTime::currentDateTime());
    Q_INVOKABLE void updatePatient(long id,QString patientId, QString name, int sex, QDate date);
    Q_INVOKABLE void deletePatient(long id);
    Q_INVOKABLE void getPatientByTimeSpan(QDate from,QDate to);
    Q_INVOKABLE void getPatientByPatientId(QString id);
    Q_INVOKABLE void getPatientByName(QString name,QDate from,QDate to);
    Q_INVOKABLE void getPatientBySex(int sex,QDate from,QDate to);
    Q_INVOKABLE void getPatientByBirthDate(QDate date);
    Q_INVOKABLE void addProgram(int type,QString name,QString params,QString data,int category=4);
    Q_INVOKABLE QObject* getPatient();

    Q_INVOKABLE QObject* getParams();

//    Q_INVOKABLE void addCheckResult(QString params,QString data,QDateTime time,Patient_ptr patient,Program_ptr program);

private:
   inline QString convertQDateToQString(QDate date);
//    Q_INVOKABLE QObject* getPatientModelByIndex(int startIndex,int count,QDate from,QDate to);
//    Q_INVOKABLE QObject* getPatientModelByIndex(int startIndex,int count,QString name);
//    Q_INVOKABLE QObject* getPatientModelByIndex(int startIndex,int count,int sex);
//    Q_INVOKABLE QObject* getPatientModelByIndex(int startIndex,int count,QDate birthDate);
};


}
#endif // DATABASEVM_H
