#ifndef CHECKPROCEDUCE_H
#define CHECKPROCEDUCE_H

#include <perimeter/main/viewModel/patientVm.h>
#include <perimeter/main/viewModel/programVm.h>

namespace Perimeter{
class CheckProceduce
{
public:
    CheckProceduce();
private:
    PatientVm* m_patientVm;
    QObject* m_programVm;
};
}
#endif // CHECKPROCEDUCE_H
