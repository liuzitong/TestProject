#include "programVm.h"
#include <qDebug>
#include <math.h>
namespace Perimeter {


StaticProgramVM::StaticProgramVM(const QVariantList & args)
{
    if(args.count()==0)
    {
        m_staticParamsVm=new StaticParamsVM();
        return;
    }
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
    m_category=int(programModel->m_category);
    for(auto& v:programModel->m_data.strategies)
    {
        m_strategies.append(int(v));
    }
//    qDebug()<<program_ptr->m_data;
    for(auto& v:programModel->m_data.dots)
    {
        m_dots.append(QPointF(v.x,v.y));
    }
}

StaticProgramVM::~StaticProgramVM()
{
    delete m_staticParamsVm;
}

void StaticProgramVM::updateProgram()
{
    Program_ptr pp=getProgramData();
    QSqlError error=qx::dao::update(pp);
}

void StaticProgramVM::insertProgram()
{

    Program_ptr pp=getProgramData();
    QSqlError error=qx::dao::insert(pp);
}

void StaticProgramVM::deleteProgram()
{
    qDebug()<<"delete Program";
    Program_ptr pp=getProgramData();
    qDebug()<<pp->m_id;
    QSqlError error=qx::dao::delete_by_id(pp);
}

Program_ptr StaticProgramVM::getProgramData()
{
    QSharedPointer<ProgramModel<Type::ThreshHold>> programModel=QSharedPointer<ProgramModel<Type::ThreshHold>>(new ProgramModel<Type::ThreshHold>());
    programModel->m_id=m_id;
    programModel->m_name=m_name;
    programModel->m_type=Type(m_type);
    programModel->m_params.commonParams=m_staticParamsVm->getCommonParams()->getData();
    programModel->m_params.fixedParams=m_staticParamsVm->getFixedParams()->getData();
    programModel->m_category= Category(m_category);

    for(auto& v:m_strategies)
    {
        programModel->m_data.strategies.push_back(StaticParams::CommonParams::Strategy(v.toInt()));
    }

    qDebug()<<"***********************************";
    qDebug()<<"***********************************";
    qDebug()<<"***********************************";
    for(auto& v:m_dots)
    {
        if(fabs(v.toPointF().x())<0.05&&fabs(v.toPointF().y())<0.05)
        {
            float x=v.toMap()["x"].toFloat();
            float y=v.toMap()["y"].toFloat();
            qDebug()<<x;
            programModel->m_data.dots.push_back({x,y});
//            programModel->m_data.dots.push_back({x,y});
        }
        else{
            programModel->m_data.dots.push_back({float(v.toPointF().x()),float(v.toPointF().y())});
        }
    }
    return programModel->ModelToDB();

}





MoveProgramVM::MoveProgramVM(const QVariantList & args)
{
    if(args.count()==0)
    {
        m_moveParamsVm=new MoveParamVM();
        return;
    }
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
    m_name=programModel->m_name;
    m_type=int(programModel->m_type);
    m_moveParamsVm=new MoveParamVM(programModel->m_params);
    m_category=int(programModel->m_category);

    for(auto& v:programModel->m_data.dots)
    {
        m_dots.append(QPoint(v.x,v.y));
    }

}

MoveProgramVM::~MoveProgramVM()
{
    delete m_moveParamsVm;
}

void MoveProgramVM::updateProgram()
{
    Program_ptr pp=getProgramData();
    QSqlError error=qx::dao::update(pp);
}

void MoveProgramVM::insertProgram()
{
    Program_ptr pp=getProgramData();
    QSqlError error=qx::dao::insert(pp);
}

void MoveProgramVM::deleteProgram()
{
    Program_ptr pp=getProgramData();
    QSqlError error=qx::dao::delete_by_id(pp);
}


Program_ptr MoveProgramVM::getProgramData()
{
    QSharedPointer<ProgramModel<Type::Move>> programModel=QSharedPointer<ProgramModel<Type::Move>>(new ProgramModel<Type::Move>());
    programModel->m_id=m_id;
    programModel->m_type=Type(m_type);
    programModel->m_name=m_name;
//    qDebug()<<m_name;
    programModel->m_params=m_moveParamsVm->getData();
    programModel->m_category= Category(m_category);


    for(auto& v:m_dots)
    {
        if(fabs(v.toPointF().x())<0.05&&fabs(v.toPointF().y())<0.05)
        {
            float x=v.toMap()["x"].toFloat();
            float y=v.toMap()["y"].toFloat();
            qDebug()<<x;
            programModel->m_data.dots.push_back({x,y});
//            programModel->m_data.dots.push_back({x,y});
        }
        else{
            programModel->m_data.dots.push_back({float(v.toPointF().x()),float(v.toPointF().y())});
        }
    }

    return programModel->ModelToDB();
}



}
