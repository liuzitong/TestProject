#include "programVm.h"
namespace Perimeter {


StaticProgramVM::StaticProgramVM(const QVariantList & args)
{
//    qDebug()<<"static programvm cons";
    int program_id=args[0].toInt();
//    qDebug()<<program_id;
    qx_query query("select * from program where program_id=:program_id");
    query.bind(":program_id",program_id);
    Program_List program_List;
    QSqlError daoError = qx::dao::execute_query(query, program_List);
    Program_ptr program_ptr=program_List.first();
//    qDebug()<<program_ptr->m_params;
    QSharedPointer<ProgramModel<Type::ThreshHold>> programModel=QSharedPointer<ProgramModel<Type::ThreshHold>>(new ProgramModel<Type::ThreshHold>(program_ptr));
    m_id=programModel->m_id;
    m_type=int(programModel->m_type);
    m_name=programModel->m_name;
    m_staticParamsVm=new StaticParamsVM(programModel->m_params);
//    qDebug()<<programModel->m_params.commonParams.Range[0];
//    qDebug()<<programModel->m_params.commonParams.Range[1];
    for(auto& v:programModel->m_data.strategies)
    {
        m_strategies.append(int(v));
    }
//    qDebug()<<program_ptr->m_data;
    for(auto& v:programModel->m_data.dots)
    {
        m_dots.append(QPoint(v.x,v.y));
    }
}

StaticProgramVM::~StaticProgramVM()
{
    delete m_staticParamsVm;
}

void StaticProgramVM::hello()
{
    qDebug()<<"hello program";
}


MoveProgramVM::MoveProgramVM(const QVariantList & args)
{
//    qDebug()<<"move programvm cons";
    int program_id=args[0].toInt();
//    qDebug()<<program_id;
    qx_query query("select * from program where program_id=:program_id");
    query.bind(":program_id",program_id);
    Program_List program_List;
    QSqlError daoError = qx::dao::execute_query(query, program_List);
    Program_ptr program_ptr=program_List.first();
//    qDebug()<<program_ptr->m_params;
    auto programModel=QSharedPointer<ProgramModel<Type::Move>>(new ProgramModel<Type::Move>(program_ptr));
    m_id=programModel->m_id;
    m_type=int(programModel->m_type);
    m_moveParamsVm=new MoveParamVM(programModel->m_params);

    for(auto& v:programModel->m_data.dots)
    {
        m_dots.append(QPoint(v.x,v.y));
    }
}

MoveProgramVM::~MoveProgramVM()
{
    delete m_moveParamsVm;
}

}
