#include "analysis_svc.h"
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QLine>
#include <QtMath>
//#include <QVector>
#include <QPair>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include "perimeter/main/viewModel/staticAnalysisResult.h"
#include "perimeter/main/appctrl/perimeter_appctrl.hxx"
#include <QFileDialog>
#include <QApplication>
#include <QtMath>
#include "perimeter/third-part/qxpack/indcom/ui_qml_base/qxpack_ic_ui_qml_api.hxx"
#include <perimeter/main/model/patientmodel.h>

namespace Perimeter {


AnalysisSvc::AnalysisSvc(QObject *parent) : QObject(parent)
{
    QFile jsonFile("./data.json");
    if( !jsonFile.open(QIODevice::ReadOnly)) {qDebug() << "read file error!";return;}
    QJsonParseError jsonParserError;
    auto JsonDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
    m_jsonArray=JsonDoc.array();
    jsonFile.close();


    auto jsonArrToVectorPoint=[&](QVector<QPoint>& vec,const QString& name,QJsonObject& jo)->void
    {
        if(jo["name"]==name)
        {
            auto arr=jo["data"].toArray();
            for(int i=0;i<arr.count();i++)
            {
                auto arr2=arr[i].toArray();
                QPoint point(arr2[0].toInt(),arr2[1].toInt());
                vec.push_back(point);
            }
        }
    };
    auto jsonArrToVectorInt=[&](QVector<int>& vec,const QString& name,QJsonObject& jo)->void
    {
        if(jo["name"]==name)
        {
            auto arr=jo["data"].toArray();
            for(int i=0;i<arr.count();i++){vec.push_back(arr[i].toInt());}
        }
    };


    for(auto i:m_jsonArray)
    {
        auto jo=i.toObject();

        jsonArrToVectorPoint(m_pointLoc_30d,"XY_NORMAL_VALUE_30d",jo);
        jsonArrToVectorPoint(m_pointLoc_60d,"XY_NORMAL_VALUE_60d",jo);

        QVector<QVector<QString>> jsonObjNames={
            {"NORMAL_VALUE36_45_B1_White","NORMAL_VALUE36_45_B1_Red","NORMAL_VALUE36_45_B1_Blue"},
            {"NORMAL_VALUE36_45_B2_White","NORMAL_VALUE36_45_B2_Red","NORMAL_VALUE36_45_B2_Blue"},
            {"NORMAL_VALUE15_35","NORMAL_VALUE36_45","NORMAL_VALUE46_55","NORMAL_VALUE56_65","NORMAL_VALUE66_75"},
            {"NORMAL_VALUE36_45_B4_White","NORMAL_VALUE36_45_B4_Red","NORMAL_VALUE36_45_B4_Blue"},
            {"NORMAL_VALUE36_45_B5_White","NORMAL_VALUE36_45_B5_Red","NORMAL_VALUE36_45_B5_Blue"}
        };

        m_value_30d_cursorSize_ageCorrectionOrCursorColor.resize(jsonObjNames.length());
        for(int i=0;i<jsonObjNames.length();i++)
        {
            m_value_30d_cursorSize_ageCorrectionOrCursorColor[i].resize(jsonObjNames[i].length());
            for(int j=0;j<jsonObjNames[i].length();j++)
            {
                 jsonArrToVectorInt(m_value_30d_cursorSize_ageCorrectionOrCursorColor[i][j],jsonObjNames[i][j],jo);
            }
        }
        jsonArrToVectorInt(m_value_60d,"NORMAL_VALUE15_35_60d",jo);

        jsonArrToVectorPoint(m_GHT1_RIGHT,"GHT1_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT2_RIGHT,"GHT2_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT3_RIGHT,"GHT3_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT4_RIGHT,"GHT4_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT5_RIGHT,"GHT5_RIGHT",jo);


        jsonArrToVectorInt(m_pe_v5[0],"PE_VALUE5",jo);
        jsonArrToVectorInt(m_pe_v2[0],"PE_VALUE2",jo);
        jsonArrToVectorInt(m_pe_v1[0],"PE_VALUE1",jo);
        jsonArrToVectorInt(m_pe_v05[0],"PE_VALUE05",jo);

        jsonArrToVectorInt(m_pe_v5[1],"PE_VALUE5_Blue_Yellow_B5",jo);
        jsonArrToVectorInt(m_pe_v2[1],"PE_VALUE2_Blue_Yellow_B5",jo);
        jsonArrToVectorInt(m_pe_v1[1],"PE_VALUE1_Blue_Yellow_B5",jo);
        jsonArrToVectorInt(m_pe_v05[1],"PE_VALUE05_Blue_Yellow_B5",jo);
    }
}

AnalysisSvc::~AnalysisSvc()
{

}


QObject* AnalysisSvc::runProcess(int report,PatientVm *patient, CheckResultVm *checkResult, QObject *program,QVariant diagramWidth)
{
    m_programType=program->property("type").toInt();
    m_report=report;
    m_patient=patient;
    m_checkResult=checkResult;
    m_program=program;
    m_previewDiagramWidth=int(diagramWidth.toFloat());
    if(m_programType!=2)
    {
        StaticProgramVM* staticProgram=static_cast<StaticProgramVM*>(program);
        auto dotList=staticProgram->getDots();
        m_dotList.resize(dotList.length());
        for(int i=0;i<dotList.length();i++){m_dotList[i]=dotList[i].toPointF();}
        auto range=staticProgram->getParams()->getCommonParams()->getRange();
        m_innerRange=range[0];
        m_range=range[1];
    }
    else
    {
        DynamicProgramVM* dynamicProgram=static_cast<DynamicProgramVM*>(program);
        auto dotList=checkResult->getResultData()->getCheckData();
        m_dotList.resize(dotList.length());
        for(int i=0;i<dotList.length();i++){m_dotList[i]=dotList[i].toPointF();}
        auto range=dynamicProgram->getParams()->getRange();
        m_innerRange=range[0];
        m_range=range[1];
    }
    m_os_od=checkResult->getOS_OD();
    m_patientAge = m_patient->getAge();
    auto values=checkResult->getResultData()->getCheckData();
    m_staticValues.resize(values.length());
    for(int i=0;i<values.length();i++){m_staticValues[i]=values[i].toInt();}
    if(m_programType==0)
    {
        staticAnalysis();
        DrawDiagram();
        return new ThresholdAnalysisResult(m_md,m_p_md,m_psd,m_p_psd,m_VFI,m_GHT);
    }
    else if(m_programType==1)
    {
        m_dotSeen=0;
        m_dotWeakSeen=0;
        m_dotUnseen=0;
        for(auto& i:m_staticValues)
        {
            if(i==0) m_dotUnseen++;
            else if(i==1) m_dotWeakSeen++;
            else if(i==2) m_dotSeen++;
        }
        DrawDiagram();
        return new ScreeningAnalysisResult(m_dotSeen,m_dotWeakSeen,m_dotUnseen);
    }
    else
    {
        auto values=checkResult->getResultData()->getCheckData();
        m_dynamicValues.resize(values.length());
        for(int i=0;i<values.length();i++){m_dynamicValues[i]=values[i].toPointF();}
//        dynamicAnalysis();
        DrawDiagram();
        return nullptr;
    }

}


QPointF AnalysisSvc::getClickDot(float MouseX, float MouseY,float width,float height)
{
    qDebug()<<QString("x:%1,y:%2,w:%3,h:%4").arg(QString::number(MouseX)).arg(QString::number(MouseY)).arg(QString::number(width)).arg(QString::number(height));

    float x_degree=(MouseX/width-0.5)*m_range*2;
    float y_degree=-(MouseY/height-0.5)*m_range*2;

    qDebug()<<QString("xdegre:%1,y:dgeree:%2").arg(QString::number(x_degree)).arg(QString::number(y_degree));

    QPointF nearestDot;
    float nearest_dist_squared=10000;
    for(int i=0;i<m_dotList.length();i++)
    {
        auto dot=m_dotList[i];
        float dist_squared=pow(dot.x()-x_degree,2)+pow(dot.y()-y_degree,2);
        if(dist_squared<nearest_dist_squared)
        {
            nearest_dist_squared=dist_squared;
            nearestDot=dot;
            m_selectedDotIndex=i;
        }
    }
    return nearestDot;
}


QPointF AnalysisSvc::getPixFromPoint(QPointF point, float width, float height)
{

    qDebug()<<QString("x:nearestDot:%1,y:nearestDot:%2").arg(QString::number(point.x())).arg(QString::number(point.y()));

    float pixPerDegW=float(width/2)/m_range;
    float pixPerDegH=float(height/2)/m_range;
    return QPointF(width/2+(point.x()+0.0)*pixPerDegW,height/2-(point.y()-0.0)*pixPerDegH);
}

void AnalysisSvc::showReport(int report)
{
    if(m_report!=report)
    {
        staticAnalysis();
        m_report=report;
    }
    DrawReportDiagram();
    if(m_reportEngine==nullptr){m_reportEngine = new LimeReport::ReportEngine();}

    QString filePath;

    switch (report)
    {
    case 0:filePath="./reports/Single.lrxml";break;
    case 1:filePath="./reports/ThreeInOne.lrxml";break;
    case 2:filePath="./reports/OverView.lrxml";break;
    case 3:filePath="./reports/Screening.lrxml";break;
    default:filePath="./reports/Dynamic.lrxml";break;
    }

    m_reportEngine->loadFromFile(filePath);
    auto manager=m_reportEngine->dataManager();
    manager->clearUserVariables();
    manager->setReportVariable("ProgramName",m_program->property("name").toString());
    manager->setReportVariable("OS_OD",m_checkResult->getOS_OD()==0?"OS":"OD");
    manager->setReportVariable("hospitalName",QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("hospitalName").toString());
    manager->setReportVariable("name",m_patient->getName());
    manager->setReportVariable("birthDate",m_patient->getBirthDate().replace('-','/'));
    manager->setReportVariable("checkDate", m_checkResult->getTime().date().toString("yyyy/MM/dd"));
    manager->setReportVariable("ID", m_patient->getPatientID());
    manager->setReportVariable("age", m_patientAge);
    manager->setReportVariable("checkTime", m_checkResult->getTime().time().toString("H:mm:ss"));
    manager->setReportVariable("sex", m_patient->getSex()==0?"male":"female");


    if(report<=3)
    {
        auto commomParams=static_cast<StaticParamsVM*>(m_checkResult->getParams())->getCommonParams();
        QString fixationMonitor;switch (commomParams->getFixationMonitor()) {case 0:fixationMonitor="No Alarm"; break;case 1:fixationMonitor="Only Alarm";break;case 2:fixationMonitor="Pause And Alarm";break;}
        QString fixationTarget;switch (commomParams->getFixationTarget()){case 0:fixationTarget="Center Point";break;case 1:fixationTarget="Small Diamond";break;case 2:fixationTarget="Big Diamond";break;case 3:fixationTarget="Bottom Point";break;}
        manager->setReportVariable("fixationMonitor","Fixation Monitor: "+fixationMonitor);
        manager->setReportVariable("fixationTarget","Fixation Target: "+fixationTarget);
        auto resultData=m_checkResult->getResultData();
        manager->setReportVariable("fixationLosses","Fixation Losses: "+QString::number(resultData->getFixationLostCount())+"/"+QString::number(resultData->getFixationLostTestCount()));
        manager->setReportVariable("falsePositiveRate","False POS Errors: "+QString::number(qRound(float(resultData->getFalsePositiveCount())/resultData->getFalsePositiveTestCount()*100))+"%");
        manager->setReportVariable("falseNegativeRate","False NEG Errors: "+QString::number(qRound(float(resultData->getFalseNegativeCount())/resultData->getFalseNegativeTestCount()*100))+"%");
        QTime time;time.setHMS(0,0,0);time=time.addSecs(resultData->getTestTimespan());
        manager->setReportVariable("testTime","Test Duration: "+time.toString("mm:ss"));
        manager->setReportVariable("centerDotCheck",QString("Fovea: ")+(commomParams->getCenterDotCheck()?"ON":"OFF"));
        QString cursorSize;switch(commomParams->getCursorSize()){case 0:cursorSize="I";break;case 1:cursorSize="II";break;case 2:cursorSize="III";break;case 3:cursorSize="IV";break;case 4:cursorSize="V";break;}
        QString cursorColor;switch(commomParams->getCursorColor()){case 0:cursorColor="White";break;case 1:cursorColor="Red";break;case 2:cursorColor="Blue";break;}
        manager->setReportVariable("stimCursor","Stimulus: "+cursorSize+","+cursorColor);
        manager->setReportVariable("backgroundColor","Background: "+QString(commomParams->getBackGroundColor()==0?"31.5":"315")+" ASB");
        QString strategy;switch(commomParams->getStrategy()){case 0:strategy="fullThreshold";break;case 1:strategy="smartInteractive";break;case 2:strategy="fastInteractive";break;case 3:strategy="oneStage";break;case 4:strategy="twoStages";break;case 5:strategy="quantifyDefects";break;case 6:strategy="singleStimulation";break;}
        qDebug()<<strategy;
        manager->setReportVariable("Strategy","Strategy: "+strategy);

        manager->setReportVariable("VFI","VFI: "+QString::number(qRound(m_VFI*100))+"%");
        QString GHT;switch (m_GHT){case 0:GHT="Out of limits";break;case 1:GHT="Low sensitivity";break;case 2:GHT="Border of limits";break;case 3:GHT="Within normal limits";break;}
        manager->setReportVariable("GHT","GHT: "+GHT);
        manager->setReportVariable("MD",QString("MD: ")+QString::number(m_md,'f',2)+" (P<"+QString::number(m_p_md)+"%)");
        manager->setReportVariable("PSD",QString("PSD: ")+QString::number(m_psd,'f',2)+" (P<"+QString::number(m_p_psd)+"%)");
    }
    else
    {
        auto resultData=m_checkResult->getResultData();
        QTime time;time.setHMS(0,0,0);time=time.addSecs(resultData->getTestTimespan());
        auto params=static_cast<DynamicParamsVM*>(m_checkResult->getParams());
        QString cursorSize;switch(params->getCursorSize()){case 0:cursorSize="I";break;case 1:cursorSize="II";break;case 2:cursorSize="III";break;case 3:cursorSize="IV";break;case 4:cursorSize="V";break;}
        QString cursorColor;switch(params->getCursorColor()){case 0:cursorColor="White";break;case 1:cursorColor="Red";break;case 2:cursorColor="Blue";break;}
        QString cursorSpeed=QString::number(params->getSpeed());
        QString cursorBrightness=QString::number(params->getBrightness());

        manager->setReportVariable("stimCursor","Stimulus: "+cursorSize+","+cursorColor+","+cursorSpeed+","+cursorBrightness);
        manager->setReportVariable("testTime","Test Duration: "+time.toString("mm:ss"));
    }
    manager->setReportVariable("pupilDiameter","Pupil Diameter: ");
    manager->setReportVariable("visualAcuity","Visual Acuity: ");
    auto rx=m_patient->getRx();
    if(m_os_od)
    {
        manager->setReportVariable("Rx_Ry","Rx:"+QString::number(rx->property("rx1_l").toDouble(),'f',2)+"DS "
                                   +QString::number(rx->property("rx2_l").toDouble(),'f',2)+"DC "
                                   +QString::number(rx->property("rx3_l").toDouble(),'f',2)+"X");
        manager->setReportVariable("visualAcuity","visualAcuity:"+QString::number(rx->property("visual_l").toDouble(),'f',2));

    }
    else
    {
        manager->setReportVariable("Rx_Ry","Rx:"+QString::number(rx->property("rx1_r").toDouble(),'f',2)+"DS "
                                   +QString::number(rx->property("rx2_r").toDouble(),'f',2)+"DC "
                                   +QString::number(rx->property("rx3_r").toDouble(),'f',2)+"X");
        manager->setReportVariable("visualAcuity","visualAcuity:"+QString::number(rx->property("visual_r").toDouble(),'f',2));
    }
    manager->setReportVariable("pupilDiameter","pupilDiameter:"+QString::number(m_checkResult->getResultData()->getPupilDiameter(),'f',2));



    switch (report)
    {
    case 0:
        manager->setReportVariable("DBImagePath","./reportImage/dBDiagram.bmp");
        manager->setReportVariable("GrayImagePath","./reportImage/gray.bmp");
        manager->setReportVariable("TotalDeviationImagePath","./reportImage/TotalDeviation.bmp");
        manager->setReportVariable("PatternDeviationImagePath","./reportImage/PatterDeviation.bmp");
        manager->setReportVariable("TotalPEImagePath","./reportImage/TotalPE.bmp");
        manager->setReportVariable("PatternPEImagePath","./reportImage/PatternPE.bmp");
        break;
    case 1:
        manager->setReportVariable("GrayImagePath","./reportImage/gray.bmp");
        manager->setReportVariable("DefectDepthImagePath","./reportImage/defectDepth.bmp");
        manager->setReportVariable("DBImagePath","./reportImage/dBDiagram.bmp");

        break;
    case 2:
        manager->setReportVariable("DBImagePath","./reportImage/dBDiagram.bmp");
        manager->setReportVariable("GrayImagePath","./reportImage/gray.bmp");
        manager->setReportVariable("TotalPEImagePath","./reportImage/TotalPE.bmp");
        manager->setReportVariable("PatternPEImagePath","./reportImage/PatternPE.bmp");
        break;
    case 3:
        manager->setReportVariable("ScreeningImagePath","./reportImage/Screening.bmp");
        manager->setReportVariable("Seen",QString("Seen:")+QString::number(m_dotSeen)+"/"+QString::number(m_dotList.length()));
        manager->setReportVariable("WeakSeen",QString("Weak Seen:")+QString::number(m_dotWeakSeen)+"/"+QString::number(m_dotList.length()));
        manager->setReportVariable("Unseen",QString("Unseen:")+QString::number(m_dotUnseen)+"/"+QString::number(m_dotList.length()));
        break;
    default:
        manager->setReportVariable("DynamicImagePath","./reportImage/Dynamic.bmp");
    }
    manager->setReportVariable("FixationDeviationImagePath","./reportImage/FixationDeviation.bmp");
    manager->setReportVariable("DoctorSign","DoctorSign: ");
    manager->setReportVariable("DiagnosisContent", m_checkResult->getComment());
    manager->setReportVariable("deviceInfo","Device Info: "+QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("deviceInfo").toString());
    manager->setReportVariable("version", "Version: "+QxPack::IcUiQmlApi::appCtrl()->property("settings").value<QObject*>()->property("version").toString());


//     report->previewReport();
//     delete report;
     m_reportEngine->setShowProgressDialog(true);
     m_reportEngine->setPreviewScaleType(LimeReport::ScaleType::Percents,50);
     m_reportEngine->previewReport(/*LimeReport::PreviewHint::ShowAllPreviewBars*/);
}



QObject *AnalysisSvc::previewReport(int report, int resultId)
{
    CheckResult_ptr checkResult_ptr;
    checkResult_ptr->m_id=resultId;
    qx::dao::fetch_by_id(checkResult_ptr);
    Program_ptr program_ptr;
    program_ptr->m_id=checkResult_ptr->m_program->m_id;
    qx::dao::fetch_by_id(program_ptr);
    Patient_ptr patient_ptr;
    patient_ptr->m_id=checkResult_ptr->m_patient->m_id;
    qx::dao::fetch_by_id(patient_ptr);


    CheckResultModel<Type::ThreshHold> checkResult(checkResult_ptr);
    ProgramModel<Type::ThreshHold> program(program_ptr);
    PatientModel patient(patient_ptr);

    if(checkResult.m_type==Type::ThreshHold)
    {
        auto params=checkResult.m_params;
        int cursorSize=int(params.commonParams.cursorSize);
        int cursorColor=int(params.commonParams.cursorColor);
        int backGroundColor=int(params.commonParams.backGroundColor);

        int value_30d_secondIndex;
        int age_correction;
        int patientAge=patient.m_age;

        if(cursorSize==2)
        {
            if(patientAge<=35){age_correction=1;}
            else if(patientAge<=45){age_correction=2;}
            else if(patientAge<=55){age_correction=3;}
            else if(patientAge<=65){age_correction=4;}
            else {age_correction=5;}
            value_30d_secondIndex=age_correction;
        }
        else
        {
            value_30d_secondIndex=cursorColor;
        }

        QVector<int> value_30d=m_value_30d_cursorSize_ageCorrectionOrCursorColor[cursorSize][value_30d_secondIndex];


        auto getIndex=[&](QPointF&& dot,QVector<QPoint>& pointLoc)->int{
            int distMin=10000;
            int index=-1;

            for(int i=0;i<pointLoc.length();i++)
            {
                int dist;
                if(checkResult.m_OS_OD==0)
                {
                    dist=pow(pointLoc[i].rx()-dot.x(),2)+pow(pointLoc[i].ry()-dot.y(),2);
                }
                else
                {
                    dist=pow(pointLoc[i].rx()-(-dot.x()),2)+pow(pointLoc[i].ry()-dot.y(),2);
                }
                if(dist<distMin){distMin=dist;index=i;}
            }
            return index;
        };

        QVector<int> pe_v5,pe_v2,pe_v1,pe_v05;
        if(cursorColor==2&&backGroundColor==1&&cursorSize==4)
        {  pe_v5=m_pe_v5[1]; pe_v2=m_pe_v2[1]; pe_v1=m_pe_v1[1]; pe_v05=m_pe_v05[1];}
        else
        {  pe_v5=m_pe_v5[0]; pe_v2=m_pe_v2[0]; pe_v1=m_pe_v1[0]; pe_v05=m_pe_v05[0];}




        auto dotList=program.m_data.dots;
        QVector<int> sv(dotList.size(),0);
        QVector<int> dev(dotList.size(),0);
        QVector<int> mDev(dotList.size(),0);
        QVector<int> peDev(dotList.size(),0);
        QVector<int> peMDev(dotList.size(),0);


        QVector<int> vfiRingStandard(5,0);
        QVector<int> vfiRingTest(5,0);

        float VFI_Weight[5]={3.29f,1.28f,0.79f,0.57f,0.43f};

        for(int i=0;i<int(dotList.size());i++)
        {
            auto dot=dotList[i];
            float radius=sqrt(pow(dot.x,2)+pow(dot.y,2));
            int index;
            if(radius<=30)
            {
                index=getIndex(QPointF{dot.x,dot.y},m_pointLoc_30d);
                sv[i]=value_30d[index]/10;
                if(!(cursorSize==2&&cursorColor==0)){
                    if(cursorColor==2)
                    {
                        if(sv[i]>0){sv[i]-=2*(age_correction-1);} else if(sv[i]<0){sv[i]+=2*(age_correction-1);}
                    }
                    else
                    {
                        if(sv[i]>0){sv[i]-=(age_correction-1);} else if(sv[i]<0){sv[i]+=(age_correction-1);}
                    }
                }

            }
            else if(radius<=60)
            {
                index=getIndex(QPointF{dot.x,dot.y},m_pointLoc_60d);
                sv[i]=m_value_60d[index]/10;
                if(sv[i]>0) sv[i]-=age_correction;else if(sv[i]<0) sv[i]+=age_correction;
            }

            if(sv[i]>0)  {dev[i]=checkResult.m_data.checkdata[i]-sv[i];}                                                   //dev 盲点
            else{ dev[i]=-99; }


            if(radius<=30)
            {
                if(dev[i]!=-99)
                {
                    int v=-dev[i];
                    if (v<=pe_v5[index]) peDev[i]=0;
                    else if(v<=pe_v2[index]) peDev[i]=1;
                    else if(v<=pe_v1[index]) peDev[i]=2;
                    else if(v<=pe_v05[index]) peDev[i]=3;
                    else peDev[i]=4;
                }
                else{
                    peDev[i]=-99;
                }
            }

            if(sv[i]>0)
            {
                int ringIndex=round(radius/6+0.4)-1;
                if(ringIndex>4) ringIndex=4;

                vfiRingStandard[ringIndex]+=sv[i];
                vfiRingTest[ringIndex]+=checkResult.m_data.checkdata[i];
            }
        }



        float vfih=0,vfid=0;
        for(int i=0;i<5;i++)
        {
            if(vfiRingStandard[i]!=0)
            {
                float temp=float(vfiRingTest[i])/(vfiRingStandard[i]+0.0001);
                if(temp>1){temp=1;}
                vfih+=VFI_Weight[i]*temp;
                vfid+=VFI_Weight[i];
            }
        }
    //    qDebug()<<vfih;
    //    qDebug()<<vfid;

        float VFI=vfih/vfid;                                     //vfi
        float md=0;float psd=0;
    //    qDebug()<<m_VFI;

        for(auto& i: dev)
        {
            if(i!=-99) md+=i;
        }
        md/=(dev.length()-2);    //扣除盲点

    //    qDebug()<<m_md;

        for(auto& i: dev)
        {
            if(i!=-99) psd+=pow(i-md,2);
        }
        psd=sqrt(psd/(dev.length()-2));

        float p_md,p_psd;

        if(psd>4.3) p_psd=0.5;
        else if(psd>3.7) p_psd=1;
        else if(psd>3.2) p_psd=2;
        else if(psd>2.5) p_psd=5;
        else if(psd>2.0) p_psd=10;

        if(md<-5.5)  p_md=0.5;
        else if(md<-3.5) p_md=1;
        else if(md<-2.6) p_md=2;
        else if(md<-2.0) p_md=5;
        else if(md<-1.5) p_md=10;

    //    qDebug()<<m_psd;

        int md_correction=round(md+psd);
    //    qDebug()<<md;

        for(int i=0;i<dev.length();i++)
        {
            if(dev[i]==-99)
                mDev[i]=-99;
            else
                mDev[i]=dev[i]-md_correction;
        }


        for(int i=0;i<mDev.length();i++)
        {

            auto index=getIndex(QPointF(dotList[i].x,dotList[i].y),m_pointLoc_30d);
            float radius=sqrt(pow(dotList[i].x,2)+pow(dotList[i].y,2));
            if(radius<30)
            {
                if(mDev[i]==-99)
                    peMDev[i]=-99;
                else
                {
                    int v=-mDev[i];
                    if (v<=pe_v5[index]) peMDev[i]=0;
                    else if(v<=pe_v2[index]) peMDev[i]=1;
                    else if(v<=pe_v1[index]) peMDev[i]=2;
                    else if(v<=pe_v05[index]) peMDev[i]=3;
                    else peMDev[i]=4;
                }
            }
        }




        auto isGHT=[](QVector<QPoint> GHTArr,QPoint point)->bool
        {
            for(auto&i:GHTArr)
            {
                if(i.x()==point.x()&&i.y()==point.y()) return true;
            }
            return false;
        };


        float ght[5][2];
        bool b27=false;
        memset(ght,0,sizeof(ght));
        for(int i=0;i<int(dotList.size());i++)
        {
            int v;QPoint dot;
            if(mDev[i]!=-99)
                v=-mDev[i];

            if(checkResult.m_OS_OD==0)
                dot={int(dotList[i].x),int(dotList[i].y)};
            else
               dot={int(-dotList[i].x),int(dotList[i].y)};

            if(isGHT(m_GHT1_RIGHT,dot)){ght[0][0]+=v;}
            if(isGHT(m_GHT2_RIGHT,dot)){if(v>=7&&abs(dot.x())==abs(dot.y())) b27=true;ght[1][0]+=v;}
            if(isGHT(m_GHT3_RIGHT,dot)){ght[2][0]+=v;}
            if(isGHT(m_GHT4_RIGHT,dot)){ght[3][0]+=v;}
            if(isGHT(m_GHT5_RIGHT,dot)){ght[4][0]+=v;}
            if(isGHT(m_GHT1_RIGHT,{dot.x(),-dot.y()})){ght[0][1]+=v;}
            if(isGHT(m_GHT2_RIGHT,{dot.x(),-dot.y()})){if(v>=7&&abs(dot.x())==abs(dot.y())) b27=true;ght[1][1]+=v;}
            if(isGHT(m_GHT3_RIGHT,{dot.x(),-dot.y()})){ght[2][1]+=v;}
            if(isGHT(m_GHT4_RIGHT,{dot.x(),-dot.y()})){ght[3][1]+=v;}
            if(isGHT(m_GHT5_RIGHT,{dot.x(),-dot.y()})){ght[4][1]+=v;}
        }

        ght[0][0] *= 1 / 3;
        ght[1][0] *= 1 / 4;
        ght[2][0] *= 1 / 5;
        ght[3][0] *= 1 / 6;
        ght[4][0] *= 1 / 4;

        ght[0][1] *= 1 / 3;
        ght[1][1] *= 1 / 4;
        ght[2][1] *= 1 / 5;
        ght[3][1] *= 1 / 6;
        ght[4][1] *= 1 / 4;

        int GHT=3;
        if (ght[0][0] >= 5 || ght[0][1] >= 5 || ght[1][0] >= 5 || ght[1][1] >= 5 ||
            ght[2][0] >= 6 || ght[2][1] >= 6 || ght[3][0] >= 6 || ght[3][1] >= 6 || ght[4][0] >= 7 || ght[4][1] >= 7)
        {
            GHT = 0;
        }
        else if(b27)
        {
            GHT = 1;
        }
        else if(ght[0][0] > 3 || ght[0][1] > 3 || ght[2][0] >= 4 || ght[2][1] >= 4 || ght[3][0] >= 4 || ght[3][1] >= 4)
        {
            GHT = 2;
        }
        return new ThresholdAnalysisResult(md,p_md,psd,p_psd,VFI,GHT);
    }
    if(checkResult.m_type==Type::Screening)
    {
        int dotSeen=0,dotWeakSeen=0,dotUnseen=0;
        auto& checkData=checkResult.m_data.checkdata;
        for(int i=0;i<int(checkData.size());i++)
        {
            for(auto& i:checkData)
            {
                if(i==0) dotUnseen++;
                else if(i==1) dotWeakSeen++;
                else if(i==2) dotSeen++;
            }
        }
        return new ScreeningAnalysisResult(dotSeen,dotWeakSeen,dotUnseen);
    }
}

QObject *AnalysisSvc::showReport(int report, int resultId)
{
    return nullptr;
}

QObject *AnalysisSvc::ThresholdAnalysis(QVector<int> &dev, QVector<int> &mDev, QVector<int> &peDev, QVector<int> &peMDev)
{
    return nullptr;
}



void AnalysisSvc::drawPixScale()
{
    int scale=m_isPreview?1:2;
    QPainter painter(&m_image);
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    painter.drawLine(QLine(0,m_image.height()/2,m_image.width(),m_image.height()/2));
    painter.drawLine(QLine(m_image.width()/2,0,m_image.width()/2,m_image.height()));

    int segmentCount=m_range/10;
    for(int i=0;i<=segmentCount*2;i++)
    {
        if(i!=segmentCount)
        {
            painter.drawLine(QLine((float(m_image.width()-scale))/(segmentCount*2)*i+scale/2,m_image.height()/2-2*scale,(float(m_image.width()-scale))/(segmentCount*2)*i+scale/2,m_image.height()/2+2*scale));
            painter.drawLine(QLine(m_image.width()/2-2*scale,(float(m_image.height()-scale))/(segmentCount*2)*i+scale/2,m_image.width()/2+2*scale,(float(m_image.height()-scale))/(segmentCount*2)*i+scale/2));
        }
    }
}

void AnalysisSvc::drawRoundCrossPixScale()
{
    int scale=m_isPreview?1:2;
    QPainter painter(&m_image);

    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    painter.drawLine(QLine(0,m_image.height()/2,m_image.width(),m_image.height()/2));
    painter.drawLine(QLine(m_image.width()/2,0,m_image.width()/2,m_image.height()));

//    auto drawDashLine=[]()->void{};
    int segmentCount=m_range/10;
    QPen pen;
    pen.setDashPattern({float(3*scale),float(5*scale)});
    pen.setWidth(float(scale));
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);
    for(int i=1;i<=segmentCount;i++)
    {
        int radius=float(m_image.width())/2/segmentCount*i-scale/2;
        painter.drawEllipse({m_image.height()/2,m_image.width()/2},radius,radius);
    }

