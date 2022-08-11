#include "progressAnalysislistVm.h"
#include <perimeter/main/database/checkResult.h>
#include <perimeter/main/services/analysis_svc.h>

namespace Perimeter {

ProgressAnalysisListVm::ProgressAnalysisListVm(const QVariantList &args)
{
    qDebug()<<"into constructor";
    m_patientId=args[0].toInt();
    m_OS_OD=args[1].toInt();
    generateDataList();
}

ProgressAnalysisListVm::~ProgressAnalysisListVm()
{
}

void ProgressAnalysisListVm::removeCheckResult(int checkResultId)
{

}

void ProgressAnalysisListVm::switchEye()
{
    m_OS_OD==0?m_OS_OD=1:m_OS_OD=0;
    beginResetModel();
    generateDataList();
    endResetModel();
}

void ProgressAnalysisListVm::generateDataList()
{
    m_originalDataList.clear();
    m_currentDataList.clear();
    qx_query query;
    CheckResult_List checkResult_List;
//    query.where("patient_id").isEqualTo(patientId).orderAsc("time");
//    query.where("patient_id").isEqualTo(patientId)/*.where("program_id").in({1,4})*/;
    query.where("patient_id").isEqualTo(m_patientId).and_("program_id").in({1,4}).and_("OS_OD").isEqualTo(m_OS_OD);
    qx::dao::fetch_by_query(query,checkResult_List);

    for(int i=0;i<checkResult_List.size();i++)
    {
        m_originalDataList.push_back(getProgressData(checkResult_List[i]));
    }

    qDebug()<<m_originalDataList.length();
    qDebug()<<m_originalDataList[0].program;
    m_currentDataList=m_originalDataList;
    m_currentDataList[0].baseLine=true;
    m_currentDataList[1].baseLine=true;
    qDebug()<<m_currentDataList.length();
}


ProgressAnalysisListVm::Data ProgressAnalysisListVm::getProgressData(CheckResult_ptr checkResult_ptr)
{
    auto checkResult=CheckResultModel<Type::ThreshHold>(checkResult_ptr);
    Program_ptr program_ptr(new Program());
    program_ptr->m_id=checkResult_ptr->m_program->m_id;
    qx::dao::fetch_by_id(program_ptr);
    auto program=ProgramModel<Type::ThreshHold>(program_ptr);

    QVector<int> dev,mDev,peDev,peMDev;
    float md,psd,VFI,p_md,p_psd;
    int GHT;
    int resultId=checkResult.m_id;
    auto analysisMethodSvc=AnalysisSvc::getSingleton();
    analysisMethodSvc->ThresholdAnalysis(resultId,dev,mDev,peDev,peMDev,md,psd,VFI,GHT,p_md,p_psd);

    QString time=checkResult.m_time.toString("yyyy/MM/dd HH:mm:ss");
    QString programName=program.m_name;
    auto strategy=checkResult.m_params.commonParams.strategy;
    QString strategyName=QVector<QString>{"全阈值","智能交互式","快速智能交互式","未知"}[int(strategy)<3?int(strategy):3];
    auto& resultData=checkResult.m_data;
    float fixationLossRate=resultData.fixationLostTestCount!=0?float(resultData.fixationLostCount)/resultData.fixationLostTestCount:0;
    float falseNegativeRate=resultData.falseNegativeTestCount!=0?float(resultData.falseNegativeCount)/resultData.falseNegativeTestCount:0;
    float falsePositiveRate=resultData.falsePositiveTestCount!=0?float(resultData.falsePositiveCount)/resultData.falsePositiveTestCount:0;

    return Data{checkResult.m_id,false,time,programName,strategyName,md,psd,fixationLossRate,falseNegativeRate,falsePositiveRate};
}

int ProgressAnalysisListVm::rowCount(const QModelIndex &parent) const
{
    return m_currentDataList.length();
}

QVariant ProgressAnalysisListVm::data(const QModelIndex &index, int role) const
{

    int row=index.row();
    auto data=m_currentDataList[row];
//    typedef  ProgressAnalysisListVm::ProgressAnalysisRoles Roles;
    using Roles = ProgressAnalysisListVm::ProgressAnalysisRoles ;
    switch (role)
    {
    case (Roles::checkResultId):return (int)data.checkResultId;
    case (Roles::baseLine):return data.baseLine;
    case (Roles::dateTime): return data.dateTime;
    case (Roles::program): return data.program;
    case (Roles::md): return data.md;
    case (Roles::psd): return data.psd;
    case (Roles::strategy): return data.strategy;
    case (Roles::fixationLossRate): return data.fixationLossRate;
    case (Roles::falseNegativeRate): return data.falseNegativeRate;
    case (Roles::falsePositiveRate): return data.falsePositiveRate;
    default:return QVariant();

    }
}

QHash<int, QByteArray> ProgressAnalysisListVm::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[checkResultId] = "checkResultId";
    roles[baseLine]="baseLine";
    roles[dateTime] = "dateTime";
    roles[program] = "program";
    roles[strategy] = "strategy";
    roles[md]="md";
    roles[psd]="psd";
    roles[fixationLossRate] = "fixationLossRate";
    roles[falseNegativeRate]="falseNegativeRate";
    roles[falsePositiveRate]="falsePositiveRate";
    return roles;
}
}
