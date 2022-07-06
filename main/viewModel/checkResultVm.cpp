#include "checkResultVm.h"
#include "paramsVm.h"
namespace Perimeter
{

CheckResultVm::CheckResultVm(const QVariantList & args)
{
    if(args.count()==0)
    {
        return;
    }
    int checkResult_id=args[0].toInt();
    qx_query query("select * from CheckResult where checkResult_id=:checkResult_id");
    query.bind(":checkResult_id",checkResult_id);
    CheckResult_List checkResult_List;
    QSqlError daoError = qx::dao::execute_query(query, checkResult_List);
    CheckResult_ptr checkResult_ptr=checkResult_List.first();
    Type type=static_cast<Type>(checkResult_ptr->m_type);
    if(type!=Type::Move)
    {
        auto checkResultModel=QSharedPointer<CheckResultModel<Type::ThreshHold>>(new CheckResultModel<Type::ThreshHold>(checkResult_ptr));
        m_id=checkResultModel->m_id;
        m_type=int(checkResultModel->m_type);
        m_OS_OD=checkResultModel->m_OS_OD;
        m_pic=checkResultModel->m_pic;
        m_time=checkResultModel->m_time;
        m_params=new StaticParamsVM(checkResultModel->m_params);
        m_resultData=new ResultDataVm(checkResultModel->m_data);
        m_patient_id=checkResultModel->m_patient_id;
        m_program_id=checkResultModel->m_program_id;
    }
    else
    {
        auto checkResultModel=QSharedPointer<CheckResultModel<Type::Move>>(new CheckResultModel<Type::Move>(checkResult_ptr));
        m_id=checkResultModel->m_id;
        m_type=int(checkResultModel->m_type);
        m_OS_OD=checkResultModel->m_OS_OD;
        m_pic=checkResultModel->m_pic;
        m_time=checkResultModel->m_time;
        m_params=new MoveParamVM(checkResultModel->m_params);
        m_resultData=new ResultDataVm(checkResultModel->m_data);
        m_patient_id=checkResultModel->m_patient_id;
        m_program_id=checkResultModel->m_program_id;
    }
}

CheckResultVm::~CheckResultVm()
{
    delete m_params;
    delete m_resultData;
}



ResultDataVm::ResultDataVm(ResultData<Type::ThreshHold> resultData)
{
    m_alarm=resultData.alarm;
    m_falseNegativeCount=resultData.falseNegativeCount;
    m_falseNegativeTestCount=resultData.falseNegativeTestCount;
    m_falsePositiveCount=resultData.falsePositiveCount;
    m_falsePositiveTestCount=resultData.falsePositiveTestCount;
    m_fixationLostCount=resultData.fixationLostCount;
    m_fixationLostTestCount=resultData.fixationLostTestCount;
    m_stimulusCount=resultData.stimulusCount;
    m_thresholdCount=resultData.thresholdCount;
    m_testTimespan=resultData.testTimespan;
    m_ambientLight=resultData.ambientLight;
    m_E_Light_pv=resultData.E_Light_pv;
    m_T_Light_pv=resultData.T_Light_pv;
    for(auto &i:resultData.fixationDeviation){m_fixationDeviation.append(i);}
    for(auto &i:resultData.checkdata){m_checkData.append(i);}
}

ResultDataVm::ResultDataVm(ResultData<Type::Move> resultData)
{
    m_alarm=resultData.alarm;
    m_falseNegativeCount=resultData.falseNegativeCount;
    m_falseNegativeTestCount=resultData.falseNegativeTestCount;
    m_falsePositiveCount=resultData.falsePositiveCount;
    m_falsePositiveTestCount=resultData.falsePositiveTestCount;
    m_fixationLostCount=resultData.fixationLostCount;
    m_fixationLostTestCount=resultData.fixationLostTestCount;
    m_stimulusCount=resultData.stimulusCount;
    m_thresholdCount=resultData.thresholdCount;
    m_testTimespan=resultData.testTimespan;
    m_ambientLight=resultData.ambientLight;
    m_E_Light_pv=resultData.E_Light_pv;
    m_T_Light_pv=resultData.T_Light_pv;
    for(auto &i:resultData.fixationDeviation){m_fixationDeviation.append(i);}
    for(auto &i:resultData.checkdata){m_checkData.append(QPointF{i.x,i.y});}
}



}
