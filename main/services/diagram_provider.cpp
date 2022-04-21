#include "diagram_provider.h"
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QLine>
DiagramProvider::DiagramProvider(QObject *parent) : QObject(parent)
{

}

void DiagramProvider::drawDiagram(QString name,int os_od,int range, QVariantList dotList, QVariantList values)
{
    QSize imageSize(250,250);
    int fontPixSize=14;
    QImage image(250, 250, QImage::Format_RGB32);
    image.fill(qRgb(255, 255, 255));


    float pixPerDegW=float(imageSize.width()/2)/range;
    float pixPerDegH=float(imageSize.width()/2)/range;
    auto convertDegLocToPixLoc=[&](QPoint DegLoc){return QPoint(image.width()/2+DegLoc.x()*pixPerDegW,image.height()/2-DegLoc.y()*pixPerDegH);};


    QPainter painter(&image);
    painter.setBackground(QBrush(QColor("white")));
    painter.setBrush(QBrush(QColor("black")));
    painter.drawLine(QLine(0,image.height()/2,image.width(),image.height()/2));
    painter.drawLine(QLine(image.width()/2,0,image.width()/2,image.height()));

    for(int i=0;i<=6;i++)
    {
        if(i!=3)
        {
            painter.drawLine(QLine((float(image.width()-1))/6*i,image.height()/2-2,(float(image.width()-1))/6*i,image.height()/2+2));
            painter.drawLine(QLine(image.width()/2-2,(float(image.height()-1))/6*i,image.width()/2+2,(float(image.height()-1))/6*i));
        }
    }

    qDebug()<<"print locloclcolcocl";
    qDebug()<<"print locloclcolcocl";
    qDebug()<<"print locloclcolcocl";
    qDebug()<<"print locloclcolcocl";
    qDebug()<<range;
    qDebug()<<pixPerDegW;

    QFont font("consolas");
    font.setPixelSize(fontPixSize);
    painter.setFont(font);

    for(int i=0;i<dotList.length()&&i<values.length();i++)
    {
        auto temp=dotList[i].toPointF();
        auto pixLoc=convertDegLocToPixLoc(temp.toPoint());
        qDebug()<<QString("test result:x:%1,y:%2,pixLocX:%3,pixLocY:%4,values:%5.").arg(temp.x()).arg(temp.y()).arg(pixLoc.x()).arg(pixLoc.y()).arg(values[i].toInt());
        painter.drawText(QPoint(pixLoc.x()-fontPixSize/2,pixLoc.y()+fontPixSize*0.4),QString::number(values[i].toInt()));
        image.setPixel(pixLoc.x(),pixLoc.y(),0xFFFF0000);
//        painter.drawImage(pixLoc,QImage(":/grays/Gray2.bmp"));
    }
//    QImage image2("./temp/btn_edit.png");
//    QImage image2(":/grays/Gray5.bmp");
//    qDebug()<<image2.pixel(4,4);
//    qDebug()<<image2.pixel(0,0);
//     painter.drawLine(QPoint(100,100),QPoint(200,200));
//     painter.drawImage(QRect(100,100,200,200),image2,QRect(0,0,4,4));
//     painter.drawImage(QPoint(100,100),image2);
//    painter.drawText(QPointF(22,33),"mmmmm");



    qDebug()<<image.save("./temp/30-2.bmp","bmp");
    qDebug()<<"save complete";
//    qDebug()<<name;
//    for(auto& i:dotList)
//    {
//        auto temp=i.toPointF();
//        qDebug()<<temp.x();
//        qDebug()<<temp.y();
//    }

//    for(auto& i:values)
//    {
//        auto temp=i.toInt();
//        qDebug()<<temp;
    //    }
}

//QPointF DiagramProvider::convertDegLocToPixLoc(QPoint DegLoc)
//{
//    return QPointF()
//}
