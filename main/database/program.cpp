#include "program.h"
#include "checkResult.h"
QX_REGISTER_CPP_QX_API(Program)

template <> void qx::register_class(QxClass<Program> &t){

    t.id(&Program::m_id, "program_id");
    t.data(&Program::m_type, "type");
    t.data(&Program::m_name, "name");
    t.data(&Program::m_params,"params");
    t.data(&Program::m_data,"data");
    t.data(&Program::m_category, "category");

    t.relationOneToMany(&Program::m_checkResultX,"CheckResult_List","program_id");
}

