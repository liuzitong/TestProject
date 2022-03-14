#include "programVm.h"
namespace Perimeter {


StaticProgramVM::StaticProgramVM(const QVariantList & args)
{
    int program_id=args[0].toInt();
    qx_query query("select * from program where program_id=:program_id");
    query.bind(":program_id",program_id);
    Program_List program_List;
    QSqlError daoError = qx::dao::execute_query(query, program_List);
    Program_ptr program_ptr=program_List.first();
    programModel=QSharedPointer<ProgramModel<Type::ThreshHold>>(new ProgramModel<Type::ThreshHold>(program_ptr));
    m_id=programModel->m_id;
    m_type=int(programModel->m_type);
    m_staticParamsVm=new StaticParamsVM(&(programModel->m_params));

    for(auto& v:programModel->m_data.strategies)
    {
        m_strategies.append(int(v));
    }
    for(auto& v:programModel->m_data.dots)
    {
        m_dots.append(QPoint(v.x,v.y));
    }
}

MoveProgramVM::MoveProgramVM(const QVariantList & args)
{
    int program_id=args[0].toInt();
    qx_query query("select * from program where program_id=:program_id");
    query.bind(":program_id",program_id);
    Program_List program_List;
    QSqlError daoError = qx::dao::execute_query(query, program_List);
    Program_ptr program_ptr=program_List.first();
    programModel=QSharedPointer<ProgramModel<Type::Move>>(new ProgramModel<Type::Move>(program_ptr));
    m_id=programModel->m_id;
    m_type=int(programModel->m_type);
    m_moveParamsVm=new MoveParamVM(&(programModel->m_params));

    for(auto& v:programModel->m_data.dots)
    {
        m_dots.append(QPoint(v.x,v.y));
    }
}

}
