#include "analysis_provider.h"
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
#include <perimeter/main/viewModel/analysisResult.h>

namespace Perimeter {

AnalysisProvider::AnalysisProvider(QObject *parent) : QObject(parent)
{
    QFile jsonFile("./data.json");
    if( !jsonFile.open(QIODevice::ReadOnly)) {qDebug() << "read file error!";return;}
    QJsonParseError jsonParserError;
    auto JsonDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
    m_jsonArray=JsonDoc.array();
    jsonFile.close();

}

AnalysisProvider::~AnalysisProvider()
{

}


QObject* AnalysisProvider::runProcess(int printType,PatientVm *patient, CheckResultVm *checkResult, QObject *program)
{
    m_type=program->property("type").toInt();
    m_printType=printType;
    m_patient=patient;
    m_checkResult=checkResult;
    if(m_type!=2)
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
        MoveProgramVM* moveProgram=static_cast<MoveProgramVM*>(program);
        auto dotList=moveProgram->getDots();
        for(int i=0;i<dotList.length();i++){m_dotList[i]=dotList[i].toPointF();}
        auto range=moveProgram->getParams()->getRange();
        m_innerRange=range[0];
        m_range=range[1];
    }
    m_os_od=checkResult->getOS_OD();
    auto values=checkResult->getResultData()->getCheckData();
    m_values.resize(values.length());
    for(int i=0;i<values.length();i++){m_values[i]=values[i].toInt();}

    analysis();
    DrawDiagram();
    return new AnalysisResult(m_md,m_psd,m_VFI,m_GHT);
}


QPointF AnalysisProvider::getClickDot(float MouseX, float MouseY,float width,float height)
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


QPointF AnalysisProvider::getPixFromPoint(QPointF point, float width, float height)
{

    qDebug()<<QString("x:nearestDot:%1,y:nearestDot:%2").arg(QString::number(point.x())).arg(QString::number(point.y()));

    float pixPerDegW=float(width/2)/m_range;
    float pixPerDegH=float(height/2)/m_range;
    return QPointF(width/2+(point.x()+0.0)*pixPerDegW,height/2-(point.y()-0.0)*pixPerDegH);
}



void AnalysisProvider::drawPixScale()
{
    QPainter painter(&m_image);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.drawLine(QLine(0,m_image.height()/2,m_image.width(),m_image.height()/2));
    painter.drawLine(QLine(m_image.width()/2,0,m_image.width()/2,m_image.height()));

    int segmentCount=m_range/10;
    for(int i=0;i<=segmentCount*2;i++)
    {
        if(i!=segmentCount)
        {
            painter.drawLine(QLine((float(m_image.width()-1))/(segmentCount*2)*i,m_image.height()/2-2,(float(m_image.width()-1))/(segmentCount*2)*i,m_image.height()/2+2));
            painter.drawLine(QLine(m_image.width()/2-2,(float(m_image.height()-1))/(segmentCount*2)*i,m_image.width()/2+2,(float(m_image.height()-1))/(segmentCount*2)*i));
        }
    }
}

void AnalysisProvider::drawDBText()
{
    QPainter painter(&m_image);
    int fontPixSize=12;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);
    for(int i=0;i<m_dotList.length()&&i<m_values.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(m_dotList[i]);
        const QRect rectangle = QRect(pixLoc.x()-fontPixSize*1.6*0.4, pixLoc.y()-fontPixSize*0.8/2, fontPixSize*1.6,fontPixSize*0.8);
        painter.drawText(rectangle,Qt::AlignCenter,QString::number(m_values[i]));
        m_image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000); //标个小红点
    }
}

void AnalysisProvider::drawDevDBText(QVector<int> values)
{
    QPainter painter(&m_image);
    int fontPixSize=12;
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

void AnalysisProvider::drawGray()
{
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
                totalValue=m_values[index];
                totalDist=1;
                break;
            }
            totalValue+=(float(m_values[index])/qSqrt(interpolationDots[j].second));
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
       QPoint tempPixLoc={pixLoc.x()-image.width()/2,pixLoc.y()-image.height()/2};
       painter.drawImage(tempPixLoc,image);
    }
}

void AnalysisProvider::drawPE(QVector<int> values)
{
    QPainter painter(&m_image);
    for(int i=0;i<m_dotList.length()&&i<m_values.length();i++)             //画DB图
    {
        auto pixLoc=convertDegLocToPixLoc(m_dotList[i]);
        QString path=QString(":/grays/PE")+QString::number(values[i])+".bmp";
        QImage image(path);
        QPoint tempPixLoc={pixLoc.x()-image.width()/2,pixLoc.y()-image.height()/2};
        painter.drawImage(tempPixLoc,image);
    }
}

QPoint AnalysisProvider::convertDegLocToPixLoc(QPointF DegLoc)
{
    float pixPerDegW=float(m_imageSize.width()/2)/m_range;
    float pixPerDegH=float(m_imageSize.width()/2)/m_range;
    return QPoint(m_image.width()/2+DegLoc.x()*pixPerDegW,m_image.height()/2-DegLoc.y()*pixPerDegH);
}

