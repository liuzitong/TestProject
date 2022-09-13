#include "progressAnalysislistVm.h"
#include <perimeter/main/database/checkResult.h>
#include <perimeter/main/services/analysis_svc.h>
#include <perimeter/main/viewModel/general_analysis_vm.h>
#include <QSharedPointer>
#include <QVector>

namespace Perimeter {

class BaseLineResult:public QObject
{
    Q_OBJECT
    Q_PROPERTY(float avgMD READ getAvgMD)
    Q_PROPERTY(float progressSpeedBase READ getProgressSpeedBase)
    Q_PROPERTY(float progressSpeedDeviation READ getProgressSpeedDeviation)
    Q_PROPERTY(int slopeType READ getSlopeType)

    float getAvgMD(){return m_avgMD;}
    float getProgressSpeedBase(){return m_progressSpeedBase;}
    float getProgressSpeedDeviation(){return m_progressSpeedDeviation;}
    int getSlopeType(){return m_slopeType;}
public:
    Q_INVOKABLE explicit BaseLineResult(float avgMD,float progressSpeedBase,float progressSpeedDeviation, int slopeType,QObject *parent=nullptr):
        QObject(parent),m_avgMD(avgMD),m_progressSpeedBase(progressSpeedBase),m_progressSpeedDeviation(progressSpeedDeviation),m_slopeType(slopeType){};
    Q_INVOKABLE virtual ~BaseLineResult(){qDebug()<<"delete";};
    Q_INVOKABLE void destroy(){this->~BaseLineResult();}
private:
    float m_avgMD;
    float m_progressSpeedBase;
    float m_progressSpeedDeviation;
    int m_slopeType;
};

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

QObject *ProgressAnalysisListVm::getProgressBaseLine(int imageSize)
{
    auto analysisSvc=AnalysisSvc::getSingleton();
    QVector<float> mdList;
    for(int i=0;i<m_currentDataList.length();i++)
    {
        mdList.push_back(m_currentDataList[i].md);
    }
//    float avgMd,progressSpeedBase,progressSpeedDeviation;int slopeType;


    int baseLineID1=m_currentDataList[0].checkResultId;
    int baseLineID2=m_currentDataList[1].checkResultId;

    QImage img=QImage({imageSize,imageSize}, QImage::Format_RGB32);

    auto drawProgressBaseLine=[&](int id,int index)->void
    {
        CheckResult_ptr checkResult_ptr(new CheckResult());
        checkResult_ptr->m_id=id;
        qx::dao::fetch_by_id(checkResult_ptr);
        Program_ptr program_ptr(new Program());
        program_ptr->m_id=checkResult_ptr->m_program->m_id;
        qx::dao::fetch_by_id(program_ptr);

        QVector<int> values,fixationValues,dev,mDev,peDev,peMDev;
        float md,psd,VFI,p_md,p_psd;
        int GHT,innerRange,range,OS_OD;

        auto checkResult=CheckResultModel<Type::ThreshHold>(checkResult_ptr);
        auto program=ProgramModel<Type::ThreshHold>(program_ptr);
        values.resize(checkResult.m_data.checkdata.size());
        for(int i=0;i<int(checkResult.m_data.checkdata.size());i++)
        {
            values[i]=checkResult.m_data.checkdata[i];
        }
        QVector<QPointF> locs;
        locs.resize(program.m_data.dots.size());
        for(int i=0;i<int(program.m_data.dots.size());i++)
        {
            locs[i]={program.m_data.dots[i].x,program.m_data.dots[i].y};
        }

        innerRange=program.m_params.commonParams.Range[0];
        range=program.m_params.commonParams.Range[1];
        m_OS_OD=checkResult.m_OS_OD;
        fixationValues.resize(checkResult.m_data.fixationDeviation.size());
        for(int i=0;i<int(checkResult.m_data.fixationDeviation.size());i++)
        {
            fixationValues[i]=checkResult.m_data.fixationDeviation[i];
        }

        analysisSvc->ThresholdAnalysis(id,dev,mDev,peDev,peMDev,md,psd,VFI,GHT,p_md,p_psd);
        analysisSvc->drawGray(values,locs,range,innerRange,img);img.save(m_previewFolder+"baseLine_gray"+QString::number(index)+".bmp");
        analysisSvc->drawText(values,locs,range,OS_OD,img);img.save(m_previewFolder+"baseLine_dBDiagram"+QString::number(index)+".bmp");
        analysisSvc->drawPE(peDev,locs,range,img);img.save(m_previewFolder+"baseLine_TotalPE"+QString::number(index)+".bmp");
        analysisSvc->drawPE(peMDev,locs,range,img);img.save(m_previewFolder+"baseLine_PatternPE"+QString::number(index)+".bmp");
    };

    drawProgressBaseLine(baseLineID1,1);
    drawProgressBaseLine(baseLineID2,2);

//    auto getMonth=[](QDateTime a,QDateTime b)->int
//    {
//        int months=(b.date().year()-a.date().year())*12+b.date().month()-a.date().month();
//        if(b.date().day()<a.date().day()) months--;
//        return months;
//    };


    auto getMonth=[](int a,QDateTime b)->int
    {
        int months=(b.date().year()-a)*12+b.date().month();
        return months;
    };

    int startYear=m_dateTimeList[0].date().year();
    QVector<int> months;
//    months.push_back(m_dateTimeList[0].date().month());
    for(int i=0;i<m_dateTimeList.length();i++)
    {
        int month=getMonth(startYear,m_dateTimeList[i]);
        months.push_back(month);
    }

    img=QImage({600,300}, QImage::Format_RGB32);
    analysisSvc->drawBaseLine(mdList,startYear,months,img);img.save(m_previewFolder+"baseLine.bmp");
    analysisSvc->BaseLineAnalysis(mdList,months,m_avgMd,m_progressSpeedBase,m_progressSpeedDeviation,m_slopeType);
    return new BaseLineResult(m_avgMd,m_progressSpeedBase,m_progressSpeedDeviation,m_slopeType);
}

void ProgressAnalysisListVm::getLastThreeProgress(int index,int imageSize)
{
    auto analysisSvc=AnalysisSvc::getSingleton();

    QVector<QVector<int>> value;
//    analysisSvc->ProgressAnalysis({m_currentDataList[0].MDev},{m_currentDataList[0].locs},0,value,value);
//    analysisSvc->ThreeFollowUpAnalysis({m_mDevList[index],m_mDevList[index-1],m_mDevList[index-2]},{m_locsList[index],m_locsList[index-1],m_locsList[index-2]},m_OS_OD,value);
}

QObject *Perimeter::ProgressAnalysisListVm::getSingleProgress(int index,int imageSize)
{
    QSharedPointer<StaticAnalysisVm> analysisResult(new StaticAnalysisVm(QVariantList{m_selectedResultId,imageSize,0}));
    return analysisResult->getResult();

}


void ProgressAnalysisListVm::removeCheckResult(int index)
{
    beginResetModel();
    m_currentDataList.removeAt(index);
    endResetModel();
}

void ProgressAnalysisListVm::switchEye()
{
    m_OS_OD==0?m_OS_OD=1:m_OS_OD=0;
    beginResetModel();
    generateDataList();
    endResetModel();
}

void ProgressAnalysisListVm::reset()
{
    beginResetModel();
    m_currentDataList=m_originalDataList;
    endResetModel();
}

void ProgressAnalysisListVm::showReport(int report)
{

}

void ProgressAnalysisListVm::generateDataList()
{
    m_originalDataList.clear();
    m_currentDataList.clear();
    qx_query query;
    CheckResult_List checkResult_List;
//    query.where("patient_id").isEqualTo(patientId).orderAsc("time");
//    query.where("patient_id").isEqualTo(patientId)/*.where("program_id").in({1,4})*/;
    query.where("patient_id").isEqualTo(m_patientId).and_("program_id").in({1,4}).and_("OS_OD").isEqualTo(m_OS_OD).orderAsc("time");
    qx::dao::fetch_by_query(query,checkResult_List);
//    m_mDevList.resize(checkResult_List.size());

    m_mDevList.clear();
    m_locsList.clear();
    m_dateTimeList.clear();

    for(int i=0;i<checkResult_List.size();i++)
    {
        m_originalDataList.push_back(getProgressData(checkResult_List[i]));
    }


    qDebug()<<m_originalDataList.length();
    qDebug()<<m_originalDataList[0].program;
    m_currentDataList=m_originalDataList;
    qDebug()<<m_currentDataList.length();
    setSelectedIndex(m_currentDataList.length()-1);
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
    bool centerDotCheck=checkResult.m_params.commonParams.centerDotCheck;
    auto analysisMethodSvc=AnalysisSvc::getSingleton();
    analysisMethodSvc->ThresholdAnalysis(resultId,dev,mDev,peDev,peMDev,md,psd,VFI,GHT,p_md,p_psd);
    QVector<QPointF> locs(program.m_data.dots.size());
    for(int i=0;i<int(program.m_data.dots.size());i++)
    {
        locs[i]={program.m_data.dots[i].x,program.m_data.dots[i].y};
    }
    m_mDevList.append(mDev);
    m_locsList.append(locs);
    m_dateTimeList.append(checkResult.m_time);

    QString time=checkResult.m_time.toString("yyyy/MM/dd HH:mm:ss");
    QString programName=program.m_name;
    auto strategy=checkResult.m_params.commonParams.strategy;
    QString strategyName=QVector<QString>{"全阈值","智能交互式","快速智能交互式","未知"}[int(strategy)<3?int(strategy):3];
    auto& resultData=checkResult.m_data;
    float fixationLossRate=resultData.fixationLostTestCount!=0?float(resultData.fixationLostCount)/resultData.fixationLostTestCount:0;
    float falseNegativeRate=resultData.falseNegativeTestCount!=0?float(resultData.falseNegativeCount)/resultData.falseNegativeTestCount:0;
    float falsePositiveRate=resultData.falsePositiveTestCount!=0?float(resultData.falsePositiveCount)/resultData.falsePositiveTestCount:0;

    return Data{checkResult.m_id,time,programName,strategyName,centerDotCheck,md,p_md,psd,p_psd,GHT,fixationLossRate,falseNegativeRate,falsePositiveRate,mDev,locs};
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
    case (Roles::dateTime): return data.dateTime;
    case (Roles::program): return data.program;
    case (Roles::strategy): return data.strategy;
    case (Roles::centerDotCheck): return data.centerDotCheck;
    case (Roles::md): return data.md;
    case (Roles::p_md): return data.p_md;
    case (Roles::psd): return data.psd;
    case (Roles::p_psd): return data.p_psd;
    case (Roles::GHT): return data.GHT;
    case (Roles::fixationLossRate): return data.fixationLossRate;
    case (Roles::falseNegativeRate): return data.falseNegativeRate;
    case (Roles::falsePositiveRate): return data.falsePositiveRate;
    default:return QVariant();

    }
}


QVariant ProgressAnalysisListVm::getData(int index, QString name)
{
    auto roleNames=this->roleNames();
    int role=roleNames.key(name.toStdString().c_str());
    return this->data(createIndex(index,0),role);
}

QHash<int, QByteArray> ProgressAnalysisListVm::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[checkResultId] = "checkResultId";
    roles[dateTime] = "dateTime";
    roles[program] = "program";
    roles[strategy] = "strategy";
    roles[centerDotCheck]="centerDotCheck";
    roles[md]="md";
    roles[p_md]="p_md";
    roles[psd]="psd";
    roles[p_psd]="p_psd";
    roles[GHT]="GHT";
    roles[fixationLossRate] = "fixationLossRate";
    roles[falseNegativeRate]="falseNegativeRate";
    roles[falsePositiveRate]="falsePositiveRate";
    return roles;
}
}

#include "progressAnalysislistVm.moc"