    qreal radius=m_image.width()/2;
    for(int i=1;i<12;i++)
    {
        if(i%3!=0)
        {
            auto angle=M_PI/6*i;
            painter.drawLine(QLine{m_image.width()/2,m_image.height()/2,int(m_image.width()/2+radius*qSin(angle)),int(m_image.height()/2+radius*qCos(angle))});
        }
    }
}



void AnalysisSvc::drawDevDBText(QVector<int> values)
{
    QPainter painter(&m_image);
    int fontPixSize=m_image.width()/18;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);
    for(int i=0;i<m_dotList.length()&&i<values.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(m_dotList[i]);
        if(values[i]==-99) continue;

         const QRect rectangle = QRect(pixLoc.x()-fontPixSize*1.6*0.4, pixLoc.y()-fontPixSize*0.8/2, fontPixSize*1.6,fontPixSize*0.8);
         painter.drawText(rectangle,Qt::AlignCenter,QString::number(values[i]));
        m_image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000);   //标个小红点
    }
}


void AnalysisSvc::drawPE(QVector<int> values)
{
    QPainter painter(&m_image);
    float scale;
    if(m_isPreview)
    {
        scale=qCeil(float(m_image.width())/240*1.5);
    }
    else scale=4;
    for(int i=0;i<m_dotList.length()&&i<m_staticValues.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(m_dotList[i]);
        QString path=QString(":/grays/PE")+QString::number(values[i])+".bmp";
        QImage image(path);
        auto scaledImage=image.scaled(image.width()*scale,image.height()*scale);
        QPoint tempPixLoc={pixLoc.x()-scaledImage.width()/2,pixLoc.y()-scaledImage.height()/2};
        painter.drawImage(tempPixLoc,scaledImage);
    }
}

