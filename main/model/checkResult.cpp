#include "checkResult.h"
#include "patient.h"
#include "program.h"
QX_REGISTER_CPP_QX_API(CheckResult)

namespace qx{
    template <> void register_class(QxClass<CheckResult> &t){
        t.id(&CheckResult::m_id,"checkResult_id");
        t.data(&CheckResult::m_strategy, "strategy");
        t.data(&CheckResult::m_params, "params");
        t.data(&CheckResult::m_data,"data");
        t.data(&CheckResult::m_time,"time");
        t.relationManyToOne(&CheckResult::m_patient,"patient_id");
        t.relationManyToOne(&CheckResult::m_program,"program_id");
    }
}
