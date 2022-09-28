#include "progressAnalysislistVm.h"
#include <perimeter/main/database/checkResult.h>
#include <perimeter/main/services/analysis_svc.h>
#include <perimeter/main/viewModel/general_analysis_vm.h>
#include <QSharedPointer>
#include <QVector>
#include "perimeter/main/appctrl/perimeter_appctrl.hxx"
#include "perimeter/third-part/qxpack/indcom/ui_qml_base/qxpack_ic_ui_qml_api.hxx"


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
    Q_INVOKABLE virtual ~BaseLineResult()=default;
    Q_INVOKABLE void destroy(){this->~BaseLineResult();}
private:
    float m_avgMD;
    float m_progressSpeedBase;
    float m_progressSpeedDeviation;
    int m_slopeType;
};


ProgressAnalysisListVm::ProgressAnalysisListVm(const QVariantList &args)
{
    Patient_ptr patient_ptr(new Patient());
    patient_ptr->m_id= args[0].toInt();
    qx::dao::fetch_by_id(patient_ptr);
    m_patient=PatientModel(patient_ptr);
    m_OS_OD=args[1].toInt();
    generateDataList();
}

ProgressAnalysisListVm::~ProgressAnalysisListVm()
{

}

QObject *ProgressAnalysisListVm::getProgressBaseLinePreview(int imageSize)
{
    auto analysisSvc=AnalysisSvc::getSingleton();
    QVector<float> mdList;
    float avgMd,progressSpeedBase,progressSpeedDeviation;int slopeType;
    for(int i=0;i<m_currentDataList.length();i++)
    {
        mdList.push_back(m_currentDataList[i].md);
    }

    QImage img=QImage({imageSize,imageSize}, QImage::Format_RGB32);

    auto drawProgressBaseLine=[&](int index)->void
    {

        QVector<int> values,peDev,peMDev;
        int innerRange=0,range=30,OS_OD;
        QVector<QPointF> locs;

        values=m_currentDataList[index].values;
        locs=m_currentDataList[index].locs;
        peDev=m_currentDataList[index].peDev;
        peMDev=m_currentDataList[index].peMDev;

        analysisSvc->drawGray(values,locs,range,innerRange,img);img.save(m_previewFolder+"baseLine_gray"+QString::number(index)+".bmp");
        analysisSvc->drawText(values,locs,range,OS_OD,img);img.save(m_previewFolder+"baseLine_dBDiagram"+QString::number(index)+".bmp");
        analysisSvc->drawPE(peDev,locs,range,img);img.save(m_previewFolder+"baseLine_TotalPE"+QString::number(index)+".bmp");
        analysisSvc->drawPE(peMDev,locs,range,img);img.save(m_previewFolder+"baseLine_PatternPE"+QString::number(index)+".bmp");
    };

    drawProgressBaseLine(0);
    drawProgressBaseLine(1);

    auto getMonth=[](int a,QDateTime b)->int
    {
        int months=(b.date().year()-a)*12+b.date().month();
        return months;
    };

    int startYear=m_currentDataList[0].dateTime.date().year();
    QVector<int> months;
    for(int i=0;i<m_currentDataList.length();i++)
    {
        int month=getMonth(startYear,m_currentDataList[i].dateTime);
        months.push_back(month);
    }

    img=QImage({600,300}, QImage::Format_RGB32);
    analysisSvc->drawBaseLine(mdList,startYear,months,img);img.save(m_previewFolder+"baseLine.bmp");
    analysisSvc->BaseLineAnalysis(mdList,months,avgMd,progressSpeedBase,progressSpeedDeviation,slopeType);
    return new BaseLineResult(avgMd,progressSpeedBase,progressSpeedDeviation,slopeType);
}

