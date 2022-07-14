#include "checkResultVm.h"
#include "paramsVm.h"
namespace Perimeter
{

static int count=0;
CheckResultVm::CheckResultVm(const QVariantList & args)
{
    qDebug()<<count;
    count++;
    if(args[0].toString()=="Threshold")
    {
        m_type=0;
        m_params=new StaticParamsVM();
        m_resultData=new ResultDataVm();
    }
    else if(args[0].toString()=="Move")
    {
        m_type=1;
        m_params=new MoveParamsVM();
        m_resultData=new ResultDataVm();
    }
    else
    {
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
            m_params=new MoveParamsVM(checkResultModel->m_params);
            m_resultData=new ResultDataVm(checkResultModel->m_data);
            m_patient_id=checkResultModel->m_patient_id;
            m_program_id=checkResultModel->m_program_id;
        }
    }

}

CheckResultVm::~CheckResultVm()
{
    count--;
    qDebug()<<count;
    delete m_params;
    delete m_resultData;
}

void CheckResultVm::insertCheckResult()
{
//     auto cp=getCheckResultData();
//     QSqlError error=qx::dao::insert(cp);
}


CheckResult_ptr CheckResultVm::getCheckResultData()
{
    if(m_type==int(Type::ThreshHold))
    {
        QSharedPointer<CheckResultModel<Type::ThreshHold>> checkResultModel=QSharedPointer<CheckResultModel<Type::ThreshHold>>(new CheckResultModel<Type::ThreshHold>());
        checkResultModel->m_id=m_id;
        checkResultModel->m_type=Type(m_type);
        checkResultModel->m_OS_OD=m_OS_OD;
        checkResultModel->m_pic=m_pic;
        checkResultModel->m_time=m_time;
        checkResultModel->m_params.commonParams=static_cast<StaticParamsVM*>(m_params)->getCommonParams()->getData();
        checkResultModel->m_params.commonParams=static_cast<StaticParamsVM*>(m_params)->getCommonParams()->getData();
        checkResultModel->m_data=m_resultData->getThresholdData();
        checkResultModel->m_patient_id=m_patient_id;
        checkResultModel->m_program_id=m_program_id;
        return checkResultModel->ModelToDB();
    }
    else
    {
        QSharedPointer<CheckResultModel<Type::Move>> checkResultModel=QSharedPointer<CheckResultModel<Type::Move>>(new CheckResultModel<Type::Move>());
        checkResultModel->m_id=m_id;
        checkResultModel->m_type=Type(m_type);
        checkResultModel->m_OS_OD=m_OS_OD;
        checkResultModel->m_pic=m_pic;
        checkResultModel->m_time=m_time;
        checkResultModel->m_params=static_cast<MoveParamsVM*>(m_params)->getData();
        checkResultModel->m_data=m_resultData->getMoveData();
        checkResultModel->m_patient_id=m_patient_id;
        checkResultModel->m_program_id=m_program_id;
        return checkResultModel->ModelToDB();
    }
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
    m_falsePositiveCount=resultData.falsePositiveCount;
    m_falsePositiveTestCount=resultData.falsePositiveTestCount;
    m_falseNegativeCount=resultData.falseNegativeCount;
    m_falseNegativeTestCount=resultData.falseNegativeTestCount;
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

ResultData<Type::ThreshHold> ResultDataVm::getThresholdData()
{
    ResultData<Type::ThreshHold> resultData;
    resultData.alarm=m_alarm;
    resultData.falsePositiveCount=m_falsePositiveCount;
    resultData.falsePositiveTestCount=m_falsePositiveTestCount;
    resultData.falseNegativeCount=m_falseNegativeCount;
    resultData.falseNegativeTestCount=m_falseNegativeTestCount;
    resultData.fixationLostCount=m_fixationLostCount;
    resultData.fixationLostTestCount=m_fixationLostTestCount;
    resultData.stimulusCount=m_stimulusCount;
    resultData.thresholdCount=m_thresholdCount;
    resultData.ambientLight=m_ambientLight;
    resultData.E_Light_pv=m_E_Light_pv;
    resultData.T_Light_pv=m_T_Light_pv;
    for(auto &i:m_fixationDeviation){resultData.fixationDeviation.push_back(i.toInt());}
    for(auto &i:m_checkData){resultData.checkdata.push_back(i.toInt());}
    return resultData;
}

ResultData<Type::Move> ResultDataVm::getMoveData()
{
    ResultData<Type::Move> resultData;
    resultData.alarm=m_alarm;
    resultData.falsePositiveCount=m_falsePositiveCount;
    resultData.falsePositiveTestCount=m_falsePositiveTestCount;
    resultData.falseNegativeCount=m_falseNegativeCount;
    resultData.falseNegativeTestCount=m_falseNegativeTestCount;
    resultData.fixationLostCount=m_fixationLostCount;
    resultData.fixationLostTestCount=m_fixationLostTestCount;
    resultData.stimulusCount=m_stimulusCount;
    resultData.thresholdCount=m_thresholdCount;
    resultData.ambientLight=m_ambientLight;
    resultData.E_Light_pv=m_E_Light_pv;
    resultData.T_Light_pv=m_T_Light_pv;
    for(auto &i:m_fixationDeviation){resultData.fixationDeviation.push_back(i.toInt());}
    for(auto &i:m_checkData){resultData.checkdata.push_back({float(i.toPoint().x()),float(i.toPoint().y())});}
    return resultData;
}
}
