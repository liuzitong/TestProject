#include "diagram_provider.h"
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QLine>
#include <QtMath>
#include <QVector>
#include <QPair>
#include <qalgorithms.h>
#include <algorithm>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace Perimeter {

DiagramProvider::DiagramProvider(QObject *parent) : QObject(parent)
{

}

void DiagramProvider::drawDiagram(int printType, int os_od,int innerRange, int range, QVariantList dotList, QVariantList values)
{
    m_printType=printType;
    m_os_od=os_od;
    m_range=range;
    m_dotList=dotList;
    m_values=values;
    m_innerRange=innerRange;
    drawDBDiagram();
    drawGrayDiagram();
}


void DiagramProvider::drawPixScale()
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

void DiagramProvider::drawDBText()
{
    QPainter painter(&m_image);
    int fontPixSize=12;
    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);
    for(int i=0;i<m_dotList.length()&&i<m_values.length();i++)             //画DB图
    {
        auto temp=m_dotList[i].toPointF();
        auto pixLoc=convertDegLocToPixLoc(temp.toPoint());
        painter.drawText(QPoint(pixLoc.x()-fontPixSize/2,pixLoc.y()+fontPixSize*0.4),QString::number(m_values[i].toInt()));
        m_image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000);
    }
}

void DiagramProvider::drawGray()
{
    QPainter painter(&m_image);
    int gap= m_range/15;
    int left_x_axis=0,right_x_axis=0,up_y_axis=0,down_y_axis=0;
    for(auto &i:m_dotList)
    {
        auto x=i.toPointF().x();auto y=i.toPointF().y();
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
            float answerInner=(float(qPow(qAbs(i)+2,2))+float(qPow(qAbs(j)+2,2)))/(m_innerRange*m_innerRange);
            qDebug()<<QString::number(answerInner);
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
                totalValue=m_values[index].toFloat();
                totalDist=1;
                break;
            }
            totalValue+=(m_values[index].toFloat()/qSqrt(interpolationDots[j].second));
            totalDist+=1/qSqrt(interpolationDots[j].second);
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

QPoint DiagramProvider::convertDegLocToPixLoc(QPoint DegLoc)
{
    float pixPerDegW=float(m_imageSize.width()/2)/m_range;
    float pixPerDegH=float(m_imageSize.width()/2)/m_range;
    return QPoint(m_image.width()/2+DegLoc.x()*pixPerDegW,m_image.height()/2-DegLoc.y()*pixPerDegH);
}

void DiagramProvider::drawDBDiagram()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    drawDBText();
    m_image.save("./temp/dBDiagram.bmp","bmp");
}

void DiagramProvider::drawGrayDiagram()
{
     m_image.fill(qRgb(255, 255, 255));
     drawPixScale();
     drawGray();
     m_image.save("./temp/gray.bmp","bmp");
}

void DiagramProvider::drawTotalDeviation()
{
    m_image.fill(qRgb(255, 255, 255));


    QFile jsonFile("./data.json");
    if( !jsonFile.open(QIODevice::ReadOnly)) {qDebug() << "read file error!";return;}
    QJsonParseError jsonParserError;
    QJsonDocument outDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
    QJsonArray jsonArray=outDoc.array();




    for(auto i:jsonArray)
    {

        auto jo=i.toObject();
//        auto keys=jo.keys();
//        for(auto&j:keys)
//        {
//            qDebug()<<j;

           qDebug()<<jo["name"].toString();
//           if(jo["name"])
           auto arr=jo["data"].toArray();
           QString str;
           for(auto j:arr)
           {
               str+=QString::number(j.toInt())+" ";
           }
           qDebug()<<str;
    }




    jsonFile.close();


    jsonFile.close();
    drawPixScale();
    m_image.save("./temp/TotalDeviation.bmp","bmp");
}

void DiagramProvider::drawPatternDeviation()
{
    m_image.fill(qRgb(255, 255, 255));
    drawPixScale();
    m_image.save("./temp/PatternDeviation.bmp","bmp");
}






void DiagramProvider::drawDiagram2(QString name,int os_od ,int range, QVariantList dotList, QVariantList values)
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