QVariantList ProgressAnalysisListVm::getThreeFollowUpsPreview(int index,int imageSize)
{
    auto analysisSvc=AnalysisSvc::getSingleton();


    QVector<QVector<int>> val,mPE,mDev,progressVal,progressPicVal;
    QVector<int> progress;
    QVector<QVector<QPointF> > locs,progressLocs;
    QVariantList progressList;
    if(m_currentDataList.length()<3) return progressList;

    mDev={m_currentDataList[0].mDev,m_currentDataList[1].mDev};
    locs={m_currentDataList[0].locs,m_currentDataList[1].locs};

    for(int i=qMax(index-2,2);i<=index;i++)
    {
        val.append(m_currentDataList[i].values);
        mPE.append(m_currentDataList[i].peMDev);
        mDev.append(m_currentDataList[i].mDev);
        locs.append(m_currentDataList[i].locs);
    }

    analysisSvc->ProgressAnalysis(mDev,locs,m_OS_OD,progressLocs,progressVal,progressPicVal,progress);

    QImage img=QImage({imageSize,imageSize}, QImage::Format_RGB32);

    for(int i=0;i<progressVal.length();i++)
    {
        analysisSvc->drawGray(val[i],locs[i],30,m_OS_OD,img);img.save(m_previewFolder+"threeFollowUps_grey"+QString::number(i)+".bmp");
        analysisSvc->drawPE(mPE[i],locs[i],30,img);img.save(m_previewFolder+"threeFollowUps_PatternPE"+QString::number(i)+".bmp");
        analysisSvc->drawText(progressVal[i],progressLocs[i],30,m_OS_OD,img);img.save(m_previewFolder+"threeFollowUps_progressVal"+QString::number(i)+".bmp");
        analysisSvc->drawProgess(progressPicVal[i],progressLocs[i],30,img);img.save(m_previewFolder+"threeFollowUps_progressPic"+QString::number(i)+".bmp");
    }

    for(int i=0;i<progress.length();i++)
    {
        progressList.append(QVariant(progress[i]));
    }
    return progressList;
}

QVariant Perimeter::ProgressAnalysisListVm::getSingleProgressPreview(int index,int imageSize)
{
    auto analysisSvc=AnalysisSvc::getSingleton();


    QVector<QVector<int>> val,mPE,mDev,progressVal,progressPicVal;
    QVector<int> progress;
    QVector<QVector<QPointF> > locs,progressLocs;
    QVariantList progressList;
    if(m_currentDataList.length()<3) return progressList;

    mDev={m_currentDataList[0].mDev,m_currentDataList[1].mDev};
    locs={m_currentDataList[0].locs,m_currentDataList[1].locs};

    for(int i=qMax(index-2,2);i<=index;i++)
    {
        val.append(m_currentDataList[i].values);
        mPE.append(m_currentDataList[i].peMDev);
        mDev.append(m_currentDataList[i].mDev);
        locs.append(m_currentDataList[i].locs);
    }

    analysisSvc->ProgressAnalysis(mDev,locs,m_OS_OD,progressLocs,progressVal,progressPicVal,progress);

    QImage img=QImage({imageSize,imageSize}, QImage::Format_RGB32);

    analysisSvc->drawProgess(progressPicVal.last(),progressLocs.last(),30,img);img.save(m_previewFolder+"single_progressPic.bmp");

    return QVariant(progress.last());
}

