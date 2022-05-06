#ifndef DIGRAM_PROVIDER_H
#define DIGRAM_PROVIDER_H

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QImage>
#include <QSize>
#include <functional>
#include <perimeter/main/viewModel/programVm.h>
#include <perimeter/main/viewModel/patientVm.h>
#include <perimeter/main/viewModel/checkResultVm.h>

namespace Perimeter {
class DiagramProvider : public QObject
{
    Q_OBJECT

public:
    explicit DiagramProvider(QObject *parent = nullptr);
    Q_INVOKABLE void drawDiagram(int printType,int os_od,int innerRange,int range,QVariantList dotList,QVariantList values);
//    Q_INVOKABLE void drawDiagram2(int printType,int os_od,int innerRange,int range,QVariantList dotList,QVariantList values);



signals:

public slots:
private:
    QSize m_imageSize=QSize(240,240);
    QImage m_image=QImage(m_imageSize, QImage::Format_RGB32);
    int m_printType;   //0,30-2
    int m_os_od;
    int m_range;
    int m_innerRange;
    int m_patientAge;

    QVariantList m_dotList;
    QVariantList m_values;

    CheckResultVm* m_checkResult=nullptr;
    PatientVm* m_patient=nullptr;
    QObject* m_program=nullptr;


private:
    void drawPixScale();
    void drawDBText();
    void drawGray();
    QPoint convertDegLocToPixLoc(QPoint DegLoc);
    void drawDBDiagram();
    void drawGrayDiagram();
    void drawTotalDeviation();
    void drawPatternDeviation();


public:
     Q_INVOKABLE void drawDiagram2(QString name,int os_od,int range,QVariantList dotList,QVariantList values);
};
}
#endif // DIGRAM_PROVIDER_H