void AnalysisProvider::DrawDiagram()
{

    drawDBDiagram();
    drawGrayDiagram();
    drawTotalDeviation();
    drawPatternDeviation();
    drawTotalPE();
    drawPatternPE();
}

void AnalysisProvider::drawDBDiagram()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    drawDBText();
    m_image.save("./temp/dBDiagram.bmp","bmp");
}

void AnalysisProvider::drawGrayDiagram()
{
     m_image.fill(qRgb(255, 255, 255));
     drawPixScale();
     drawGray();
     m_image.save("./temp/gray.bmp","bmp");
}

void AnalysisProvider::drawTotalDeviation()
{

    m_image.fill(qRgb(255, 255, 255));
    drawDevDBText(m_dev);
    drawPixScale();
    m_image.save("./temp/TotalDeviation.bmp","bmp");
}


void AnalysisProvider::drawPatternDeviation()
{

    m_image.fill(qRgb(255, 255, 255));
    drawDevDBText(m_mDev);
    drawPixScale();
    m_image.save("./temp/PatterDeviation.bmp","bmp");

}

void AnalysisProvider::drawTotalPE()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPE(m_peDev);
    drawPixScale();
    m_image.save("./temp/TotalPE.bmp","bmp");
}

void AnalysisProvider::drawPatternPE()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPE(m_peMDev);
    drawPixScale();
    m_image.save("./temp/PatternPE.bmp","bmp");
}