void ProgressAnalysisListVm::getProgressBaseLineReport()
{
    auto analysisSvc=AnalysisSvc::getSingleton();
    QVector<float> mdList;
    float avgMd,progressSpeedBase,progressSpeedDeviation;int slopeType;
    for(int i=0;i<m_currentDataList.length();i++)
    {
        mdList.push_back(m_currentDataList[i].md);
    }

    QImage img=QImage({420,420}, QImage::Format_RGB32);

    auto drawProgressBaseLine=[&](int index)->void
    {

        QVector<int> values,peDev,peMDev;
        int innerRange=0,range=30,OS_OD;
        QVector<QPointF> locs;

        values=m_currentDataList[index].values;
        locs=m_currentDataList[index].locs;
        peDev=m_currentDataList[index].peDev;
        peMDev=m_currentDataList[index].peMDev;

        analysisSvc->drawGray(values,locs,range,innerRange,img);img.save(m_reportFolder+"baseLine_gray"+QString::number(index)+".bmp");
        analysisSvc->drawText(values,locs,range,OS_OD,img);img.save(m_reportFolder+"baseLine_dBDiagram"+QString::number(index)+".bmp");
        analysisSvc->drawPE(peDev,locs,range,img);img.save(m_reportFolder+"baseLine_TotalPE"+QString::number(index)+".bmp");
        analysisSvc->drawPE(peMDev,locs,range,img);img.save(m_reportFolder+"baseLine_PatternPE"+QString::number(index)+".bmp");
    };

    drawProgressBaseLine(0);
    drawProgressBaseLine(1);

    auto getMonth=[](int a,QDateTime b)->int
    {
        int months=(b.date().year()-a)*12+b.date().month();
        return months;
    };

    int startYear=m_currentDataList[0].dateTime.date().year();
    QVector<int> months;
    for(int i=0;i<m_currentDataList.length();i++)
    {
        int month=getMonth(startYear,m_currentDataList[i].dateTime);
        months.push_back(month);
    }

    img=QImage({1200,600}, QImage::Format_RGB32);
    analysisSvc->drawBaseLine(mdList,startYear,months,img);img.save(m_reportFolder+"baseLine.bmp");
    analysisSvc->BaseLineAnalysis(mdList,months,avgMd,progressSpeedBase,progressSpeedDeviation,slopeType);


    auto reportEngine = QSharedPointer<LimeReport::ReportEngine>(new LimeReport::ReportEngine());
    reportEngine->loadFromFile("./reports/baseLine.lrxml");
    auto manager=reportEngine->dataManager();
    manager->clearUserVariables();
    manager->setReportVariable("hospitalName",QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("hospitalName").toString());
    manager->setReportVariable("name",m_patient.m_name);
    manager->setReportVariable("birthDate",m_patient.m_birthDate.toString("yyyy/MM/dd"));
    manager->setReportVariable("sex", int(m_patient.m_sex)==0?"male":"female");
    manager->setReportVariable("ID", m_patient.m_patientId);
    manager->setReportVariable("age", m_patient.m_age);
    manager->setReportVariable("OS_OD",m_OS_OD==0?"OS":"OD");

    for(int i=0;i<2;i++)
    {
        QString GHTstr[]={"Out of limits","Low sensitivity","Border of limits","Within normal limits"};
        manager->setReportVariable("Date"+QString::number(i),"Date:"+m_currentDataList[i].dateTime.date().toString("yyyy/MM/dd"));
        manager->setReportVariable("programName"+QString::number(i),"Program:"+m_currentDataList[i].program);
        manager->setReportVariable("GHT"+QString::number(i),"GHT:"+GHTstr[m_currentDataList[i].GHT]);
        manager->setReportVariable("centerDotCheck"+QString::number(i),QString("Fovea: ")+(m_currentDataList[i].centerDotCheck?"ON":"OFF"));
        manager->setReportVariable("MD"+QString::number(i),QString("MD: ")+QString::number(m_currentDataList[i].md,'f',2)+(m_currentDataList[i].p_md-5<=FLT_EPSILON?" (P<"+QString::number(m_currentDataList[i].p_md)+"%)":""));
        manager->setReportVariable("PSD"+QString::number(i),QString("PSD: ")+QString::number(m_currentDataList[i].psd,'f',2)+(m_currentDataList[i].p_psd-5<=FLT_EPSILON?" (P<"+QString::number(m_currentDataList[i].p_psd)+"%)":""));
        manager->setReportVariable("fixationLosses"+QString::number(i),"FixationLossRate: "+QString::number(m_currentDataList[i].fixationLostCount)+"/"+QString::number(m_currentDataList[i].fixationLostTestCount));
        manager->setReportVariable("falsePositiveRate"+QString::number(i),"FalsePositiveRate: "+QString::number(qRound(m_currentDataList[i].falsePositiveRate*100))+"%");
        manager->setReportVariable("falseNegativeRate"+QString::number(i),"FalseNegativeRate: "+QString::number(qRound(m_currentDataList[i].falseNegativeRate*100))+"%");

        manager->setReportVariable("baseLine_gray"+QString::number(i),m_reportFolder+"baseLine_gray"+QString::number(i)+".bmp");
        manager->setReportVariable("baseLine_DB"+QString::number(i),m_reportFolder+"baseLine_dBDiagram"+QString::number(i)+".bmp");
        manager->setReportVariable("baseLine_totalPE"+QString::number(i),m_reportFolder+"baseLine_TotalPE"+QString::number(i)+".bmp");
        manager->setReportVariable("baseLine_patternPE"+QString::number(i),m_reportFolder+"baseLine_PatternPE"+QString::number(i)+".bmp");
    }

    manager->setReportVariable("baseLine","./reportImage/baseLine.bmp");

    manager->setReportVariable("ProgressSpeed","ProgressSpeed: "+QString::number(progressSpeedBase,'f',2)+"±"+QString::number(progressSpeedDeviation,'f',2));
    QString slopeStr[]={"slope insignificant","slope significant"};
    manager->setReportVariable("Slope","SlopeStatus: "+slopeStr[slopeType]);
    manager->setReportVariable("AvgMD","AverageMD: "+QString::number(avgMd));

    manager->setReportVariable("DoctorSign","DoctorSign: ");
//    manager->setReportVariable("DiagnosisContent", m_checkResult.m_diagnosis);
    manager->setReportVariable("deviceInfo","Device Info: "+QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("deviceInfo").toString());
    manager->setReportVariable("version", "Version: "+QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("version").toString());


    reportEngine->setShowProgressDialog(true);
    reportEngine->setPreviewScaleType(LimeReport::ScaleType::Percents,50);
    reportEngine->previewReport(/*LimeReport::PreviewHint::ShowAllPreviewBars*/);

}