QPoint AnalysisSvc::convertDegLocToPixLoc(QPointF DegLoc)
{
    float pixPerDegW=float(m_image.width()/2)/m_range;
    float pixPerDegH=float(m_image.height()/2)/m_range;
    return QPoint(m_image.width()/2+DegLoc.x()*pixPerDegW,m_image.height()/2-DegLoc.y()*pixPerDegH);
}


void AnalysisSvc::DrawDiagram()
{

    m_imageSavePath="./previewImage";
    m_isPreview=true;
    if(m_report==0)                 //single
    {
        QSize imageSize={m_previewDiagramWidth,m_previewDiagramWidth};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawDBDiagram();
        drawGrayDiagram();
        drawTotalDeviation();
        drawPatternDeviation();
        drawTotalPE();
        drawPatternPE();
    }
    if(m_report==1)
    {
        QSize imageSize={m_previewDiagramWidth,m_previewDiagramWidth};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawGrayDiagram();
        drawDefectDepthDiagram();
        drawDBDiagram();
    }

    if(m_report==2)
    {
        QSize imageSize={m_previewDiagramWidth,m_previewDiagramWidth};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawDBDiagram();
        drawGrayDiagram();
        drawTotalPE();
        drawPatternPE();
    }

    if(m_report==3)
    {
        QSize imageSize={m_previewDiagramWidth,m_previewDiagramWidth};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawScreening();
    }
    if(m_report>3)
    {
        QSize imageSize={m_previewDiagramWidth,m_previewDiagramWidth};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawDynamic();
    }

}

