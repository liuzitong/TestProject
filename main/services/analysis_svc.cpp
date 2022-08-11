#include "analysis_svc.h"
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QLine>
#include <QtMath>
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

AnalysisSvc* AnalysisSvc::singleton=nullptr;

AnalysisSvc::AnalysisSvc()
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
    delete singleton;
}


void AnalysisSvc::ThresholdAnalysis(int resultId,QVector<int>& dev,QVector<int>& mDev,QVector<int>& peDev,QVector<int>& peMDev,float& md,float& psd,float& VFI,int& GHT, float& p_md,float& p_psd)
{
    CheckResult_ptr checkResult_ptr(new CheckResult());
    checkResult_ptr->m_id=resultId;
    qx::dao::fetch_by_id(checkResult_ptr);
    Program_ptr program_ptr(new Program());
    program_ptr->m_id=checkResult_ptr->m_program->m_id;
    qx::dao::fetch_by_id(program_ptr);
    Patient_ptr patient_ptr(new Patient());
    patient_ptr->m_id=checkResult_ptr->m_patient->m_id;
    qx::dao::fetch_by_id(patient_ptr);

    CheckResultModel<Type::ThreshHold> checkResult(checkResult_ptr);
    ProgramModel<Type::ThreshHold> program(program_ptr);
    PatientModel patient(patient_ptr);


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
    dev.fill(0,dotList.size());
    mDev.fill(0,dotList.size());
    peDev.fill(0,dotList.size());
    peMDev.fill(0,dotList.size());

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

    VFI=vfih/vfid;                                     //vfi
//    qDebug()<<m_VFI;
    md=0;
    for(auto& i: dev)
    {
        if(i!=-99) md+=i;
    }
    md/=(dev.length()-2);    //扣除盲点

//    qDebug()<<m_md;
    psd=0;
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

    GHT=3;
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
}

void AnalysisSvc::ScreeningAnalysis(int resultId,int& dotSeen,int& dotWeakSeen,int& dotUnseen)
{
    CheckResult_ptr checkResult_ptr(new CheckResult());
    checkResult_ptr->m_id=resultId;
    qx::dao::fetch_by_id(checkResult_ptr);
    Program_ptr program_ptr(new Program());
    program_ptr->m_id=checkResult_ptr->m_program->m_id;
    qx::dao::fetch_by_id(program_ptr);

    CheckResultModel<Type::ThreshHold> checkResult(checkResult_ptr);
    ProgramModel<Type::ThreshHold> program(program_ptr);

    if(checkResult.m_type==Type::Screening)
    {
        dotSeen=0,dotWeakSeen=0,dotUnseen=0;
        auto& checkData=checkResult.m_data.checkdata;
        for(auto& i:checkData)
        {
            if(i==0) dotUnseen++;
            else if(i==1) dotWeakSeen++;
            else if(i==2) dotSeen++;
        }
    }
}

AnalysisSvc *AnalysisSvc::getSingleton()
{
    if (singleton==nullptr)
    {
        singleton=new AnalysisSvc();
    }
    return singleton;
}


void AnalysisSvc::drawPixScale(int range, QImage &img)
{
    int scale=img.width()<500?1:2;
    QPainter painter(&img);
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    painter.drawLine(QLine(0,img.height()/2,img.width(),img.height()/2));
    painter.drawLine(QLine(img.width()/2,0,img.width()/2,img.height()));

    int segmentCount=range/10;
    for(int i=0;i<=segmentCount*2;i++)
    {
        if(i!=segmentCount)
        {
            painter.drawLine(QLine((float(img.width()-scale))/(segmentCount*2)*i+scale/2,img.height()/2-2*scale,(float(img.width()-scale))/(segmentCount*2)*i+scale/2,img.height()/2+2*scale));
            painter.drawLine(QLine(img.width()/2-2*scale,(float(img.height()-scale))/(segmentCount*2)*i+scale/2,img.width()/2+2*scale,(float(img.height()-scale))/(segmentCount*2)*i+scale/2));
        }
    }
}

void AnalysisSvc::drawRoundCrossPixScale(int range, QImage &img)
{
    int scale=img.width()<500?1:2;
    QPainter painter(&img);

    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    painter.drawLine(QLine(0,img.height()/2,img.width(),img.height()/2));
    painter.drawLine(QLine(img.width()/2,0,img.width()/2,img.height()));

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
        int radius=float(img.width())/2/segmentCount*i-scale/2;
        painter.drawEllipse({img.height()/2,img.width()/2},radius,radius);
    }

    qreal radius=img.width()/2;
    for(int i=1;i<12;i++)
    {
        if(i%3!=0)
        {
            auto angle=M_PI/6*i;
            painter.drawLine(QLine{img.width()/2,img.height()/2,int(img.width()/2+radius*qSin(angle)),int(img.height()/2+radius*qCos(angle))});
        }
    }
}