void AnalysisProvider::analysis()
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

    auto birthDate=QDate::fromString(m_patient->getBirthDate(),"yyyy-MM-dd");
    auto currentDate=QDate::currentDate();

    int m_patientAge = currentDate.year() - birthDate.year();
    if (currentDate.month() < birthDate.month() || (currentDate.month() == birthDate.month() && currentDate.day() < birthDate.day())) { m_patientAge--;}


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


        if(m_sv[i]>0)  {m_dev[i]=m_values[i]-m_sv[i];}                                                   //dev 盲点
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
            m_vfiRingTest[ringIndex]+=m_values[i];
        }
    }


    float vfih=0,vfid=0;
    for(int i=0;i<5;i++)
    {
        if(m_vfiRingStandard[i]!=0)
        {
            qDebug()<<m_vfiRingTest[i];
            qDebug()<<m_vfiRingStandard[i];
            float temp=float(m_vfiRingTest[i])/(m_vfiRingStandard[i]+0.0001);
            qDebug()<<temp;
            if(temp>1){temp=1;}
            vfih+=m_VFI_Weight[i]*temp;
            vfid+=m_VFI_Weight[i];
        }
    }
    qDebug()<<vfih;
    qDebug()<<vfid;

    m_VFI=vfih/vfid;                                     //vfi
    qDebug()<<m_VFI;

    for(auto& i: m_dev)
    {
        if(i!=-99) m_md+=i;
    }
    m_md/=(m_dev.length()-2);    //扣除盲点

    qDebug()<<m_md;

    for(auto& i: m_dev)
    {
        if(i!=-99) m_psd+=pow(i-m_md,2);
    }
    m_psd=sqrt(m_psd/(m_dev.length()-2));

    qDebug()<<m_psd;

    int md=round(m_md+m_psd);
    qDebug()<<md;

    for(int i=0;i<m_dev.length();i++)
    {
        if(m_dev[i]==-99)
            m_mDev[i]=-99;
        else
            m_mDev[i]=m_dev[i]-md;
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


void AnalysisProvider::drawDiagram2(QString name,int os_od ,int range, QVariantList dotList, QVariantList values)
{
    qDebug()<<range;
    QSize imageSize(240,240);
//    QSize imageSize(2000,2000);
    int fontPixSize=12;
    QImage image(imageSize, QImage::Format_RGB32);
    image.fill(qRgb(255, 255, 255));


    float pixPerDegW=float(imageSize.width()/2)/range;
    float pixPerDegH=float(imageSize.width()/2)/range;
    auto convertDegLocToPixLoc=[&](QPoint DegLoc)->QPoint{return QPoint(image.width()/2+DegLoc.x()*pixPerDegW,image.height()/2-DegLoc.y()*pixPerDegH);};


    QPainter painter(&image);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.drawLine(QLine(0,image.height()/2,image.width(),image.height()/2));
    painter.drawLine(QLine(image.width()/2,0,image.width()/2,image.height()));

    int segmentCount=range/10;
    for(int i=0;i<=segmentCount*2;i++)
    {
        if(i!=segmentCount)
        {
            painter.drawLine(QLine((float(image.width()-1))/(segmentCount*2)*i,image.height()/2-2,(float(image.width()-1))/(segmentCount*2)*i,image.height()/2+2));
            painter.drawLine(QLine(image.width()/2-2,(float(image.height()-1))/(segmentCount*2)*i,image.width()/2+2,(float(image.height()-1))/(segmentCount*2)*i));
        }
    }

    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);



//    for(int i=0;i<dotList.length()&&i<values.length();i++)             //画DB图
//    {
//        auto temp=dotList[i].toPointF();
//        auto pixLoc=convertDegLocToPixLoc(temp.toPoint());
//        painter.drawText(QPoint(pixLoc.x()-fontPixSize/2,pixLoc.y()+fontPixSize*0.4),QString::number(values[i].toInt()));
//        image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000);
//    }



    int gap= range/15;
    int left_x_axis=0,right_x_axis=0,up_y_axis=0,down_y_axis=0;
    for(auto &i:dotList)
    {
        auto x=i.toPointF().x();
        auto y=i.toPointF().y();
        if(x<left_x_axis) left_x_axis=x;
        if(x>right_x_axis) right_x_axis=x;
        if(y>up_y_axis) up_y_axis=y;
        if(y<down_y_axis) down_y_axis=y;
    }



    int rangeX=qMax(qAbs(left_x_axis),right_x_axis)+1.5*gap;;int rangeY=qMax(qAbs(down_y_axis),up_y_axis)+1.5*gap;
     qDebug()<<QString("rangeX:%1,rangeY:%2").arg(rangeX).arg(rangeY);


//    left_x_axis-=gap;right_x_axis+=gap;up_y_axis+=gap;down_y_axis-=gap;

    qDebug()<<QString("left:%1,right:%2,up:%3,down:%4").
              arg(QString::number(left_x_axis)).
              arg(QString::number(right_x_axis)).
              arg(QString::number(up_y_axis)).
              arg(QString::number(down_y_axis));


    QVector<QPoint> paintLocs;
    for(int i=up_y_axis;i>=down_y_axis;i-=gap)
    {
        for(int j=left_x_axis;j<=right_x_axis;j+=gap)
        {
//             float answer=float(i*i)/(rangeX*rangeX)+float(j*j)/(rangeY*rangeY);
             float answer=float(qPow(qAbs(i)+2,2))/(rangeX*rangeX)+float(qPow(qAbs(j)+2,2))/(rangeY*rangeY);
             if(answer<=1.0) paintLocs.append(QPoint(j,i));
        }
    }





//    for(auto &i:paintLocs)
//    {
//        auto pixLoc=convertDegLocToPixLoc(i);
//        if(i.x()==1||i.x()==-1||i.y()==1||i.y()==-1) continue;
//        QPoint tempPixLoc={pixLoc.x()-fontPixSize,pixLoc.y()-fontPixSize};
//        painter.drawText(tempPixLoc,QString("%1,%2").arg(i.x()).arg(i.y()));
//    }

//    QVector<QPoint> paintLocs;
//    paintLocs.append(QPoint(-19,17));
//     paintLocs.append(QPoint(5,9));
//      paintLocs.append(QPoint(-3,-3));


    for(int i=0;i<paintLocs.length();i++)
    {
//        if(paintLocs[i].x()==1||paintLocs[i].x()==-1||paintLocs[i].y()==1||paintLocs[i].y()==-1) continue;

        QVector<QPair<int,int>> distIndexList;

        for(int j=0;j<dotList.length();j++)
        {
            auto dot=dotList[j].toPoint();
//            qDebug()<<paintLocs[i].x();
//            qDebug()<<dot.x();
//            qDebug()<<qPow(paintLocs[i].x()-dot.x(),2);
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

//        for(auto& j:interpolationDots)
//        {
//            auto p=dotList[j.first].toPoint();
//            qDebug()<<QString("%1:%2").arg(p.x()).arg(p.y());
//        }

        float totalValue=0,totalDist=0;
        for(int j=0;j<interpolationDots.length();j++)
        {
            int index=interpolationDots[j].first;
            if(interpolationDots[j].second==0)
            {
                totalValue=values[index].toFloat();
                totalDist=1;
                break;
            }
//            qDebug()<<values[index].toFloat();
            totalValue+=(values[index].toFloat()/qSqrt(interpolationDots[j].second));
            totalDist+=1/qSqrt(interpolationDots[j].second);
        }
        float interpolVal=totalValue/totalDist;
        int grayVal=qCeil(interpolVal/5);
        if(grayVal>9) grayVal=9;



       QPoint tempPoint;
       if(paintLocs[i].x()>0) tempPoint.setX(paintLocs[i].x()+2); else tempPoint.setX(paintLocs[i].x()-2);
       if(paintLocs[i].y()>0) tempPoint.setY(paintLocs[i].y()+2); else tempPoint.setY(paintLocs[i].y()-2);
       auto pixLoc=convertDegLocToPixLoc(tempPoint);
//       auto pixLoc=convertDegLocToPixLoc(paintLocs[i]);
       QString path=QString(":/grays/Gray")+QString::number(grayVal)+".bmp";
//       qDebug()<<path;
       QImage image(path);
       QPoint tempPixLoc={pixLoc.x()-image.width()/2,pixLoc.y()-image.height()/2};
       painter.drawImage(tempPixLoc,image);

    }


    qDebug()<<image.save("./temp/pic_1.bmp","bmp");
    qDebug()<<"save complete";




}




}