void AnalysisSvc::DrawReportDiagram()
{
    m_imageSavePath="./reportImage";
    m_isPreview=false;
    if(m_report==0)                 //single
    {
        QSize imageSize={550,550};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawDBDiagram();
        drawGrayDiagram();
        imageSize={500,500};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawTotalDeviation();
        drawPatternDeviation();
        drawTotalPE();
        drawPatternPE();
        drawFixationDeviation();
    }
    if(m_report==1)
    {
        QSize imageSize={550,550};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawGrayDiagram();
        drawDefectDepthDiagram();
        drawDBDiagram();
        drawFixationDeviation();
    }

    if(m_report==2)
    {
        QSize imageSize={550,550};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawDBDiagram();
        drawGrayDiagram();
        drawTotalPE();
        drawPatternPE();
        drawFixationDeviation();
    }

    if(m_report==3)
    {
        QSize imageSize={1100,1100};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawScreening();
        drawFixationDeviation();
    }
    if(m_report>3)
    {
        QSize imageSize={1100,1100};
        m_image=QImage(imageSize, QImage::Format_RGB32);
        drawDynamic();
        drawFixationDeviation();
    }
}


void AnalysisSvc::drawDBDiagram()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    QPainter painter(&m_image);
    int fontPixSize=m_image.width()/18;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);
    QPoint blindDot;
    if(m_os_od==0) blindDot={-14,0};else blindDot={14,0};
    auto pixLoc=convertDegLocToPixLoc(blindDot);
    QImage blindImage(":/grays/SE3.bmp");
    float scale;
    if(m_isPreview){ scale=1;}
    else scale=3;
    blindImage=blindImage.scaled(blindImage.width()*scale,blindImage.height()*scale);
    painter.drawImage(QPoint{pixLoc.x()-blindImage.width()/2,pixLoc.y()-blindImage.height()/2},blindImage);

    for(int i=0;i<m_dotList.length()&&i<m_staticValues.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(m_dotList[i]);
        const QRect rectangle = QRect(pixLoc.x()-fontPixSize*1.6*0.4, pixLoc.y()-fontPixSize*0.8/2, fontPixSize*1.6,fontPixSize*0.8);
        painter.drawText(rectangle,Qt::AlignCenter,QString::number(m_staticValues[i]));
        m_image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000); //标个小红点
    }
    m_image.save(m_imageSavePath+"/dBDiagram.bmp","bmp");
}