void Perimeter::ProgressAnalysisListVm::getThreeFollowUpsReport(int index)
{
    auto analysisSvc=AnalysisSvc::getSingleton();
    QVector<QVector<int>> val,mPE,mDev,progressVal,progressPicVal;
    QVector<int> progress;
    QVector<QVector<QPointF> > locs,progressLocs;
    QVariantList progressList;

    mDev={m_currentDataList[0].mDev,m_currentDataList[1].mDev};
    locs={m_currentDataList[0].locs,m_currentDataList[1].locs};

    for(int i=qMax(index-2,2);i<=index;i++)
    {
        val.append(m_currentDataList[i].values);
        mPE.append(m_currentDataList[i].peMDev);
        mDev.append(m_currentDataList[i].mDev);
        locs.append(m_currentDataList[i].locs);
    }

    analysisSvc->ProgressAnalysis(mDev,locs,m_OS_OD,progressLocs,progressVal,progressPicVal,progress);

    QImage img=QImage({420,420}, QImage::Format_RGB32);

    for(int i=0;i<progressVal.length();i++)
    {
        analysisSvc->drawGray(val[i],locs[i],30,m_OS_OD,img);img.save(m_reportFolder+"threeFollowUps_grey"+QString::number(i)+".bmp");
        analysisSvc->drawPE(mPE[i],locs[i],30,img);img.save(m_reportFolder+"threeFollowUps_PatternPE"+QString::number(i)+".bmp");
        analysisSvc->drawText(progressVal[i],progressLocs[i],30,m_OS_OD,img);img.save(m_reportFolder+"threeFollowUps_progressVal"+QString::number(i)+".bmp");
        analysisSvc->drawProgess(progressPicVal[i],progressLocs[i],30,img);img.save(m_reportFolder+"threeFollowUps_progressPic"+QString::number(i)+".bmp");
    }

    for(int i=0;i<progress.length();i++)
    {
        progressList.append(QVariant(progress[i]));
    }


    auto reportEngine = QSharedPointer<LimeReport::ReportEngine>(new LimeReport::ReportEngine());
    reportEngine->loadFromFile("./reports/threeFollowUps.lrxml");
    auto manager=reportEngine->dataManager();
    manager->clearUserVariables();
    manager->setReportVariable("hospitalName",QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("hospitalName").toString());
    manager->setReportVariable("name",m_patient.m_name);
    manager->setReportVariable("birthDate",m_patient.m_birthDate.toString("yyyy/MM/dd"));
    manager->setReportVariable("sex", int(m_patient.m_sex)==0?"male":"female");
    manager->setReportVariable("ID", m_patient.m_patientId);
    manager->setReportVariable("age", m_patient.m_age);
    manager->setReportVariable("OS_OD",m_OS_OD==0?"OS":"OD");

    for(int i=0;i<progressVal.length();i++)
    {
        int dataIndex=qMax(index-2,2)+i;
        auto data=m_currentDataList[dataIndex];
        QString GHTstr[]={"Out of limits","Low sensitivity","Border of limits","Within normal limits"};
        manager->setReportVariable("Date"+QString::number(i),"Date:"+data.dateTime.date().toString("yyyy/MM/dd"));
        manager->setReportVariable("programName"+QString::number(i),"Program:"+data.program);
        manager->setReportVariable("GHT"+QString::number(i),"GHT:"+GHTstr[data.GHT]);
        manager->setReportVariable("centerDotCheck"+QString::number(i),QString("Fovea: ")+(data.centerDotCheck?"ON":"OFF"));
        manager->setReportVariable("MD"+QString::number(i),QString("MD: ")+QString::number(data.md,'f',2)+(data.p_md-5<=FLT_EPSILON?" (P<"+QString::number(data.p_md)+"%)":""));
        manager->setReportVariable("PSD"+QString::number(i),QString("PSD: ")+QString::number(data.psd,'f',2)+(data.p_psd-5<=FLT_EPSILON?" (P<"+QString::number(data.p_psd)+"%)":""));
        QString progressStr[]={"no progress","possible progress","very possible progress"};
        manager->setReportVariable("progress"+QString::number(i),QString("progress: ")+progressStr[progress[i]]);
        manager->setReportVariable("fixationLosses"+QString::number(i),"FixationLossRate: "+QString::number(data.fixationLostCount)+"/"+QString::number(data.fixationLostTestCount));
        manager->setReportVariable("falsePositiveRate"+QString::number(i),"FalsePositiveRate: "+QString::number(qRound(data.falsePositiveRate*100))+"%");
        manager->setReportVariable("falseNegativeRate"+QString::number(i),"FalseNegativeRate: "+QString::number(qRound(data.falseNegativeRate*100))+"%");

        manager->setReportVariable("threeFollowUps_grey"+QString::number(i),m_reportFolder+"threeFollowUps_grey"+QString::number(i)+".bmp");
        manager->setReportVariable("threeFollowUps_PatternPE"+QString::number(i),m_reportFolder+"threeFollowUps_PatternPE"+QString::number(i)+".bmp");
        manager->setReportVariable("threeFollowUps_progressVal"+QString::number(i),m_reportFolder+"threeFollowUps_progressVal"+QString::number(i)+".bmp");
        manager->setReportVariable("threeFollowUps_progressPic"+QString::number(i),m_reportFolder+"threeFollowUps_progressPic"+QString::number(i)+".bmp");
    }
    manager->setReportVariable("baseLine_MD","baseLine average MD:"+QString::number((m_currentDataList[0].md+m_currentDataList[1].md)/2,'f',2));
    manager->setReportVariable("baseLine_Dates","   "+m_currentDataList[0].dateTime.date().toString("yyyy/MM/dd")+"   "+m_currentDataList[1].dateTime.date().toString("yyyy/MM/dd"));

    manager->setReportVariable("DoctorSign","DoctorSign: ");
//    manager->setReportVariable("DiagnosisContent", m_checkResult.m_diagnosis);
    manager->setReportVariable("deviceInfo","Device Info: "+QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("deviceInfo").toString());
    manager->setReportVariable("version", "Version: "+QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("version").toString());


    reportEngine->setShowProgressDialog(true);
    reportEngine->setPreviewScaleType(LimeReport::ScaleType::Percents,50);
    reportEngine->previewReport(/*LimeReport::PreviewHint::ShowAllPreviewBars*/);
}

