#ifndef PATIENTVM_H
#define PATIENTVM_H
#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_viewmodelbase.hxx"
#include <memory>
class Patient;
namespace Perimeter{
class patientVm : public QxPack::IcViewModelBase
{
public:

    patientVm();
private:
    std::shared_ptr<Patient> m_patient;
};
}
#endif // PATIENTVM_H
