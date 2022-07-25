#include "analysislobbylistvm.h"
#include <perimeter/main/database/patient.h>
#include <perimeter/main/database/program.h>
#include <perimeter/main/database/checkResult.h>
#include <perimeter/main/model/programModel.h>
#include <perimeter/main/model/checkResultModel.h>

namespace Perimeter{
class SimpleCheckResultList:public QAbstractListModel
{
    enum SimpleCheckResultListRoles
    {
        checkResultId=Qt::UserRole+1,
        picName,
    };

    struct Data
    {
        int checkResultId;
        QString picName;
    };
    Q_OBJECT
    // QAbstractItemModel interface
public:
    SimpleCheckResultList()=default;
//    SimpleCheckResultList(QVector<int> checkResultIdList,QVector<QString> picNameList):
//        m_checkResultIdList(checkResultIdList),m_picNameList(picNameList){}
    ~SimpleCheckResultList()=default;

    virtual int rowCount(const QModelIndex &parent) const override
    {
        return m_dataList.count();
    }
    virtual QVariant data(const QModelIndex &index, int role) const override
    {
//        QVector<QString> picName={"dBDiagram.bmp","defectDepth.bmp","gray.bmp","PatterDeviation.bmp"};
        switch (role)
        {
            case SimpleCheckResultListRoles::checkResultId:return m_dataList[index.row()].checkResultId;
            case SimpleCheckResultListRoles::picName:return m_dataList[index.row()].picName;
        }
        return QVariant();
    }
    virtual QHash<int, QByteArray> roleNames() const override
    {
        QHash<int, QByteArray> roles;
        roles[checkResultId] = "checkResultId";
        roles[picName] = "picName";
        return roles;
    }
public:
    QVector<Data> m_dataList;

};








AnalysisLobbyListVm::AnalysisLobbyListVm()
{

}




AnalysisLobbyListVm::AnalysisLobbyListVm(const QVariantList &args)
{
    initialize();
    int id=args[0].toInt();
    m_imageHeight=args[1].toFloat();
    Patient_ptr patient_ptr(new Patient());patient_ptr->m_id=id;
    qx::dao::fetch_by_id(patient_ptr);
    auto birthDate=patient_ptr->m_birthDate;
    auto currentDate=QDateTime::currentDateTime().date();
    m_patientAge = currentDate.year()- birthDate.year();
    if (currentDate.month() < birthDate.month() || (currentDate.month() ==birthDate.month() && currentDate.day() < birthDate.day()))
        { m_patientAge--;}

    qx_query query;
    query.where("patient_id").isEqualTo(id);
    CheckResult_List checkResult_list;
    qx::dao::fetch_by_query(query, checkResult_list);              //不能用execute_query
    qDebug()<<checkResult_list.size();
    generateModelListData(checkResult_list);
}

void AnalysisLobbyListVm::initialize()
{
    QFile jsonFile("./data.json");
    if( !jsonFile.open(QIODevice::ReadOnly)) {qDebug() << "read file error!";return;}
    QJsonParseError jsonParserError;
    auto JsonDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
    m_jsonArray=JsonDoc.array();
    jsonFile.close();


}

void AnalysisLobbyListVm::initData(CheckResultModel<Type::ThreshHold> &checkResultModel)
{
    auto params =checkResultModel.m_params ;
    int cursorSize=int(params.commonParams.cursorSize);
    int cursorColor=int(params.commonParams.cursorColor);
    int backGroundColor=int(params.commonParams.backGroundColor);

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


    m_pointLoc_30d.clear();
    m_pointLoc_60d.clear();
    m_value_30d.clear();
    m_value_60d.clear();
    m_age_correction=0;

    m_v5.clear();
    m_v2.clear();
    m_v1.clear();
    m_v05.clear();


    if(m_patientAge<=35){m_age_correction=0;}
    else if(m_patientAge<=45){m_age_correction=1;}
    else if(m_patientAge<=55){m_age_correction=2;}
    else if(m_patientAge<=65){m_age_correction=3;}
    else {m_age_correction=4;}


    for(auto i:m_jsonArray)
    {
        auto jo=i.toObject();

        jsonArrToVectorPoint(m_pointLoc_30d,"XY_NORMAL_VALUE_30d",jo);
        jsonArrToVectorPoint(m_pointLoc_60d,"XY_NORMAL_VALUE_60d",jo);
        jsonArrToVectorInt(m_value_30d,XY_NORMAL_VALUE,jo);
        jsonArrToVectorInt(m_value_60d,"NORMAL_VALUE15_35_60d",jo);

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
}


//按照日期分组
void AnalysisLobbyListVm::generateModelListData(CheckResult_List checkResult_list)
{
    QVector<QDate> dateList;
    qDebug()<<checkResult_list.size();
    for(auto &i:checkResult_list)
    {
        if(!dateList.contains(i->m_time.date()))
        {dateList.append(i->m_time.date());}
    }
    qDebug()<<dateList.length();


    std::sort(dateList.begin(),dateList.end(),[](const QDate& d1,const QDate& d2){
        return  !(d1<d2);
    });

    for(int i=0;i<dateList.length();i++ )
    {
        Data data;
        data.year=QString::number(dateList[i].year());
//        data.monthDay=QString::number(dateList[i].month())+"/"+QString::number(dateList[i].day());
        data.monthDay=dateList[i].toString("MM/dd");
        qDebug()<<data.year;
        qDebug()<<data.monthDay;
        SimpleCheckResultList* simpleCheckResultList=new SimpleCheckResultList();
        for(int j=0;j<checkResult_list.length();j++)
        {
            qDebug()<<checkResult_list[j]->m_time.date();
            qDebug()<<dateList[i];
            if(checkResult_list[j]->m_time.date()==dateList[i])
            {
                auto imageFileName=drawImage(checkResult_list[j]);
                simpleCheckResultList->m_dataList.append({checkResult_list[j]->m_id,imageFileName});
                data.simpleCheckResult=simpleCheckResultList;
            }
        }
        m_dataList.append(data);
    }
}


QString AnalysisLobbyListVm::drawImage(CheckResult_ptr checkResult_ptr)
{
    QString os_od=checkResult_ptr->m_OS_OD?"OS":"OD";
    QString checkTime=checkResult_ptr->m_time.time().toString("HH:mm:ss");
    Program_ptr program_ptr(new Program());
    program_ptr->m_id=checkResult_ptr->m_program->m_id;
    qx::dao::fetch_by_id(program_ptr);
    QString programName=program_ptr->m_name;
    m_image=QImage(m_imageHeight*0.8,m_imageHeight*0.8,QImage::Format_ARGB32);
    auto total=QImage(m_imageHeight*0.8,m_imageHeight,QImage::Format_ARGB32);
    auto info=QImage(m_imageHeight*0.8,m_imageHeight*0.2,QImage::Format_ARGB32);
    qDebug()<<programName;
    auto program_reports= Utility::QStringToEntity<std::vector<int>>(program_ptr->m_report);
    for(auto&i:program_reports)
    {
        qDebug()<<i;
    }
    auto checkReport=[&](int report)->bool{return std::find(program_reports.begin(),program_reports.end(),report)!=program_reports.end();};
    qDebug()<<checkReport(0);
    qDebug()<<checkReport(2);
    if(checkReport(0)||checkReport(2))        //常规分析和总览显示概率图
    {
        auto crm=CheckResultModel<Type::ThreshHold>(checkResult_ptr);
        auto pm=ProgramModel<Type::ThreshHold>(program_ptr);
        drawPatternPE(crm,pm);
    }
    else if(checkReport(1))      //三合一显示量化缺损
    {
        auto crm=CheckResultModel<Type::ThreshHold>(checkResult_ptr);
        auto pm=ProgramModel<Type::ThreshHold>(program_ptr);
        drawDefectDepth(crm,pm);
    }
    else if(checkReport(3))      //显示筛选
    {
        auto crm=CheckResultModel<Type::ThreshHold>(checkResult_ptr);
        auto pm=ProgramModel<Type::ThreshHold>(program_ptr);
        drawScreening(crm,pm);
    }
    else                         //移动
    {
        auto crm=CheckResultModel<Type::Dynamic>(checkResult_ptr);
        auto pm=ProgramModel<Type::Dynamic>(program_ptr);
        drawDynamic(crm,pm);
    }
    info.fill(qRgb(255, 255, 255));
    QPainter painter(&info);
//    int fontPixSize=info.height()*0.7;
//    font.setPixelSize(fontPixSize);
//    font.setBold(true);
    painter.setFont({"consolas",int(info.height()*0.5),QFont::Weight::Bold});
    painter.drawText(QRect{QPoint{int(info.width()*0.03),int(info.height()*0.2)},QPoint{int(info.width()*0.22),int(info.height()*0.8)}},Qt::AlignCenter,os_od);
    painter.drawLine(QPoint{int(info.width()*0.25),int(info.height()*0.1)},QPoint{int(info.width()*0.25),int(info.height()*0.9)});
    painter.setFont({"consolas",int(info.height()*0.30),QFont::Weight::Black});
    painter.drawText(QRect{QPoint{int(info.width()*0.28),int(info.height()*0.10)},QPoint{int(info.width()*0.95),int(info.height()*0.5)}},Qt::AlignCenter,programName);
    painter.drawText(QRect{QPoint{int(info.width()*0.28),int(info.height()*0.5)},QPoint{int(info.width()*0.95),int(info.height()*0.90)}},Qt::AlignCenter,checkTime);


    QString imageSavePath="./analysisLobbyImage/"+QString::number(checkResult_ptr->m_id)+".bmp";
    total.fill(qRgb(255, 255, 255));
    QPainter painter2(&total);
    painter2.drawImage((total.width()-m_image.width())/2,0,m_image);
    painter2.drawImage(0,total.height()*0.8+1,info);
    total.save(imageSavePath);
    return imageSavePath;
}

void AnalysisLobbyListVm::drawPatternPE(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel)
{
    m_image.fill(qRgb(255, 255, 255));
    initData(checkResultModel);
    int range=programModel.m_params.commonParams.Range[1];
    int os_od=checkResultModel.m_OS_OD;
    drawPixScale(range);
    auto params =checkResultModel.m_params ;
    int cursorSize=int(params.commonParams.cursorSize);
    int cursorColor=int(params.commonParams.cursorColor);
    auto checkResultValues=checkResultModel.m_data.checkdata;
    QVector<QPointF> dotList;
    for(auto&i:programModel.m_data.dots){dotList.append({i.x,i.y});}

    QPainter painter(&m_image);
    float scale=1;

    QVector<int> sv;
    QVector<int> dev;
    QVector<int> mDev;
    QVector<int> peDev;
    QVector<int> peMDev;
    float md,psd,p_md,p_psd;

    sv.fill(0,dotList.length());
    dev.fill(0,dotList.length());
    mDev.fill(0,dotList.length());
    peDev.fill(0,dotList.length());
    peMDev.fill(0,dotList.length());
    md=0;
    psd=0;

    for(int i=0;i<dotList.length();i++)
    {
        auto dot=dotList[i];
        float radius=sqrt(pow(dot.x(),2)+pow(dot.y(),2));
        int index;
        if(radius<=30)
        {
            index=getIndex(dot,m_pointLoc_30d,os_od);
            sv[i]=m_value_30d[index]/10;
            if(!(cursorSize==2&&cursorColor==0)){
                if(cursorColor==2)
                {
                    if(sv[i]>0){sv[i]-=2*(m_age_correction-1);} else if(sv[i]<0){sv[i]+=2*(m_age_correction-1);}
                }
                else
                {
                    if(sv[i]>0){sv[i]-=(m_age_correction-1);} else if(sv[i]<0){sv[i]+=(m_age_correction-1);}
                }
            }

        }
        else if(radius<=60)
        {
            index=getIndex(dot,m_pointLoc_60d,os_od);
            sv[i]=m_value_60d[index]/10;
            if(sv[i]>0) sv[i]-=m_age_correction;else if(sv[i]<0) sv[i]+=m_age_correction;
        }

        if(sv[i]>0)  {dev[i]=checkResultModel.m_data.checkdata[i]-sv[i];}                                                   //dev 盲点
        else{ dev[i]=-99; }

        if(radius<=30)
        {
            if(dev[i]!=-99)
            {
                int v=dev[i];
                if (v<=m_v5[index]) peDev[i]=0;
                else if(v<=m_v2[index]) peDev[i]=1;
                else if(v<=m_v1[index]) peDev[i]=2;
                else if(v<=m_v05[index]) peDev[i]=3;
                else peDev[i]=4;
            }
            else{
                peDev[i]=-99;
            }
        }
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

    int md_correction=round(md+psd);

    for(int i=0;i<dev.length();i++)
    {
        if(dev[i]==-99)
            mDev[i]=-99;
        else
            mDev[i]=dev[i]-md_correction;
    }

    for(int i=0;i<mDev.length();i++)
    {
        auto dot=dotList[i];
        auto index=getIndex(dot,m_pointLoc_30d,os_od);
        float radius=sqrt(pow(dot.x(),2)+pow(dot.y(),2));
        if(radius<30)
        {
            if(mDev[i]==-99)
                peMDev[i]=-99;
            else
            {
                int v=-mDev[i];
                if (v<=m_v5[index]) peMDev[i]=0;
                else if(v<=m_v2[index]) peMDev[i]=1;
                else if(v<=m_v1[index]) peMDev[i]=2;
                else if(v<=m_v05[index])peMDev[i]=3;
                else peMDev[i]=4;
            }
        }
    }



    for(int i=0;i<dotList.length()&&i<int(checkResultValues.size());i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(dotList[i],range);
        QString path=QString(":/grays/PE")+QString::number(peMDev[i])+".bmp";
        QImage image(path);
        auto scaledImage=image.scaled(image.width()*scale,image.height()*scale);
        QPoint tempPixLoc={pixLoc.x()-scaledImage.width()/2,pixLoc.y()-scaledImage.height()/2};
        painter.drawImage(tempPixLoc,scaledImage);
    }
}

void AnalysisLobbyListVm::drawDefectDepth(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel)
{
    m_image.fill(qRgb(255, 255, 255));
    initData(checkResultModel);
    int range=programModel.m_params.commonParams.Range[1];
    int os_od=checkResultModel.m_OS_OD;
    drawPixScale(range);
    auto params =checkResultModel.m_params ;
    int cursorSize=int(params.commonParams.cursorSize);
    int cursorColor=int(params.commonParams.cursorColor);
    auto checkResultValues=checkResultModel.m_data.checkdata;
    QVector<QPointF> dotList;
    for(auto&i:programModel.m_data.dots){dotList.append({i.x,i.y});}
    QPainter painter(&m_image);
    int fontPixSize=m_image.width()/18;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);

    QVector<int> sv;
    QVector<int> dev;
    sv.fill(0,dotList.length());
    dev.fill(0,dotList.length());

    for(int i=0;i<dotList.length();i++)
    {
        auto dot=dotList[i];
        float radius=sqrt(pow(dot.x(),2)+pow(dot.y(),2));
        int index;
        if(radius<=30)
        {
            index=getIndex(dot,m_pointLoc_30d,os_od);
            sv[i]=m_value_30d[index]/10;
            if(!(cursorSize==2&&cursorColor==0)){
                if(cursorColor==2)
                {
                    if(sv[i]>0){sv[i]-=2*(m_age_correction-1);} else if(sv[i]<0){sv[i]+=2*(m_age_correction-1);}
                }
                else
                {
                    if(sv[i]>0){sv[i]-=(m_age_correction-1);} else if(sv[i]<0){sv[i]+=(m_age_correction-1);}
                }
            }

        }
        else if(radius<=60)
        {
            index=getIndex(dot,m_pointLoc_60d,os_od);
            sv[i]=m_value_60d[index]/10;
            if(sv[i]>0) sv[i]-=m_age_correction;else if(sv[i]<0) sv[i]+=m_age_correction;
        }

        if(sv[i]>0)  {dev[i]=checkResultModel.m_data.checkdata[i]-sv[i];}                                                   //dev 盲点
        else{ dev[i]=-99; }
    }



    for(int i=0;i<dotList.length()&&i<dev.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(dotList[i],range);
        if(dev[i]==-99) continue;
        if(dev[i]>-4)
        {
            float scale=1;
            QImage image(":/grays/DE0.bmp");
            image=image.scaled(image.width()*scale,image.height()*scale);
            painter.drawImage(QPointF{pixLoc.x()-image.width()*0.48,pixLoc.y()-image.height()*0.50},image);
        }
        else
        {
            const QRect rectangle = QRect(pixLoc.x()-fontPixSize*1.6*0.45, pixLoc.y()-fontPixSize*0.8/2, fontPixSize*1.6,fontPixSize*0.8);
            painter.drawText(rectangle,Qt::AlignCenter,QString::number(-dev[i]));
            m_image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000);   //标个小红点
        }
    }
}

void AnalysisLobbyListVm::drawScreening(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel)
{
    m_image.fill(qRgb(255, 255, 255));
    int range=programModel.m_params.commonParams.Range[1];
    int os_od=checkResultModel.m_OS_OD;
    drawPixScale(range);
    auto checkResultValues=checkResultModel.m_data.checkdata;
    QVector<QPointF> dotList;
    for(auto&i:programModel.m_data.dots){dotList.append({i.x,i.y});}
    QPainter painter(&m_image);
    QPoint blindDot;
    if(os_od==0) blindDot={-14,0};else blindDot={14,0};
    auto pixLoc=convertDegLocToPixLoc(blindDot,range);
    QImage blindImage(":/grays/SE3.bmp");
    QImage screenImageSE0(":/grays/SE0.bmp");
    QImage screenImageSE1(":/grays/SE1.bmp");
    QImage screenImageSE2(":/grays/SE2.bmp");
    int scale=1;
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
    for(int i=0;i<dotList.length()&&i<int(checkResultValues.size());i++)
    {
        auto pixLoc=convertDegLocToPixLoc(dotList[i],range);
        auto imageLoc=QPoint{pixLoc.x()-screenImageSE0.width()/2,pixLoc.y()-screenImageSE0.height()/2};
        switch (checkResultValues[i])
        {
        case 0:painter.drawImage(imageLoc,screenImageSE2);break;
        case 1:painter.drawImage(imageLoc,screenImageSE1);break;
        case 2:painter.drawImage(imageLoc,screenImageSE0);break;
        default:break;
        }
    }
}

void AnalysisLobbyListVm::drawDynamic(CheckResultModel<Type::Dynamic>& checkResultModel,ProgramModel<Type::Dynamic>& programModel)
{
    m_image.fill(qRgb(255, 255, 255));
    int range=programModel.m_params.Range[1];
    QVector<QPointF> dynamicValues;
    for(auto&i:checkResultModel.m_data.checkdata){dynamicValues.append({i.x,i.y});}
    drawRoundCrossPixScale(range);
    int scale=1;
    QPainter painter(&m_image);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    qDebug()<<dynamicValues.length();
    for(int i=0;i<dynamicValues.length();i++)
    {
        auto begin=convertDegLocToPixLoc(dynamicValues[i],range);
        auto end=convertDegLocToPixLoc(dynamicValues[(i+1)%dynamicValues.length()],range);
        painter.drawLine(QLine(begin.x(),begin.y(),end.x(),end.y()));
        painter.drawEllipse(begin,3*scale,3*scale);
    }
}

void AnalysisLobbyListVm::drawPixScale(int range)
{
    int scale=1;
    QPainter painter(&m_image);
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    painter.drawLine(QLine(0,m_image.height()/2,m_image.width(),m_image.height()/2));
    painter.drawLine(QLine(m_image.width()/2,0,m_image.width()/2,m_image.height()));

    int segmentCount=range/10;
    for(int i=0;i<=segmentCount*2;i++)
    {
        if(i!=segmentCount)
        {
            painter.drawLine(QLine((float(m_image.width()-scale))/(segmentCount*2)*i+scale/2,m_image.height()/2-2*scale,(float(m_image.width()-scale))/(segmentCount*2)*i+scale/2,m_image.height()/2+2*scale));
            painter.drawLine(QLine(m_image.width()/2-2*scale,(float(m_image.height()-scale))/(segmentCount*2)*i+scale/2,m_image.width()/2+2*scale,(float(m_image.height()-scale))/(segmentCount*2)*i+scale/2));
        }
    }
}

void AnalysisLobbyListVm::drawRoundCrossPixScale(int range)
{
        int scale=1;
        QPainter painter(&m_image);

        painter.setBrush(QBrush(QColor("black")));
        painter.setPen({Qt::black,float(scale)});
        painter.drawLine(QLine(0,m_image.height()/2,m_image.width(),m_image.height()/2));
        painter.drawLine(QLine(m_image.width()/2,0,m_image.width()/2,m_image.height()));

    //    auto drawDashLine=[]()->void{};
        int segmentCount=range/10;
        QPen pen;
        pen.setDashPattern({float(3*scale),float(5*scale)});
        pen.setWidth(float(scale));
        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing);
        for(int i=1;i<=segmentCount;i++)
        {
            int radius=float(m_image.width())/2/segmentCount*i+scale/2;
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

//不会截断线了 难看
//        painter.setRenderHint(QPainter::RenderHint::Antialiasing,false);
//        painter.setPen({Qt::black,float(scale)});
//        for(int i=0;i<=segmentCount*2;i++)
//        {
//            if(i!=segmentCount)
//            {
//                painter.drawLine(QLine((float(m_image.width()-scale))/(segmentCount*2)*i+scale/2,m_image.height()/2-4*scale,(float(m_image.width()-scale))/(segmentCount*2)*i+scale/2,m_image.height()/2+4*scale));
//                painter.drawLine(QLine(m_image.width()/2-4*scale,(float(m_image.height()-scale))/(segmentCount*2)*i+scale/2,m_image.width()/2+4*scale,(float(m_image.height()-scale))/(segmentCount*2)*i+scale/2));
//            }
//        }
}

QPoint AnalysisLobbyListVm::convertDegLocToPixLoc(QPointF DegLoc,int range)
{
    float pixPerDegW=float(m_image.width()/2)/range;
    float pixPerDegH=float(m_image.height()/2)/range;
    return QPoint(m_image.width()/2+DegLoc.x()*pixPerDegW,m_image.height()/2-DegLoc.y()*pixPerDegH);
}

int AnalysisLobbyListVm::getIndex(QPointF &dot, QVector<QPoint> &pointLoc,int os_od)
{
    int distMin=10000;
    int index=-1;

    for(int i=0;i<pointLoc.length();i++)
    {
        int dist;
        if(os_od==0)
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
}



AnalysisLobbyListVm::~AnalysisLobbyListVm()
{
    for(auto& i:m_dataList)
    {
        if(i.simpleCheckResult!=nullptr)
        {
            auto temp=static_cast<SimpleCheckResultList*>(i.simpleCheckResult);
            if(temp!=nullptr) delete temp;
        }
    }
}

void AnalysisLobbyListVm::refreshData()
{

}

int AnalysisLobbyListVm::rowCount(const QModelIndex &parent) const
{
    return m_dataList.length();
}

QVariant AnalysisLobbyListVm::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case year:return m_dataList[index.row()].year;
    case monthDay:return m_dataList[index.row()].monthDay;
    case simpleCheckResult:return QVariant::fromValue(m_dataList[index.row()].simpleCheckResult);
    }
    return QVariant();
}

QHash<int, QByteArray> AnalysisLobbyListVm::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[year] = "year";
    roles[monthDay]="monthDay";
    roles[simpleCheckResult] = "simpleCheckResult";
    return roles;
}






//template<Type type>
//void AnalysisLobbyListVm::drawImage(CheckResultModel<type> checkResultModel)
//{

//}
}
#include "analysislobbylistvm.moc"