void Perimeter::ProgressAnalysisListVm::getSingleProgressReport(int index)
{
    CheckResult_ptr checkResult_ptr(new CheckResult());
    checkResult_ptr->m_id=m_selectedResultId;
    qx::dao::fetch_by_id(checkResult_ptr);
    auto checkResult=CheckResultModel<Type::ThreshHold>(checkResult_ptr);


    QVector<int> single_dev,single_mDev,single_peDev,single_peMDev;
    float md,psd,VFI,p_md,p_psd;
    int GHT;
    auto analysisSvc=AnalysisSvc::getSingleton();
    analysisSvc->ThresholdAnalysis(m_selectedResultId,single_dev,single_mDev,single_peDev,single_peMDev,md,psd,VFI,GHT,p_md,p_psd);

    QVector<QVector<int>> val,mPE,mDev,progressVal,progressPicVal;
    QVector<int> progress;
    QVector<QVector<QPointF> > locs,progressLocs;
    QVariantList progressList;

    mDev={m_currentDataList[0].mDev,m_currentDataList[1].mDev};
    locs={m_currentDataList[0].locs,m_currentDataList[1].locs};

    for(int i=qMax(index-2,2);i<=index;i++)
    {
        val.append(m_currentDataList[i].values);
        mPE.append(m_currentDataList[i].peMDev);
        mDev.append(m_currentDataList[i].mDev);
        locs.append(m_currentDataList[i].locs);
    }

    analysisSvc->ProgressAnalysis(mDev,locs,m_OS_OD,progressLocs,progressVal,progressPicVal,progress);

    QImage img=QImage({500,500}, QImage::Format_RGB32);
    analysisSvc->drawText(val.last(),locs.last(),30,m_OS_OD,img);img.save(m_reportFolder+"singleProgress_dBDiagram.bmp");
    analysisSvc->drawGray(val.last(),locs.last(),30,0,img);img.save(m_reportFolder+"singleProgress_gray.bmp");

    analysisSvc->drawText(m_currentDataList[index].dev,locs.last(),30,m_OS_OD,img);img.save(m_reportFolder+"singleProgress_TotalDeviation.bmp");
    analysisSvc->drawText(m_currentDataList[index].mDev,locs.last(),30,m_OS_OD,img);img.save(m_reportFolder+"singleProgress_PatternDeviation.bmp");

    analysisSvc->drawPE(m_currentDataList[index].peDev,locs.last(),30,img);img.save(m_reportFolder+"singleProgress_TotalPE.bmp");
    analysisSvc->drawPE(m_currentDataList[index].peMDev,locs.last(),30,img);img.save(m_reportFolder+"singleProgress_PatternPE.bmp");

    analysisSvc->drawProgess(progressPicVal.last(),progressLocs.last(),30,img);img.save(m_reportFolder+"singleProgress.bmp");

    QVector<int> fixationValues;
    for(int i=0;i<int(checkResult.m_data.fixationDeviation.size());i++)
    {
        fixationValues.append(checkResult.m_data.fixationDeviation[i]);
    }

    QImage imgFixation=QImage({322*2,27*2}, QImage::Format_RGB32);
    analysisSvc->drawFixationDeviation(fixationValues,imgFixation);imgFixation.save(m_reportFolder+"FixationDeviation.bmp");


    auto reportEngine = QSharedPointer<LimeReport::ReportEngine>(new LimeReport::ReportEngine());
    reportEngine->loadFromFile("./reports/SingleProgress.lrxml");
    auto manager=reportEngine->dataManager();
    manager->clearUserVariables();
    manager->setReportVariable("ProgramName",m_currentDataList[index].program);
    manager->setReportVariable("OS_OD",m_OS_OD==0?"OS":"OD");
    manager->setReportVariable("hospitalName",QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("hospitalName").toString());
    manager->setReportVariable("name",m_patient.m_name);
    manager->setReportVariable("birthDate",m_patient.m_birthDate.toString("yyyy/MM/dd"));
    manager->setReportVariable("checkDate", m_currentDataList[index].dateTime.date().toString("yyyy/MM/dd"));
    manager->setReportVariable("ID", m_patient.m_patientId);
    manager->setReportVariable("age", m_patient.m_age);
    manager->setReportVariable("checkTime", m_currentDataList[index].dateTime.time().toString("H:mm:ss"));
    manager->setReportVariable("sex", int(m_patient.m_sex)==0?"male":"female");
    auto rx=m_patient.m_rx;
    if(m_OS_OD)
    {
        manager->setReportVariable("Rx_Ry","Rx:"+QString::number(rx.rx1_l,'f',2)+"DS "+QString::number(rx.rx2_l,'f',2)+"DC "+QString::number(rx.rx3_l,'f',2)+"X");
        manager->setReportVariable("visualAcuity","visualAcuity:"+QString::number(rx.visual_l,'f',2));
    }
    else
    {
        manager->setReportVariable("Rx_Ry","Rx:"+QString::number(rx.rx1_r,'f',2)+"DS "+QString::number(rx.rx2_r,'f',2)+"DC "+QString::number(rx.rx3_r,'f',2)+"X");
        manager->setReportVariable("visualAcuity","visualAcuity:"+QString::number(rx.visual_r,'f',2));
    }
    manager->setReportVariable("pupilDiameter","pupilDiameter:"+QString::number(checkResult.m_data.pupilDiameter,'f',2));

    auto commomParams=checkResult.m_params.commonParams;
    QString fixationMonitor;switch (int(commomParams.fixationMonitor)) {case 0:fixationMonitor="No Alarm"; break;case 1:fixationMonitor="Only Alarm";break;case 2:fixationMonitor="Pause And Alarm";break;}
    QString fixationTarget;switch (int(commomParams.fixationTarget)){case 0:fixationTarget="Center Point";break;case 1:fixationTarget="Small Diamond";break;case 2:fixationTarget="Big Diamond";break;case 3:fixationTarget="Bottom Point";break;}
    manager->setReportVariable("fixationMonitor","Fixation Monitor: "+fixationMonitor);
    manager->setReportVariable("fixationTarget","Fixation Target: "+fixationTarget);
    auto resultData=checkResult.m_data;
    manager->setReportVariable("fixationLosses","Fixation Losses: "+QString::number(resultData.fixationLostCount)+"/"+QString::number(resultData.fixationLostTestCount));
    manager->setReportVariable("falsePositiveRate","False POS Errors: "+QString::number(qRound(float(resultData.falsePositiveCount)/resultData.falsePositiveTestCount*100))+"%");
    manager->setReportVariable("falseNegativeRate","False NEG Errors: "+QString::number(qRound(float(resultData.falseNegativeCount)/resultData.falseNegativeTestCount*100))+"%");
    QTime time;time.setHMS(0,0,0);time=time.addSecs(resultData.testTimespan);
    manager->setReportVariable("testTime","Test Duration: "+time.toString("mm:ss"));
    manager->setReportVariable("centerDotCheck",QString("Fovea: ")+(commomParams.centerDotCheck?"ON":"OFF"));
    QString cursorSize;switch(int(commomParams.cursorSize)){case 0:cursorSize="I";break;case 1:cursorSize="II";break;case 2:cursorSize="III";break;case 3:cursorSize="IV";break;case 4:cursorSize="V";break;}
    QString cursorColor;switch(int(commomParams.cursorColor)){case 0:cursorColor="White";break;case 1:cursorColor="Red";break;case 2:cursorColor="Blue";break;}
    manager->setReportVariable("stimCursor","Stimulus: "+cursorSize+","+cursorColor);
    manager->setReportVariable("backgroundColor","Background: "+QString(int(commomParams.backGroundColor)==0?"31.5":"315")+" ASB");
    QString strategy;switch(int(commomParams.strategy)){case 0:strategy="fullThreshold";break;case 1:strategy="smartInteractive";break;case 2:strategy="fastInteractive";break;case 3:strategy="oneStage";break;case 4:strategy="twoStages";break;case 5:strategy="quantifyDefects";break;case 6:strategy="singleStimulation";break;}
    manager->setReportVariable("Strategy","Strategy: "+strategy);
    manager->setReportVariable("VFI","VFI: "+QString::number(qRound(VFI*100))+"%");
    QString GHTstr[]={"Out of limits","Low sensitivity","Border of limits","Within normal limits"};
    manager->setReportVariable("GHT","GHT: "+GHTstr[GHT]);
    manager->setReportVariable("MD",QString("MD: ")+QString::number(md,'f',2)+(p_md-5<=FLT_EPSILON?" (P<"+QString::number(p_md)+"%)":""));
    manager->setReportVariable("PSD",QString("PSD: ")+QString::number(psd,'f',2)+(p_psd-5<=FLT_EPSILON?" (P<"+QString::number(p_psd)+"%)":""));

    manager->setReportVariable("DBImagePath",m_reportFolder+"singleProgress_dBDiagram.bmp");
    manager->setReportVariable("GrayImagePath",m_reportFolder+"singleProgress_gray.bmp");
    manager->setReportVariable("TotalDeviationImagePath",m_reportFolder+"singleProgress_TotalDeviation.bmp");
    manager->setReportVariable("PatternDeviationImagePath",m_reportFolder+"singleProgress_PatternDeviation.bmp");
    manager->setReportVariable("TotalPEImagePath",m_reportFolder+"singleProgress_TotalPE.bmp");
    manager->setReportVariable("PatternPEImagePath",m_reportFolder+"singleProgress_PatternPE.bmp");
    manager->setReportVariable("singleProgressPath",m_reportFolder+"singleProgress.bmp");
    manager->setReportVariable("FixationDeviationImagePath",m_reportFolder+"FixationDeviation.bmp");
    QString progressStr[]={"no progress","possible progress","very possible progress"};
    manager->setReportVariable("progress",QString("progress: ")+progressStr[progress.last()]);

    manager->setReportVariable("baseLine_Dates","   "+m_currentDataList[0].dateTime.date().toString("yyyy/MM/dd")+"   "+m_currentDataList[1].dateTime.date().toString("yyyy/MM/dd"));
    manager->setReportVariable("previous_Dates","   "+m_currentDataList[index-1].dateTime.date().toString("yyyy/MM/dd")+"   "+m_currentDataList[index-2].dateTime.date().toString("yyyy/MM/dd"));

    reportEngine->setShowProgressDialog(true);
    reportEngine->setPreviewScaleType(LimeReport::ScaleType::Percents,50);
    reportEngine->previewReport(/*LimeReport::PreviewHint::ShowAllPreviewBars*/);
}