void AnalysisSvc::drawGrayDiagram()
{
     m_image.fill(qRgb(255, 255, 255));
     drawPixScale();
     QPainter painter(&m_image);
     int gap= m_range/15;
     int left_x_axis=0,right_x_axis=0,up_y_axis=0,down_y_axis=0;
     for(auto &i:m_dotList)
     {
         auto x=i.x();auto y=i.y();
         if(x<left_x_axis) left_x_axis=x;
         if(x>right_x_axis) right_x_axis=x;
         if(y>up_y_axis) up_y_axis=y;
         if(y<down_y_axis) down_y_axis=y;
     }



     int rangeX=qMax(qAbs(left_x_axis),right_x_axis)+1.5*gap;;int rangeY=qMax(qAbs(down_y_axis),up_y_axis)+1.5*gap;

     QVector<QPoint> paintLocs;
     for(int i=up_y_axis;i>=down_y_axis;i-=gap)
     {
         for(int j=left_x_axis;j<=right_x_axis;j+=gap)
         {
             float answerOutter=float(qPow(qAbs(i)+2,2))/(rangeX*rangeX)+float(qPow(qAbs(j)+2,2))/(rangeY*rangeY);
             float answerInner=1.0;
             if(m_innerRange!=0){answerInner=(float(qPow(qAbs(i)+2,2))+float(qPow(qAbs(j)+2,2)))/(m_innerRange*m_innerRange);}
             if((answerOutter<=1.0)&&(answerInner>=1.0))
                 paintLocs.append(QPoint(j,i));
         }
     }

     for(int i=0;i<paintLocs.length();i++)
     {
         QVector<QPair<int,int>> distIndexList;
         for(int j=0;j<m_dotList.length();j++)
         {
             auto dot=m_dotList[j].toPoint();
             int dist=qPow(paintLocs[i].x()-dot.x(),2)+qPow(paintLocs[i].y()-dot.y(),2);
             distIndexList.append(QPair<int,int>(j,dist));
         }

         std::sort(distIndexList.begin(),distIndexList.end(),[](QPair<int,int> a,QPair<int,int> b){return a.second<b.second;});
         QVector<QPair<int,int>> interpolationDots;
         for(int j=0;j<4;j++)
         {
             if(j<2)
             {
                 interpolationDots.append(distIndexList[j]);
             }
             else
             {
                 if(distIndexList[j].second<=2*qPow(3*gap,2))
                     interpolationDots.append(distIndexList[j]);
             }
         }

         float totalValue=0,totalDist=0;
         for(int j=0;j<interpolationDots.length();j++)
         {
             int index=interpolationDots[j].first;
             if(interpolationDots[j].second==0)
             {
                 totalValue=m_staticValues[index];
                 totalDist=1;
                 break;
             }
             totalValue+=(float(m_staticValues[index])/qSqrt(interpolationDots[j].second));
             totalDist+=1.0f/qSqrt(interpolationDots[j].second);
         }
         float interpolVal=totalValue/totalDist;
         int grayVal=qCeil(interpolVal/5);
         if(grayVal>9) grayVal=9;



        QPoint tempPoint;
        if(paintLocs[i].x()>0) tempPoint.setX(paintLocs[i].x()+2); else tempPoint.setX(paintLocs[i].x()-2);
        if(paintLocs[i].y()>0) tempPoint.setY(paintLocs[i].y()+2); else tempPoint.setY(paintLocs[i].y()-2);
        auto pixLoc=convertDegLocToPixLoc(tempPoint);
        QString path=QString(":/grays/Gray")+QString::number(grayVal)+".bmp";
        QImage image(path);
        float scale;
        if(m_isPreview){scale=1;}
        else scale=2;
        auto scaledImage=image.scaled(image.width()*scale,image.height()*scale);
        QPoint tempPixLoc={pixLoc.x()-scaledImage.width()/2,pixLoc.y()-scaledImage.height()/2};
        painter.drawImage(tempPixLoc,scaledImage);
     }


     m_image.save(m_imageSavePath+"/gray.bmp","bmp");
}

