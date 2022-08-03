#include "progressAnalysislistVm.h"
#include <perimeter/main/database/checkResult.h>


namespace Perimeter {

ProgressAnalysisListVm::ProgressAnalysisListVm(const QVariantList &args)
{
    qDebug()<<"into constructor";
    initialize();
    int patientId=args[0].toInt();
    m_OS_OD=args[1].toInt();
    qx_query query;
    CheckResult_List checkResult_List;
//    query.where("patient_id").isEqualTo(patientId).orderAsc("time");
//    query.where("patient_id").isEqualTo(patientId)/*.where("program_id").in({1,4})*/;
    query.where("patient_id").isEqualTo(patientId).and_("program_id").in({1,4}).and_("OS_OD").isEqualTo(m_OS_OD);
    qx::dao::fetch_by_query(query,checkResult_List);



    m_patient=new PatientVm(QVariantList{patientId});
    m_patientAge=m_patient->getAge();
    m_program_30_2=new StaticProgramVM(QVariantList{1});
    m_program_24_2=new StaticProgramVM(QVariantList{4});

    for(int i=0;i<checkResult_List.size();i++)
    {
        m_checkResultList.push_back(CheckResultModel<Type::ThreshHold>(checkResult_List[i]));
    }

    for(int i=0;i<m_checkResultList.size();i++)
    {
        if(m_checkResultList[i].m_program_id==1)
        {
            m_originalDataList.push_back(getProgressData(m_checkResultList[i],m_program_30_2));
        }
        else
        {
            m_originalDataList.push_back(getProgressData(m_checkResultList[i],m_program_24_2));
        }
    }
    qDebug()<<m_originalDataList.length();
    qDebug()<<m_originalDataList[0].program;
    m_currentDataList=m_originalDataList;
//    for(int i=0;i<m_currentDataList.length();i++)
//    {
//        if(i<2)  m_currentDataList[i].baseLine=true;
//        else m_currentDataList[i].baseLine=false;
//    }
    m_currentDataList[0].baseLine=true;
    m_currentDataList[1].baseLine=true;
    qDebug()<<m_currentDataList.length();
}

ProgressAnalysisListVm::~ProgressAnalysisListVm()
{
    if(m_patient!=nullptr) delete m_patient;m_patient=nullptr;
    if(m_program_24_2!=nullptr) delete m_program_24_2;m_program_24_2=nullptr;
    if(m_program_30_2!=nullptr) delete m_program_30_2;m_program_30_2=nullptr;
}

void ProgressAnalysisListVm::removeCheckResult(int checkResultId)
{


}

void ProgressAnalysisListVm::initialize()
{
    QFile jsonFile("./data.json");
    if( !jsonFile.open(QIODevice::ReadOnly)) {qDebug() << "read file error!";return;}
    QJsonParseError jsonParserError;
    auto JsonDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
    m_jsonArray=JsonDoc.array();
    jsonFile.close();

}

ProgressAnalysisListVm::Data ProgressAnalysisListVm::getProgressData(const CheckResultModel<Type::ThreshHold> &result, Perimeter::StaticProgramVM *program)
{

    QVector<QPointF> m_dotList;
    QVector<int> staticValues;

    QVector<QPoint> pointLoc_30d;
    QVector<QPoint> pointLoc_60d;
    QVector<int> value_30d;
    QVector<int> value_60d;
    int age_correction;
    QVector<int> sv;
    QVector<int> dev;
    float md,psd;
    int m_range;

    auto dotList=program->getDots();
    m_dotList.resize(dotList.length());
    for(int i=0;i<dotList.length();i++){m_dotList[i]=dotList[i].toPointF();}
    auto range=program->getParams()->getCommonParams()->getRange();
    m_range=range[1];
    auto values=result.m_data.checkdata;
    staticValues.resize(values.size());
    for(int i=0;i<int(values.size());i++){staticValues[i]=values[i];}

    pointLoc_30d.clear();
    pointLoc_60d.clear();
    value_30d.clear();
    value_60d.clear();
    age_correction=0;

    sv.fill(0,m_dotList.length());
    dev.fill(0,m_dotList.length());

    md=0;
    psd=0;

    if(m_patientAge<=35){age_correction=0;}
    else if(m_patientAge<=45){age_correction=1;}
    else if(m_patientAge<=55){age_correction=2;}
    else if(m_patientAge<=65){age_correction=3;}
    else {age_correction=4;}


    auto params =result.m_params;
    int cursorSize=int(params.commonParams.cursorSize);
    int cursorColor=int(params.commonParams.cursorColor);
//    int backGroundColor=int(params.commonParams.backGroundColor);

    QString XY_NORMAL_VALUE;

    if(cursorSize==2&&cursorColor==0)  //30 内的程序才能调尺寸,颜色,此种尺寸颜色比较精确
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

        jsonArrToVectorPoint(pointLoc_30d,"XY_NORMAL_VALUE_30d",jo);
        jsonArrToVectorPoint(pointLoc_60d,"XY_NORMAL_VALUE_60d",jo);
        jsonArrToVectorInt(value_30d,XY_NORMAL_VALUE,jo);
        jsonArrToVectorInt(value_60d,"NORMAL_VALUE15_35_60d",jo);

    }
    auto getIndex=[&](QPointF& dot,QVector<QPoint>& pointLoc)->int{
        int distMin=10000;
        int index=-1;

        for(int i=0;i<pointLoc.length();i++)
        {
            int dist;
            if(m_OS_OD==0)
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
            index=getIndex(dot,pointLoc_30d);
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
            index=getIndex(dot,pointLoc_60d);
            sv[i]=value_60d[index]/10;
            if(sv[i]>0) sv[i]-=age_correction;else if(sv[i]<0) sv[i]+=age_correction;
        }

        if(sv[i]>0)  {dev[i]=staticValues[i]-sv[i];}                                                   //dev 盲点
        else{ dev[i]=-99; }
    }


    for(auto& i: dev)
    {
        if(i!=-99) md+=i;
    }
    md/=(dev.length()-2);    //扣除盲点


    for(auto& i: dev)
    {
        if(i!=-99) psd+=pow(i-md,2);
    }
    psd=sqrt(psd/(dev.length()-2));

    QString time=result.m_time.toString("yyyy/MM/dd HH:mm:ss");
    QString programName=program->getName();
    auto strategy=result.m_params.commonParams.strategy;
    QString strategyName=QVector<QString>{"全阈值","智能交互式","快速智能交互式","未知"}[int(strategy)<3?int(strategy):3];
    auto& resultData=result.m_data;
    float fixationLossRate=resultData.fixationLostTestCount!=0?float(resultData.fixationLostCount)/resultData.fixationLostTestCount:0;
    float falseNegativeRate=resultData.falseNegativeTestCount!=0?float(resultData.falseNegativeCount)/resultData.falseNegativeTestCount:0;
    float falsePositiveRate=resultData.falsePositiveTestCount!=0?float(resultData.falsePositiveCount)/resultData.falsePositiveTestCount:0;

    return Data{result.m_id,false,time,programName,strategyName,md,psd,fixationLossRate,falseNegativeRate,falsePositiveRate};
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
