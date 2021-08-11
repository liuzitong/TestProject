#include "patient.h"
#include "checkResult.h"
QX_REGISTER_CPP_QX_API(Patient)
template <> void qx::register_class(QxClass<Patient> &t){
    t.id(&Patient::m_id, "patient_id");
    t.data(&Patient::m_name, "name");
    t.data(&Patient::m_birthDate, "birthDate");
    t.data(&Patient::m_sex,"sex");
    t.relationOneToMany(&Patient::m_checkResultX,"CheckResult_List","patient_id");
}


