#ifndef DATABASEVM_H
#define DATABASEVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include "patientVm.h"


namespace Perimeter {
class databaseSvc: QObject
{
    Q_OBJECT
public:
    databaseSvc( )=default;
    Q_INVOKABLE static void initDataBase();
    Q_INVOKABLE static PatientVm GetPatientById(QString id);
};
}
#endif // DATABASEVM_H
