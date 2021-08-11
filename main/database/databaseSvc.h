#ifndef DATABASEVM_H
#define DATABASEVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include "patientVm.h"


namespace Perimeter {
class databaseSvcPriv;
class databaseSvc: public QxPack::IcViewModelBase
{
public:
    Q_INVOKABLE explicit databaseSvc( );
    virtual ~databaseSvc( ) Q_DECL_OVERRIDE;
//    PatientVm GetPatientById(QString id);
private:
    databaseSvcPriv* m_obj;
};
}
#endif // DATABASEVM_H
