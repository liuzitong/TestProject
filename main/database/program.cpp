#include "program.h"

QX_REGISTER_CPP_QX_API(Program)

template <> void qx::register_class(QxClass<Program> &t){

    t.id(&Program::m_id, "program_id");
    t.data(&Program::m_predefined, "predefined");
    t.data(&Program::m_type, "type");
    t.data(&Program::m_name, "name");
    t.data(&Program::m_params,"params");
    t.data(&Program::m_data,"data");

    t.relationOneToMany(&Program::m_checkResultX,"list_checkResult","program_id");
}

