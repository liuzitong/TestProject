#include "checkResultVm.h"
#include "paramsVm.h"
namespace Perimeter
{

CheckResultVm::CheckResultVm(const QVariantList & args)
{
    if(args[0].toString()=="Threshold")
    {
        m_type=0;
        m_params=new StaticParamsVM();
        m_resultData=new ResultDataVm();
    }
    else if(args[0].toString()=="Dynamic")
    {
        m_type=1;
        m_params=new DynamicParamsVM();
        m_resultData=new ResultDataVm();
    }
    else
    {
        int checkResult_id=args[0].toInt();
//        qx_query query("select * from CheckResult where checkResult_id=:checkResult_id");
//        query.bind(":checkResult_id",checkResult_id);
//        CheckResult_List checkResult_List;
//        QSqlError daoError = qx::dao::execute_query(query, checkResult_List);
//        CheckResult_ptr checkResult_ptr=checkResult_List.first();
        CheckResult_ptr checkResult_ptr(new CheckResult());
        checkResult_ptr->m_id=checkResult_id;
        qx::dao::fetch_by_id(checkResult_ptr);
        qDebug()<<checkResult_ptr->m_id;
        qDebug()<<checkResult_ptr->m_OS_OD;
        qDebug()<<checkResult_ptr->m_type;
        Type type=static_cast<Type>(checkResult_ptr->m_type);
        if(type!=Type::Dynamic)
        {
            auto checkResultModel=QSharedPointer<CheckResultModel<Type::ThreshHold>>(new CheckResultModel<Type::ThreshHold>(checkResult_ptr));
            m_id=checkResultModel->m_id;
            m_type=int(checkResultModel->m_type);
            m_OS_OD=checkResultModel->m_OS_OD;
            m_pic=checkResultModel->m_pic;
            m_diagnosis=checkResultModel->m_diagnosis;
            m_time=checkResultModel->m_time;
            m_params=new StaticParamsVM(checkResultModel->m_params);
            m_resultData=new ResultDataVm(checkResultModel->m_data);
            m_patient_id=checkResultModel->m_patient_id;
            m_program_id=checkResultModel->m_program_id;
        }
        else
        {
            auto checkResultModel=QSharedPointer<CheckResultModel<Type::Dynamic>>(new CheckResultModel<Type::Dynamic>(checkResult_ptr));
            m_id=checkResultModel->m_id;
            m_type=int(checkResultModel->m_type);
            m_OS_OD=checkResultModel->m_OS_OD;
            m_pic=checkResultModel->m_pic;
            m_diagnosis=checkResultModel->m_diagnosis;
            m_time=checkResultModel->m_time;
            m_params=new DynamicParamsVM(checkResultModel->m_params);
            m_resultData=new ResultDataVm(checkResultModel->m_data);
            m_patient_id=checkResultModel->m_patient_id;
            m_program_id=checkResultModel->m_program_id;
        }
    }

}

CheckResultVm::~CheckResultVm()
{
    delete m_params;
    delete m_resultData;
}

void CheckResultVm::insert()
{
     auto cp=getCheckResultData();
     QSqlError error=qx::dao::insert(cp);
}

void CheckResultVm::update()
{
    auto cp=getCheckResultData();
    QSqlError error=qx::dao::update(cp);
}


CheckResult_ptr CheckResultVm::getCheckResultData()
{
    if((m_type==int(Type::ThreshHold))||(m_type==int(Type::Screening)))
    {
        QSharedPointer<CheckResultModel<Type::ThreshHold>> checkResultModel=QSharedPointer<CheckResultModel<Type::ThreshHold>>(new CheckResultModel<Type::ThreshHold>());
        checkResultModel->m_id=m_id;
        checkResultModel->m_type=Type(m_type);
        checkResultModel->m_OS_OD=m_OS_OD;
        checkResultModel->m_pic=m_pic;
        checkResultModel->m_diagnosis=m_diagnosis;
        checkResultModel->m_time=m_time;
        checkResultModel->m_params.commonParams=static_cast<StaticParamsVM*>(m_params)->getCommonParams()->getData();
        checkResultModel->m_params.fixedParams=static_cast<StaticParamsVM*>(m_params)->getFixedParams()->getData();
        checkResultModel->m_data=m_resultData->getThresholdData();
        checkResultModel->m_patient_id=m_patient_id;
        checkResultModel->m_program_id=m_program_id;
        return checkResultModel->ModelToDB();
    }
    else
    {
        QSharedPointer<CheckResultModel<Type::Dynamic>> checkResultModel=QSharedPointer<CheckResultModel<Type::Dynamic>>(new CheckResultModel<Type::Dynamic>());
        checkResultModel->m_id=m_id;
        checkResultModel->m_type=Type(m_type);
        checkResultModel->m_OS_OD=m_OS_OD;
        checkResultModel->m_pic=m_pic;
        checkResultModel->m_diagnosis=m_diagnosis;
        checkResultModel->m_time=m_time;
        checkResultModel->m_params=static_cast<DynamicParamsVM*>(m_params)->getData();
        checkResultModel->m_data=m_resultData->getDynamicData();
        checkResultModel->m_patient_id=m_patient_id;
        checkResultModel->m_program_id=m_program_id;
        return checkResultModel->ModelToDB();
    }
}



ResultDataVm::ResultDataVm(ResultData<Type::ThreshHold> resultData)
{
    m_alarm=resultData.alarm;
    m_pupilDiameter=resultData.pupilDiameter;
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

ResultDataVm::ResultDataVm(ResultData<Type::Dynamic> resultData)
{
    m_alarm=resultData.alarm;
    m_pupilDiameter=resultData.pupilDiameter;
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
    resultData.pupilDiameter=m_pupilDiameter;
    resultData.falsePositiveCount=m_falsePositiveCount;
    resultData.falsePositiveTestCount=m_falsePositiveTestCount;
    resultData.falseNegativeCount=m_falseNegativeCount;
    resultData.falseNegativeTestCount=m_falseNegativeTestCount;
    resultData.fixationLostCount=m_fixationLostCount;
    resultData.fixationLostTestCount=m_fixationLostTestCount;
    resultData.stimulusCount=m_stimulusCount;
    resultData.thresholdCount=m_thresholdCount;
    resultData.ambientLight=m_ambientLight;
    resultData.testTimespan=m_testTimespan;
    resultData.E_Light_pv=m_E_Light_pv;
    resultData.T_Light_pv=m_T_Light_pv;
    for(auto &i:m_fixationDeviation){resultData.fixationDeviation.push_back(i.toInt());}
    for(auto &i:m_checkData){resultData.checkdata.push_back(i.toInt());}
    return resultData;
}

ResultData<Type::Dynamic> ResultDataVm::getDynamicData()
{
    ResultData<Type::Dynamic> resultData;
    resultData.alarm=m_alarm;
    resultData.pupilDiameter=m_pupilDiameter;
    resultData.falsePositiveCount=m_falsePositiveCount;
    resultData.falsePositiveTestCount=m_falsePositiveTestCount;
    resultData.falseNegativeCount=m_falseNegativeCount;
    resultData.falseNegativeTestCount=m_falseNegativeTestCount;
    resultData.fixationLostCount=m_fixationLostCount;
    resultData.fixationLostTestCount=m_fixationLostTestCount;
    resultData.stimulusCount=m_stimulusCount;
    resultData.thresholdCount=m_thresholdCount;
    resultData.ambientLight=m_ambientLight;
    resultData.testTimespan=m_testTimespan;
    resultData.E_Light_pv=m_E_Light_pv;
    resultData.T_Light_pv=m_T_Light_pv;
    for(auto &i:m_fixationDeviation){resultData.fixationDeviation.push_back(i.toInt());}
    for(auto &i:m_checkData){resultData.checkdata.push_back({float(i.toPoint().x()),float(i.toPoint().y())});}
    return resultData;
}
}
