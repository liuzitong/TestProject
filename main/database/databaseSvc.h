#ifndef DATABASEVM_H
#define DATABASEVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
class CheckResult;class Patient;class Program;

namespace Perimeter {
class databaseSvc: public QxPack::IcViewModelBase
{
public:
    databaseSvc();
};
}
#endif // DATABASEVM_H
