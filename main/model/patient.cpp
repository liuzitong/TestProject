#include "patient.h"

QX_REGISTER_CPP_QX_API(Patient)

namespace qx{

    template <> void register_class(QxClass<Patient> &t){

        t.id(&Patient::m_id, "patient_id");
        t.data(&Patient::m_name, "name");
        t.data(&Patient::m_birthDate, "birthDate");
        t.data(&Patient::m_sex,"sex");

        t.relationOneToMany(&Patient::m_checkResultX,"list_checkResult","patient_id");
    }
}