void AnalysisSvc::drawText(QVector<int> values,QVector<QPointF> locs,int range,int OS_OD,QImage& img)
{
    img.fill(qRgb(255, 255, 255));
    drawPixScale(range,img);
    QPainter painter(&img);
    int fontPixSize=img.width()/18;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);
    QPoint blindDot;
    if(OS_OD==0) blindDot={-14,0};else blindDot={14,0};
    auto pixLoc=convertDegLocToPixLoc(blindDot,range,img);
    QImage blindImage(":/grays/SE3.bmp");
    float scale;
    if(img.width()<=400){ scale=1;}
    else scale=3;
    blindImage=blindImage.scaled(blindImage.width()*scale,blindImage.height()*scale);
    painter.drawImage(QPoint{pixLoc.x()-blindImage.width()/2,pixLoc.y()-blindImage.height()/2},blindImage);

    for(int i=0;i<locs.length()&&i<values.length();i++)             //画DB图
    {
        if(values[i]==-99) continue;
        auto pixLoc=convertDegLocToPixLoc(locs[i],range,img);
        const QRect rectangle = QRect(pixLoc.x()-fontPixSize*1.6*0.4, pixLoc.y()-fontPixSize*0.8/2, fontPixSize*1.6,fontPixSize*0.8);
        painter.drawText(rectangle,Qt::AlignCenter,QString::number(values[i]));
        img.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000); //标个小红点
    }
}

void AnalysisSvc::drawGray(QVector<int> values, QVector<QPointF> locs, int range, int innerRange, QImage& img)
{
    img.fill(qRgb(255, 255, 255));
    drawPixScale(range,img);
    QPainter painter(&img);
    int gap= range/15;
    int left_x_axis=0,right_x_axis=0,up_y_axis=0,down_y_axis=0;
    for(auto &i:locs)
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
            if(innerRange!=0){answerInner=(float(qPow(qAbs(i)+2,2))+float(qPow(qAbs(j)+2,2)))/(innerRange*innerRange);}
            if((answerOutter<=1.0)&&(answerInner>=1.0))
                paintLocs.append(QPoint(j,i));
        }
    }

    for(int i=0;i<paintLocs.length();i++)
    {
        QVector<QPair<int,int>> distIndexList;
        for(int j=0;j<locs.length();j++)
        {
            auto dot=locs[j].toPoint();
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
                totalValue=values[index];
                totalDist=1;
                break;
            }
            totalValue+=(float(values[index])/qSqrt(interpolationDots[j].second));
            totalDist+=1.0f/qSqrt(interpolationDots[j].second);
        }
        float interpolVal=totalValue/totalDist;
        int grayVal=qCeil(interpolVal/5);
        if(grayVal>9) grayVal=9;



       QPoint tempPoint;
       if(paintLocs[i].x()>0) tempPoint.setX(paintLocs[i].x()+2); else tempPoint.setX(paintLocs[i].x()-2);
       if(paintLocs[i].y()>0) tempPoint.setY(paintLocs[i].y()+2); else tempPoint.setY(paintLocs[i].y()-2);
       auto pixLoc=convertDegLocToPixLoc(tempPoint,range,img);
       QString path=QString(":/grays/Gray")+QString::number(grayVal)+".bmp";
       QImage image(path);
       float scale;
       if(img.width()<=400){scale=1;}
       else scale=2;
       auto scaledImage=image.scaled(image.width()*scale,image.height()*scale);
       QPoint tempPixLoc={pixLoc.x()-scaledImage.width()/2,pixLoc.y()-scaledImage.height()/2};
       painter.drawImage(tempPixLoc,scaledImage);
    }
}

void AnalysisSvc::drawPE(QVector<int> values, QVector<QPointF> locs, int range,QImage& img)
{
    img.fill(qRgb(255, 255, 255));
    drawPixScale(range,img);
    QPainter painter(&img);
    float scale;
    if(img.width()<150) scale=1;
    else if(img.width()<300) scale=2;
    else scale=4;
    for(int i=0;i<locs.length()&&i<values.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(locs[i],range,img);
        QString path=QString(":/grays/PE")+QString::number(values[i])+".bmp";
        QImage image(path);
        auto scaledImage=image.scaled(image.width()*scale,image.height()*scale);
        QPoint tempPixLoc={pixLoc.x()-scaledImage.width()/2,pixLoc.y()-scaledImage.height()/2};
        painter.drawImage(tempPixLoc,scaledImage);
    }
}