void AnalysisSvc::drawDefectDepthDiagram()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    QPainter painter(&m_image);
    int fontPixSize=m_image.width()/18;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);

    for(int i=0;i<m_dotList.length()&&i<m_dev.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(m_dotList[i]);
        if(m_dev[i]==-99) continue;
        if(m_dev[i]>-4)
        {
            float scale;
            if(m_isPreview)
            {
                scale=qFloor(float(m_image.width())/240);
                if(scale<1) scale=1;
            }
            else scale=2;
            QImage image(":/grays/DE0.bmp");
            image=image.scaled(image.width()*scale,image.height()*scale);
            painter.drawImage(QPointF{pixLoc.x()-image.width()*0.48,pixLoc.y()-image.height()*0.50},image);
        }
        else
        {
            const QRect rectangle = QRect(pixLoc.x()-fontPixSize*1.6*0.45, pixLoc.y()-fontPixSize*0.8/2, fontPixSize*1.6,fontPixSize*0.8);
            painter.drawText(rectangle,Qt::AlignCenter,QString::number(-m_dev[i]));
            m_image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000);   //标个小红点
        }
    }
    m_image.save(m_imageSavePath+"/defectDepth.bmp","bmp");
}

void AnalysisSvc::drawTotalDeviation()
{

    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    drawDevDBText(m_dev);
    m_image.save(m_imageSavePath+"/TotalDeviation.bmp","bmp");
}


void AnalysisSvc::drawPatternDeviation()
{

    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    drawDevDBText(m_mDev);
    m_image.save(m_imageSavePath+"/PatterDeviation.bmp","bmp");

}

void AnalysisSvc::drawTotalPE()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    drawPE(m_peDev);
    m_image.save(m_imageSavePath+"/TotalPE.bmp","bmp");
}

void AnalysisSvc::drawPatternPE()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    drawPE(m_peMDev);
    m_image.save(m_imageSavePath+"/PatternPE.bmp","bmp");
}

void AnalysisSvc::drawScreening()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    QPainter painter(&m_image);
    QPoint blindDot;
    if(m_os_od==0) blindDot={-14,0};else blindDot={14,0};
    auto pixLoc=convertDegLocToPixLoc(blindDot);
    QImage blindImage(":/grays/SE3.bmp");
    QImage screenImageSE0(":/grays/SE0.bmp");
    QImage screenImageSE1(":/grays/SE1.bmp");
    QImage screenImageSE2(":/grays/SE2.bmp");
    int scale=m_isPreview?1:2;
//    int scale;
//    if(m_isPreview)
//    {
//        scale=qCeil(float(m_image.width())/500);
//    }
//    else scale=3;
    QImage image(":/grays/DE0.bmp");
    image=image.scaled(image.width()*scale,image.height()*scale);
    screenImageSE0=screenImageSE0.scaled(screenImageSE0.width()*scale,screenImageSE0.height()*scale);
    screenImageSE1=screenImageSE1.scaled(screenImageSE1.width()*scale,screenImageSE1.height()*scale);
    screenImageSE2=screenImageSE2.scaled(screenImageSE2.width()*scale,screenImageSE2.height()*scale);
    blindImage=blindImage.scaled(blindImage.width()*scale,blindImage.height()*scale);
    painter.drawImage(QPoint{pixLoc.x()-blindImage.width()/2,pixLoc.y()-blindImage.height()/2},blindImage);
    for(int i=0;i<m_dotList.length()&&i<m_staticValues.length();i++)
    {
        auto pixLoc=convertDegLocToPixLoc(m_dotList[i]);
        auto imageLoc=QPoint{pixLoc.x()-screenImageSE0.width()/2,pixLoc.y()-screenImageSE0.height()/2};
        switch (m_staticValues[i])
        {
        case 0:painter.drawImage(imageLoc,screenImageSE2);break;
        case 1:painter.drawImage(imageLoc,screenImageSE1);break;
        case 2:painter.drawImage(imageLoc,screenImageSE0);break;
        default:break;
        }
    }
    m_image.save(m_imageSavePath+"/Screening.bmp","bmp");
}

void AnalysisSvc::drawDynamic()
{
    m_image.fill(qRgb(255, 255, 255));
    drawRoundCrossPixScale();
    int scale=m_isPreview?1:2;
    QPainter painter(&m_image);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    qDebug()<<m_dynamicValues.length();
    for(int i=0;i<m_dynamicValues.length();i++)
    {
        auto begin=convertDegLocToPixLoc(m_dynamicValues[i]);
        auto end=convertDegLocToPixLoc(m_dynamicValues[(i+1)%m_dynamicValues.length()]);
        painter.drawLine(QLine(begin.x(),begin.y(),end.x(),end.y()));
        painter.drawEllipse(begin,3*scale,3*scale);
    }

    m_image.save(m_imageSavePath+"/Dynamic.bmp","bmp");
}