void ProgressAnalysisListVm::removeCheckResult(int index)
{
    beginResetModel();
    m_currentDataList.removeAt(index);
//    if(m_selectedIndex>=index)
//    {
//        setSelectedIndex(--m_selectedIndex);
//    }
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
    query.where("patient_id").isEqualTo(int(m_patient.m_id)).and_("program_id").in({1,4}).and_("OS_OD").isEqualTo(m_OS_OD).orderAsc("time");
    qx::dao::fetch_by_query(query,checkResult_List);
//    m_mDevList.resize(checkResult_List.size());



    for(int i=0;i<checkResult_List.size();i++)
    {
        m_originalDataList.push_back(getProgressData(checkResult_List[i]));
    }

    m_currentDataList=m_originalDataList;
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
    QVector<int> values(checkResult.m_data.checkdata.size());

    for(int i=0;i<int(program.m_data.dots.size());i++)
    {
        locs[i]={program.m_data.dots[i].x,program.m_data.dots[i].y};
    }
    for(int i=0;i<int(checkResult.m_data.checkdata.size());i++)
    {
        values[i]=checkResult.m_data.checkdata[i];
    }


    auto time=checkResult.m_time;
    QString programName=program.m_name;
    auto strategy=checkResult.m_params.commonParams.strategy;
    QString strategyName=QVector<QString>{"全阈值","智能交互式","快速智能交互式","未知"}[int(strategy)<3?int(strategy):3];
    auto& resultData=checkResult.m_data;
    float fixationLossRate=resultData.fixationLostTestCount!=0?float(resultData.fixationLostCount)/resultData.fixationLostTestCount:0;
    float falseNegativeRate=resultData.falseNegativeTestCount!=0?float(resultData.falseNegativeCount)/resultData.falseNegativeTestCount:0;
    float falsePositiveRate=resultData.falsePositiveTestCount!=0?float(resultData.falsePositiveCount)/resultData.falsePositiveTestCount:0;

    return Data{checkResult.m_id,time,programName,strategyName,centerDotCheck,md,p_md,psd,p_psd,GHT,resultData.fixationLostCount,resultData.fixationLostTestCount,fixationLossRate,
                falseNegativeRate,falsePositiveRate,values,dev,mDev,peDev,peMDev,locs};
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
    case (Roles::fixationLostCount): return data.fixationLostCount;
    case (Roles::fixationLostTestCount): return data.fixationLostTestCount;
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
    roles[fixationLostCount]="fixationLostCount";
    roles[fixationLostTestCount]="fixationLostTestCount";
    roles[falseNegativeRate]="falseNegativeRate";
    roles[falsePositiveRate]="falsePositiveRate";
    return roles;
}
}

#include "progressAnalysislistVm.moc"