void AnalysisSvc::drawDefectDepth(QVector<int> values, QVector<QPointF> locs, int range, QImage& img)
{
    img.fill(qRgb(255, 255, 255));
    drawPixScale(range,img);
    QPainter painter(&img);
    int fontPixSize=img.width()/18;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);

    for(int i=0;i<locs.length()&&i<values.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(locs[i],range,img);
        if(values[i]==-99) continue;
        if(values[i]>-4)
        {
            float scale;
            if(img.width()<400)
            {
//                scale=qFloor(float(m_image.width())/240);
//                if(scale<1) scale=1;
                scale=1;
            }
            else scale=2;
            QImage image(":/grays/DE0.bmp");
            image=image.scaled(image.width()*scale,image.height()*scale);
            painter.drawImage(QPointF{pixLoc.x()-image.width()*0.48,pixLoc.y()-image.height()*0.50},image);
        }
        else
        {
            const QRect rectangle = QRect(pixLoc.x()-fontPixSize*1.6*0.45, pixLoc.y()-fontPixSize*0.8/2, fontPixSize*1.6,fontPixSize*0.8);
            painter.drawText(rectangle,Qt::AlignCenter,QString::number(-values[i]));
            img.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000);   //标个小红点
        }
    }
}

void AnalysisSvc::drawScreening(QVector<int> values, QVector<QPointF> locs, int range, int OS_OD,QImage& img)
{
    img.fill(qRgb(255, 255, 255));
    drawPixScale(range,img);
    QPainter painter(&img);
    QPoint blindDot;
    if(OS_OD==0) blindDot={-14,0};else blindDot={14,0};
    auto pixLoc=convertDegLocToPixLoc(blindDot,range,img);
    QImage blindImage(":/grays/SE3.bmp");
    QImage screenImageSE0(":/grays/SE0.bmp");
    QImage screenImageSE1(":/grays/SE1.bmp");
    QImage screenImageSE2(":/grays/SE2.bmp");
    int scale=img.width()<=900?1:2;
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
    for(int i=0;i<locs.length()&&i<values.length();i++)
    {
        auto pixLoc=convertDegLocToPixLoc(locs[i],range,img);
        auto imageLoc=QPoint{pixLoc.x()-screenImageSE0.width()/2,pixLoc.y()-screenImageSE0.height()/2};
        switch (values[i])
        {
        case 0:painter.drawImage(imageLoc,screenImageSE2);break;
        case 1:painter.drawImage(imageLoc,screenImageSE1);break;
        case 2:painter.drawImage(imageLoc,screenImageSE0);break;
        default:break;
        }
    }
}

void AnalysisSvc::drawDynamic(QVector<QPointF> values, int range, QImage& img)
{
//    QImage img=QImage({imageSize,imageSize}, QImage::Format_RGB32);
    img.fill(qRgb(255, 255, 255));
    drawRoundCrossPixScale(range,img);
    int scale=img.width()<=900?1:2;
    QPainter painter(&img);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    qDebug()<<values.length();
    for(int i=0;i<values.length();i++)
    {
        auto begin=convertDegLocToPixLoc(values[i],range,img);
        auto end=convertDegLocToPixLoc(values[(i+1)%values.length()],range,img);
        painter.drawLine(QLine(begin.x(),begin.y(),end.x(),end.y()));
        painter.drawEllipse(begin,3*scale,3*scale);
    }
}

void AnalysisSvc::drawFixationDeviation(QVector<int> values, QImage& img)
{
    int scale=2;
//    m_isPreview?scale=1:scale=2;
//    auto dots=m_checkResult->getResultData()->getFixationDeviation();
//    dots={5,-5,5,-5,5,-5,13,13,13,-13,-13,-13};
//    QImage image(322*scale,27*scale, QImage::Format_RGB32);
    img.fill(qRgb(255, 255, 255));
    QPainter painter(&img);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.setPen({Qt::black,float(scale)});
    painter.drawLine(QLine(scale/2,0,scale/2,27*scale));
    painter.drawLine(QLine(img.width()-scale/2,0,img.width()-scale/2,27*scale));
    painter.drawLine(QLine(0,27*scale/2,img.width()-scale/2,27*scale/2));
    auto convertY=[=](int p)->int{return 13-p;};
    for(int i=0;i<values.length();i++)
    {
        int dot=convertY(values[i]);
        painter.drawLine(QLine((i+1.5)*scale,27*scale/2,(i+1.5)*scale,dot*scale+1));
    }
}


QPoint AnalysisSvc::convertDegLocToPixLoc(QPointF DegLoc,int range,QImage img)
{
    float pixPerDegW=float(img.width()/2)/range;
    float pixPerDegH=float(img.height()/2)/range;
    return QPoint(img.width()/2+DegLoc.x()*pixPerDegW,img.height()/2-DegLoc.y()*pixPerDegH);
}




//void AnalysisSvc::dynamicAnalysis()
//{

//}

}