void AnalysisSvc::drawFixationDeviation()
{

    int scale=2;
//    m_isPreview?scale=1:scale=2;
    auto dots=m_checkResult->getResultData()->getFixationDeviation();
//    dots={5,-5,5,-5,5,-5,13,13,13,-13,-13,-13};
    QImage image(322*scale,27*scale, QImage::Format_RGB32);
    image.fill(qRgb(255, 255, 255));
    QPainter painter(&image);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    painter.drawLine(QLine(scale/2,0,scale/2,27*scale));
    painter.drawLine(QLine(image.width()-scale/2,0,image.width()-scale/2,27*scale));
    painter.drawLine(QLine(0,27*scale/2,image.width()-scale/2,27*scale/2));
    auto convertY=[=](int p)->int{return 13-p;};
    for(int i=0;i<dots.length();i++)
    {
        qDebug()<<dots[i].toInt();
        int dot=convertY(dots[i].toInt());
        painter.drawLine(QLine((i+1.5)*scale,27*scale/2,(i+1.5)*scale,dot*scale+1));
    }
    image.save("./reportImage/FixationDeviation.bmp","bmp");
}



void AnalysisSvc::staticAnalysis()
{
    m_pointLoc_30d.clear();
    m_pointLoc_60d.clear();
    m_value_30d.clear();
    m_value_60d.clear();
    m_age_correction=0;

    m_v5.clear();
    m_v2.clear();
    m_v1.clear();
    m_v05.clear();


    m_sv.fill(0,m_dotList.length());
    m_dev.fill(0,m_dotList.length());
    m_mDev.fill(0,m_dotList.length());
    m_peDev.fill(0,m_dotList.length());
    m_peMDev.fill(0,m_dotList.length());

    m_vfiRingStandard.fill(0,5);
    m_vfiRingTest.fill(0,5);

    m_md=0;
    m_psd=0;


    if(m_patientAge<=35){m_age_correction=0;}
    else if(m_patientAge<=45){m_age_correction=1;}
    else if(m_patientAge<=55){m_age_correction=2;}
    else if(m_patientAge<=65){m_age_correction=3;}
    else {m_age_correction=4;}


    StaticParamsVM* params =static_cast<StaticParamsVM*>(m_checkResult->getParams()) ;
    int cursorSize=params->getCommonParams()->getCursorSize();
    int cursorColor=params->getCommonParams()->getCursorColor();
    int backGroundColor=params->getCommonParams()->getBackGroundColor();

    QString XY_NORMAL_VALUE;

    if(cursorSize==2)  //30 内的程序才能调尺寸,颜色,此种尺寸颜色比较精确
    {
        if(m_patientAge<=35){XY_NORMAL_VALUE="NORMAL_VALUE15_35";}
        else if(m_patientAge<=45){XY_NORMAL_VALUE="NORMAL_VALUE36_45";}
        else if(m_patientAge<=55){XY_NORMAL_VALUE="NORMAL_VALUE46_55";}
        else if(m_patientAge<=65){XY_NORMAL_VALUE="NORMAL_VALUE56_65";}
        else {XY_NORMAL_VALUE="NORMAL_VALUE66_75";}
    }

    else if(cursorSize==0&&cursorColor==0){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B1_White";}
    else if(cursorSize==1&&cursorColor==0){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B2_White";}
    else if(cursorSize==3&&cursorColor==0){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B4_White";}
    else if(cursorSize==4&&cursorColor==0){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B5_White";}

    else if(cursorSize==0&&cursorColor==1){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B1_Red";}
    else if(cursorSize==1&&cursorColor==1){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B2_Red";}
    else if(cursorSize==3&&cursorColor==1){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B4_Red";}
    else if(cursorSize==4&&cursorColor==1){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B5_Red";}

    else if(cursorSize==0&&cursorColor==2){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B1_Blue";}
    else if(cursorSize==1&&cursorColor==2){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B2_Blue";}
    else if(cursorSize==3&&cursorColor==2){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B4_Blue";}
    else if(cursorSize==4&&cursorColor==2){XY_NORMAL_VALUE="NORMAL_VALUE36_45_B5_Blue";}



    auto jsonArrToVectorPoint=[&](QVector<QPoint>& vec,const QString& name,QJsonObject& jo)->void
    {
        if(jo["name"]==name)
        {

            auto arr=jo["data"].toArray();
            for(int i=0;i<arr.count();i++)
            {
                auto arr2=arr[i].toArray();
                QPoint point(arr2[0].toInt(),arr2[1].toInt());
                vec.push_back(point);
            }
        }
    };
    auto jsonArrToVectorInt=[&](QVector<int>& vec,const QString& name,QJsonObject& jo)->void
    {
        if(jo["name"]==name)
        {
            auto arr=jo["data"].toArray();
            for(int i=0;i<arr.count();i++){vec.push_back(arr[i].toInt());}
        }
    };



    for(auto i:m_jsonArray)
    {
        auto jo=i.toObject();

        jsonArrToVectorPoint(m_pointLoc_30d,"XY_NORMAL_VALUE_30d",jo);
        jsonArrToVectorPoint(m_pointLoc_60d,"XY_NORMAL_VALUE_60d",jo);
        jsonArrToVectorInt(m_value_30d,XY_NORMAL_VALUE,jo);
        jsonArrToVectorInt(m_value_60d,"NORMAL_VALUE15_35_60d",jo);

        jsonArrToVectorPoint(m_GHT1_RIGHT,"GHT1_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT2_RIGHT,"GHT2_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT3_RIGHT,"GHT3_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT4_RIGHT,"GHT4_RIGHT",jo);
        jsonArrToVectorPoint(m_GHT5_RIGHT,"GHT5_RIGHT",jo);

        if(cursorColor==2&&backGroundColor==1&&cursorSize==4)
        {
            jsonArrToVectorInt(m_v5,"PE_VALUE5_Blue_Yellow_B5",jo);
            jsonArrToVectorInt(m_v2,"PE_VALUE2_Blue_Yellow_B5",jo);
            jsonArrToVectorInt(m_v1,"PE_VALUE1_Blue_Yellow_B5",jo);
            jsonArrToVectorInt(m_v05,"PE_VALUE05_Blue_Yellow_B5",jo);
        }
        else
        {
            jsonArrToVectorInt(m_v5,"PE_VALUE5",jo);
            jsonArrToVectorInt(m_v2,"PE_VALUE2",jo);
            jsonArrToVectorInt(m_v1,"PE_VALUE1",jo);
            jsonArrToVectorInt(m_v05,"PE_VALUE05",jo);
        }
    }
    auto getIndex=[&](QPointF& dot,QVector<QPoint>& pointLoc)->int{
        int distMin=10000;
        int index=-1;

        for(int i=0;i<pointLoc.length();i++)
        {
            int dist;
            if(m_os_od==0)
            {
                dist=pow(pointLoc[i].rx()-dot.x(),2)+pow(pointLoc[i].ry()-dot.y(),2);
            }
            else
            {
                dist=pow(pointLoc[i].rx()-(-dot.x()),2)+pow(pointLoc[i].ry()-dot.y(),2);
            }
            if(dist<distMin){distMin=dist;index=i;}
        }
        return index;
    };

    for(int i=0;i<m_dotList.length();i++)
    {
        auto dot=m_dotList[i];
        float radius=sqrt(pow(dot.x(),2)+pow(dot.y(),2));
        int index;
        if(radius<=30)
        {
            index=getIndex(dot,m_pointLoc_30d);
            m_sv[i]=m_value_30d[index]/10;
            if(!(cursorSize==2&&cursorColor==0)){
                if(cursorColor==2)
                {
                    if(m_sv[i]>0){m_sv[i]-=2*(m_age_correction-1);} else if(m_sv[i]<0){m_sv[i]+=2*(m_age_correction-1);}
                }
                else
                {
                    if(m_sv[i]>0){m_sv[i]-=(m_age_correction-1);} else if(m_sv[i]<0){m_sv[i]+=(m_age_correction-1);}
                }
            }

        }
        else if(radius<=60)
        {
            index=getIndex(dot,m_pointLoc_60d);
            m_sv[i]=m_value_60d[index]/10;
            if(m_sv[i]>0) m_sv[i]-=m_age_correction;else if(m_sv[i]<0) m_sv[i]+=m_age_correction;
        }


        if(m_sv[i]>0)  {m_dev[i]=m_staticValues[i]-m_sv[i];}                                                   //dev 盲点
        else{ m_dev[i]=-99; }


        if(radius<=30)
        {
            if(m_dev[i]!=-99)
            {
                int v=-m_dev[i];
                if (v<=m_v5[index]) m_peDev[i]=0;
                else if(v<=m_v2[index]) m_peDev[i]=1;
                else if(v<=m_v1[index]) m_peDev[i]=2;
                else if(v<=m_v05[index]) m_peDev[i]=3;
                else m_peDev[i]=4;
            }
            else{
                m_peDev[i]=-99;
            }
        }

        if(m_sv[i]>0)
        {
            int ringIndex=round(radius/6+0.4)-1;
            if(ringIndex>4) ringIndex=4;

            m_vfiRingStandard[ringIndex]+=m_sv[i];
            m_vfiRingTest[ringIndex]+=m_staticValues[i];
        }
    }


    float vfih=0,vfid=0;
    for(int i=0;i<5;i++)
    {
        if(m_vfiRingStandard[i]!=0)
        {
//            qDebug()<<m_vfiRingTest[i];
//            qDebug()<<m_vfiRingStandard[i];
            float temp=float(m_vfiRingTest[i])/(m_vfiRingStandard[i]+0.0001);
//            qDebug()<<temp;
            if(temp>1){temp=1;}
            vfih+=m_VFI_Weight[i]*temp;
            vfid+=m_VFI_Weight[i];
        }
    }
//    qDebug()<<vfih;
//    qDebug()<<vfid;

    m_VFI=vfih/vfid;                                     //vfi
//    qDebug()<<m_VFI;

    for(auto& i: m_dev)
    {
        if(i!=-99) m_md+=i;
    }
    m_md/=(m_dev.length()-2);    //扣除盲点

//    qDebug()<<m_md;

    for(auto& i: m_dev)
    {
        if(i!=-99) m_psd+=pow(i-m_md,2);
    }
    m_psd=sqrt(m_psd/(m_dev.length()-2));


    if(m_psd>4.3) m_p_psd=0.5;
    else if(m_psd>3.7) m_p_psd=1;
    else if(m_psd>3.2) m_p_psd=2;
    else if(m_psd>2.5) m_p_psd=5;
    else if(m_psd>2.0) m_p_psd=10;

    if(m_md<-5.5)  m_p_md=0.5;
    else if(m_md<-3.5) m_p_md=1;
    else if(m_md<-2.6) m_p_md=2;
    else if(m_md<-2.0) m_p_md=5;
    else if(m_md<-1.5) m_p_md=10;

//    qDebug()<<m_psd;

    int md_correction=round(m_md+m_psd);
//    qDebug()<<md;

    for(int i=0;i<m_dev.length();i++)
    {
        if(m_dev[i]==-99)
            m_mDev[i]=-99;
        else
            m_mDev[i]=m_dev[i]-md_correction;
    }

    for(int i=0;i<m_mDev.length();i++)
    {
        auto dot=m_dotList[i];
        auto index=getIndex(dot,m_pointLoc_30d);
        float radius=sqrt(pow(dot.x(),2)+pow(dot.y(),2));
        if(radius<30)
        {
            if(m_mDev[i]==-99)
                m_peMDev[i]=-99;
            else
            {
                int v=-m_mDev[i];
                if (v<=m_v5[index]) m_peMDev[i]=0;
                else if(v<=m_v2[index]) m_peMDev[i]=1;
                else if(v<=m_v1[index]) m_peMDev[i]=2;
                else if(v<=m_v05[index]) m_peMDev[i]=3;
                else m_peMDev[i]=4;
            }
        }
    }




    auto isGHT=[](QVector<QPoint> GHTArr,QPoint point)->bool
    {
        for(auto&i:GHTArr)
        {
            if(i.x()==point.x()&&i.y()==point.y()) return true;
        }
        return false;
    };


    float ght[5][2];
    bool b27=false;
    memset(ght,0,sizeof(ght));
    for(int i=0;i<m_dotList.length();i++)
    {
        int v;QPoint dot;
        if(m_mDev[i]!=-99)
            v=-m_mDev[i];

        if(m_os_od==0)
           dot=m_dotList[i].toPoint();
        else
           dot={-m_dotList[i].toPoint().x(),m_dotList[i].toPoint().y()};

        if(isGHT(m_GHT1_RIGHT,dot)){ght[0][0]+=v;}
        if(isGHT(m_GHT2_RIGHT,dot)){if(v>=7&&abs(dot.x())==abs(dot.y())) b27=true;ght[1][0]+=v;}
        if(isGHT(m_GHT3_RIGHT,dot)){ght[2][0]+=v;}
        if(isGHT(m_GHT4_RIGHT,dot)){ght[3][0]+=v;}
        if(isGHT(m_GHT5_RIGHT,dot)){ght[4][0]+=v;}
        if(isGHT(m_GHT1_RIGHT,{dot.x(),-dot.y()})){ght[0][1]+=v;}
        if(isGHT(m_GHT2_RIGHT,{dot.x(),-dot.y()})){if(v>=7&&abs(dot.x())==abs(dot.y())) b27=true;ght[1][1]+=v;}
        if(isGHT(m_GHT3_RIGHT,{dot.x(),-dot.y()})){ght[2][1]+=v;}
        if(isGHT(m_GHT4_RIGHT,{dot.x(),-dot.y()})){ght[3][1]+=v;}
        if(isGHT(m_GHT5_RIGHT,{dot.x(),-dot.y()})){ght[4][1]+=v;}
    }

    ght[0][0] *= 1 / 3;
    ght[1][0] *= 1 / 4;
    ght[2][0] *= 1 / 5;
    ght[3][0] *= 1 / 6;
    ght[4][0] *= 1 / 4;

    ght[0][1] *= 1 / 3;
    ght[1][1] *= 1 / 4;
    ght[2][1] *= 1 / 5;
    ght[3][1] *= 1 / 6;
    ght[4][1] *= 1 / 4;

    m_GHT=3;
    if (ght[0][0] >= 5 || ght[0][1] >= 5 || ght[1][0] >= 5 || ght[1][1] >= 5 ||
        ght[2][0] >= 6 || ght[2][1] >= 6 || ght[3][0] >= 6 || ght[3][1] >= 6 || ght[4][0] >= 7 || ght[4][1] >= 7)
    {
        m_GHT = 0;
    }
    else if(b27)
    {
        m_GHT = 1;
    }
    else if(ght[0][0] > 3 || ght[0][1] > 3 || ght[2][0] >= 4 || ght[2][1] >= 4 || ght[3][0] >= 4 || ght[3][1] >= 4)
    {
        m_GHT = 2;
    }



}

void AnalysisSvc::ThresholdAnalysis(int checkResultId)
{
    CheckResult_ptr checkResult_ptr;
    checkResult_ptr->m_id=checkResultId;
    qx::dao::fetch_by_id(checkResult_ptr);
    Program_ptr program_ptr;
    program_ptr->m_id=checkResult_ptr->m_program->m_id;
    qx::dao::fetch_by_id(program_ptr);
    Patient_ptr patient_ptr;
    patient_ptr->m_id=checkResult_ptr->m_patient->m_id;
    qx::dao::fetch_by_id(patient_ptr);

    CheckResultModel<Type::ThreshHold> checkResult(checkResult_ptr);
    ProgramModel<Type::ThreshHold> program(program_ptr);


}

//void AnalysisSvc::dynamicAnalysis()
//{

//}

}
